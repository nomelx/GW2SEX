#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */


#include <stdio.h>
#include <iostream>
#include <libconfig.h++>

#define MAX_MATCH 4096

class ServerConfig
{
public:
    ServerConfig();
public:
    bool Import(const char* filename);
    bool Export(const char* filename);
public:
    const char* GetSettingString(const char* Key);
    int GetSettingInteger(const char* Key);
    bool GetSettingBool(const char *Key);

private:
    libconfig::Config m_ConfigInstance;
};

#endif // SERVERCONFIG_H
