#include "gw2packet.h"

GW2Packet::GW2Packet(const char* path, GW2PacketType type) :m_PacketHeader(),  m_PacketData(),
    m_Type(type), m_Document(), m_RequestNode(nullptr), m_Elements()
{
    // Set the request path.
    memset(m_PacketHeader, 0, sizeof(m_PacketHeader));
    sprintf(m_PacketHeader, "P %s STS/1.0\r\n", path);
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
    switch(m_Type) {
        case PT_REQUSET:
            m_RequestNode = m_Document.allocate_node(rapidxml::node_element, "Request");
            break;
    case PT_REPLY:
            m_RequestNode = m_Document.allocate_node(rapidxml::node_element, "Reply");
            break;
    case PT_MESSAGE:
            m_RequestNode = m_Document.allocate_node(rapidxml::node_element, "Message");
            break;
    }

    for (rapidxml::xml_node<>* element : m_Elements) {
        m_RequestNode->append_node(element);
    }

    m_Document.append_node(m_RequestNode);

    std::stringstream bodyBuffer;
    bodyBuffer << m_Document.document();
    //print(std::back_inserter(bodyBuffer), m_Document, 0);

    return bodyBuffer.str().c_str();
}
