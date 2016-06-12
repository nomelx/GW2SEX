#include "serverconfig.h"

ServerConfig::ServerConfig()
{

}

bool ServerConfig::Import(const char *filename)
{
    try {
        m_ConfigInstance.readFile(filename);
    } catch(const libconfig::FileIOException &ex) {
        printf("<Config::Import> Unable to read configuration file %s, IO Exception\n", filename);
        printf(">> %s\n", ex.what());
        return false;
    } catch(const libconfig::ParseException &pex) {
        printf("<Config::Import> Unable to parse file\n");
        printf(">> Parse error at %s : %d - %s\n", pex.getFile(), pex.getLine(), pex.getError());
        return false;
    }
    return true;
}

bool ServerConfig::Export(const char *filename)
{
    try {
        m_ConfigInstance.writeFile(filename);
    } catch (const libconfig::FileIOException &ex) {
        printf("<Config::Export> Unable to write configuration to %s\n", filename);
        printf(">> %s\n", ex.what());
    }

    return false;
}

const char* ServerConfig::GetSettingString(const char* Key)
{
    try {
        return m_ConfigInstance.lookup(Key).c_str();
    } catch(const libconfig::SettingNotFoundException &nfex) {
        printf("<ServerConfig::GetSettingString> Lookup of %s failed.\n", Key);
    }
    return 0;
}

int ServerConfig::GetSettingInteger(const char *Key)
{
    try {
        int integerBuffer = 0;
        m_ConfigInstance.lookupValue(Key, integerBuffer);
        return integerBuffer;
    } catch(const libconfig::SettingNotFoundException &nfex) {
        printf("<ServerConfig::GetSettingString> Lookup of %s failed.\n", Key);
    }
    return 0;
}

bool ServerConfig::GetSettingBool(const char *Key)
{
    try {
        bool booleanBuffer = false;
        booleanBuffer = m_ConfigInstance.lookup(Key);
        return booleanBuffer;
    } catch(const libconfig::SettingNotFoundException &nfex) {
        printf("<ServerConfig::GetSettingString> Lookup of %s failed.\n", Key);
    }
    return false;
}
