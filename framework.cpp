#include "framework.h"

Framework::Framework() : m_Running(true), m_Options(), m_LoginServer(), m_Clock()
{

}

int Framework::Run(int argc, char **argv)
{
    printf(">> GW2SEX - Guild Wars 2 Server Emulator for XNIX\n");
    printf(">> Some ideas taken from http://poke152.blogspot.com.au/2012_12_01_archive.html\n");
    printf(">> Developed by Nomelx\n\n");

    System::IgnoreSigPipe();    // For debugging with GDB
    System::CheckRoot();        // Servers should have a unique user to run the application

    if (!Configure(argc, argv)) {
        printf("!) Server attempting to start with incorrect configuration.\n");
        return 1;
    }


    if (strcmp(m_Options.m_ServerMode, "gateway") == 0) {
        return RunGateway();
    }

    return 0;
}

int Framework::RunGateway()
{
    int gatewayPort = GetSettingInteger("networking.login_server.port");
    const char* gatewayCertificate = GetSettingString("networking.login_server.certFile");
    const char* gatewayPrivateKey = GetSettingString("networking.login_server.keyFile");

    printf("Starting %s\n", m_Options.m_ServerName);
    printf("Starting login gateway server on port %d\n", gatewayPort);

    // This will spawn off the login / server socket thread
    if (!m_LoginServer.Startup(gatewayPort,
                               gatewayCertificate,
                               gatewayPrivateKey,
                               m_Options.m_MITMMode)) {
        m_LoginServer.Shutdown();
        printf("!) Unable to create login server (listner).\n");
        return 1;
    }

    // Start the server clock for regulation, this does not regulate the listning socket.
    // We would idealy use this to make sure we are not spending too much time doing expensive operations.
    m_Clock.Start();

    // Main thread loop, do any non blocking functions here.
    while (m_Running) {

        // Poll the users, this is where all player managment is done.
        m_LoginServer.Update();

        // Update the clock.
        m_Clock.Frame();

    }

    m_LoginServer.Shutdown();

    return 0;
}

bool Framework::Configure(int argc, char **argv)
{
    // Check argc agains the required argument count, we subtract one because the OS includes the running path.
    if ((argc-1)!=REQ_ARGUMENTS_C) {
        ShowUsage();
        return false;
    }

    // Assume arg 1 is the config file.
    if (!Import(argv[1])) {
        return false;
    }

    // Load any missions critical options here.
    sprintf(m_Options.m_ServerName, "%s", GetSettingString("networking.server.name"));
    sprintf(m_Options.m_ServerMode, "%s", GetSettingString("networking.server.mode"));

    m_Options.m_MITMMode = GetSettingBool("networking.server.mitmMode");

    return true;
}

void Framework::ShowUsage() {
    printf("Usage: gw2sex config-file[.cofg]\n");
    printf("\n");
}
