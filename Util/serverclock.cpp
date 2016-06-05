#include "serverclock.h"

ServerClock::ServerClock() : m_cTime(), m_pTime(), m_uDelta(0), m_Frames(0)
{

}

void ServerClock::Start()
{
    clock_gettime(CLOCK_REALTIME, &m_pTime);
    m_Frames = 0;
}

void ServerClock::Frame()
{
    // Update the current frame time
    clock_gettime(CLOCK_REALTIME, &m_cTime);

    // Calculate delta - negate seconds / convert to nano, combine with active nano and calc delta
    long long secondsDelta = (m_cTime.tv_sec-m_pTime.tv_sec)*1000*1000*1000;
    long long nanoDelta = m_cTime.tv_nsec - m_pTime.tv_nsec;
    m_pTime = m_cTime;
    m_FrameDelta = secondsDelta + nanoDelta;
    m_uDelta += m_FrameDelta;

    // Calc Milliseconds
    long micro = m_uDelta / 1000;
    long mili = micro / 1000;

    // Update frames every second
    if (mili >= 1000) {
        //printf("Ticks per second: %d\n", m_Frames);
        m_uDelta = 0;
        m_Frames = 0;
    }

    // Increment frame counter
    m_Frames++;

}
