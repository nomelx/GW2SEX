#include "gw2packet.h"

#include "../3rdParty/rapidxml-1.13/rapidxml_print.hpp"

GW2Packet::GW2Packet(const char* path, int sequence, GW2PacketType type) :m_PacketHeader(),  m_PacketData(),
    m_Sequence(sequence), m_Type(type), m_Document(), m_RequestNode(nullptr), m_Elements()
{
    // Set the request path.
    memset(m_PacketHeader, 0, sizeof(m_PacketHeader));
}

void GW2Packet::AddElement(const char *key, const char *value)
{
    m_Elements.push_back(
                m_Document.allocate_node(rapidxml::node_element, key, value));
}

void GW2Packet::AddAttribute(const char *key, const char *attribute)
{
    for (rapidxml::xml_node<>* element : m_Elements) {
        const char* name = element->name();
        if (strcmp(name, key) == 0) {
            element->append_attribute(
                        m_Document.allocate_attribute(key, attribute));
        }
    }
}

const char *GW2Packet::Payload()
{
    std::string header = "";
    std::string reply = "";

    switch(m_Type) {
        case PT_REQUSET:
            m_RequestNode = m_Document.allocate_node(rapidxml::node_element, "Request");
            break;
    case PT_REPLY:
            m_RequestNode = m_Document.allocate_node(rapidxml::node_element, "Reply");
            header = "STS/1.0 200 OK\r\n";
            header += "s:" + std::to_string(m_Sequence) + "R\r\n";
            break;
    case PT_MESSAGE:
            m_RequestNode = m_Document.allocate_node(rapidxml::node_element, "Message");
            break;
    }

    for (rapidxml::xml_node<>* element : m_Elements) {
        m_RequestNode->append_node(element);
    }

    // Build the reply / request / message body
    m_Document.append_node(m_RequestNode);
    std::string bodyBuffer;
    rapidxml::print(std::back_inserter(bodyBuffer), m_Document, 0);

    // Append the header, rapidxml puts an extra newline at the end that GW2 doesnt like
    header += "l:"+ std::to_string(bodyBuffer.size()-1) + "\r\n";
    header += "\r\n";

    reply += header;
    reply += bodyBuffer;

    FILE* debug = fopen("out.dat", "wb");
    fwrite(reply.c_str(), sizeof(char), reply.size(), debug);
    fclose(debug);

    return reply.c_str();
}
