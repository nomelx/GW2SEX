#ifndef SERVERCLOCK_H
#define SERVERCLOCK_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include <unistd.h>
#include <stdio.h>
#include <time.h>

class ServerClock
{
public:
    ServerClock();
public:
    void Start();
    void Frame();
private:
    timespec m_cTime;
    timespec m_pTime;
    long long m_uDelta;
public:
    int m_Frames;
    long m_FrameDelta;
};

#endif // SERVERCLOCK_H
