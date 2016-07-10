#ifndef XMLPACKET_H
#define XMLPACKET_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../Util/messagetokenizer.h"
#include "../3rdParty/rapidxml-1.13/rapidxml.hpp"

class XMLPacket
{
public:
    XMLPacket();
public:
    bool    Parse(const char* Data, unsigned int Length);
    bool    Validate();
    void    Clear();
public:
    char    m_Path[256];
    char    m_Data[2056];
    char    m_Meta[256];
    int     m_Length;
    rapidxml::xml_document<> m_XMLDocument;
private:
    bool    m_Ready;
    int     m_DataWritten;
};

#endif // XMLPACKET_H
