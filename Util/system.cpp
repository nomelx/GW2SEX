#include "system.h"

System::System()
{

}

void System::IgnoreSigPipe()
{
    signal(SIGPIPE, SIG_IGN);
}

void System::CheckRoot()
{
    if(geteuid() == 0) {
        printf("!) Running as root, this is not a good idea.\n");
    }
}

void System::DebugDump(const char *Filename, const char *Data, int Length)
{
    FILE* Out = fopen(Filename, "wb");
    fwrite(Data, sizeof(char), Length, Out);
    fclose(Out);
}

void System::DebugWriteMessage(const char *Filename, const char *Data, int Length)
{
    FILE* Out = fopen(Filename, "ab");
    fwrite(Data, sizeof(char), Length, Out);
    fclose(Out);
}
