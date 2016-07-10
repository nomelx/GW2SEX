#include "xmlpacket.h"

XMLPacket::XMLPacket() : m_Path(), m_Data(), m_Meta(), m_Length(0),
    m_XMLDocument(), m_Ready(false), m_DataWritten(0)
{
    memset(m_Path, 0, 256);
    memset(m_Data, 0, 2056);
}

bool XMLPacket::Parse(const char *Data, unsigned int Length)
{
    if (Data == 0 || Length == 0) {
        return false;
    }

    // Copy over the packet to do our parsing
    char* tmpReadBuffer = new char[Length];
    memset(tmpReadBuffer, 0, Length);
    memcpy(tmpReadBuffer, Data, Length);

    // Make sure we are not fucking around with newlines
    MessageTokenizer tokenizer;
    tokenizer.Read(tmpReadBuffer, Length);

    // Read lines and start filling out our xml request
    // We only look for the Path, Sequence and Length; everything else is XML content
    for (unsigned int i = 0; i < tokenizer.m_Lines.size(); i++) {
        MessageLine line = tokenizer.m_Lines.at(i);

        if (line.m_Start[0] == 'P') {
            strncpy(m_Path, line.m_Start+2, line.m_Length-2);
        } else if (line.m_Start[0] == 'l') {
            m_Length = atoi(line.m_Start+2);
        } else if (line.m_Start[0] == 's') {
            strncpy(m_Meta, line.m_Start, line.m_Length);
        } else {
            strncpy(m_Data+m_DataWritten, line.m_Start, line.m_Length);
            m_DataWritten += line.m_Length;
        }

    }

    delete[] tmpReadBuffer;

    return true;
}

bool XMLPacket::Validate()
{
    if (m_Data == 0 || m_DataWritten == 0) {
        return false;
    }

    try {
        m_XMLDocument.parse<0>(m_Data);
        //printf("%s\n", m_XMLDocument.first_node()->name());
        m_Ready = true;
    } catch (rapidxml::parse_error ex) {
        //printf("Rapid XML Error %s [%s]\n", ex.what(), ex.where<char>());
        // This can happen if junk is sent before the XML, has happened every now and then.
        if (m_Data[0] != '<') {
            m_DataWritten = 0;
            memset(m_Data, 0, 2056);
        }
        m_Ready = false;
        return false;
    }

    return true;
}

void XMLPacket::Clear()
{
    memset(m_Path, 0, 256);
    memset(m_Data, 0, 2056);
    memset(m_Meta, 0, 256);
    m_Length = 0;
    m_Ready = false;
    m_DataWritten = 0;
}
