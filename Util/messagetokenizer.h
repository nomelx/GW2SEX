#ifndef MESSAGETOKENIZER_H
#define MESSAGETOKENIZER_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include <vector>

struct MessageLine {
    const char* m_Start;
    int m_Length;
};

class MessageTokenizer
{
public:
    MessageTokenizer();
public:
    void Read(char* Message, int Length);
public:
    std::vector<MessageLine> m_Lines;
};

#endif // MESSAGETOKENIZER_H
