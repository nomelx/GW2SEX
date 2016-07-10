#include "messagetokenizer.h"

MessageTokenizer::MessageTokenizer() : m_Lines()
{

}

void MessageTokenizer::Read(char *Message, int Length)
{
    // First pass change \r to \n
    for (int i = 0; i < Length; i++) {
        if (Message[i] == '\r') {
            Message[i] = '\n';
        }
    }

    // Second pass we gather all the lines ignoring CR & newlines etc
    MessageLine CurrentLine;
    CurrentLine.m_Length = 0;
    CurrentLine.m_Start = 0;

    for (int i = 0; i < Length; i++) {
        // Assume a 0 value means we are past any valuable information.
        if (Message[i] == 0) {
            return;
        }
        if (Message[i] == '\n' || i == Length-1) {
            if (CurrentLine.m_Start) {
                m_Lines.push_back(CurrentLine);
                CurrentLine.m_Length = 0;
                CurrentLine.m_Start = 0;
            }
        } else {
            if (!CurrentLine.m_Start) {
                CurrentLine.m_Start = Message+i;
            }
            CurrentLine.m_Length++;
        }
    }

}

