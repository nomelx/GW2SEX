#ifndef SYSTEM_H
#define SYSTEM_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

class System
{
private:
    System();
public:
    static void IgnoreSigPipe();
    static void CheckRoot();
    static void DebugDump(const char* Filename, const char* Data, int Length);
    static void DebugWriteMessage(const char* Filename, const char* Data, int Length);
};

#endif // SYSTEM_H
