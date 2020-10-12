/*
 *  Copyright (C) 2019+ WarheadCore
 */

#include "Logo.h"
#include "Log.h"
#include "GitRevision.h"
#include "StringFormat.h"
#include "Config.h"
#include <ace/Version.h>
#include <openssl/opensslv.h>
#include <openssl/crypto.h>
#include <boost/version.hpp>

void Warhead::Logo::Show(char const* applicationName, char const* configName, void(*log)(char const* text))
{
    log(Warhead::StringFormat("%s (%s)", GitRevision::GetFullVersion(), applicationName).c_str());
    log("<Ctrl-C> to stop");
    log("");
    log("  ██╗     ██╗  █████╗  ██████╗  ██╗  ██╗ ███████╗  █████╗  ██████╗");
    log("  ██║     ██║ ██╔══██╗ ██╔══██╗ ██║  ██║ ██╔════╝ ██╔══██╗ ██╔═══██╗");
    log("  ██║     ██║ ███████║ ██████╔╝ ███████║ █████╗   ███████║ ██║   ██║");
    log("  ██║ ██╗ ██║ ██╔══██║ ██╔══██╗ ██╔══██║ ██╔══╝   ██╔══██║ ██║   ██║");
    log("  ╚═██╔═██╔═╝ ██║  ██║ ██║  ██║ ██║  ██║ ███████╗ ██║  ██║ ██████╔═╝");
    log("    ╚═╝ ╚═╝   ╚═╝  ╚═╝ ╚═╝  ╚═╝ ╚═╝  ╚═╝ ╚══════╝ ╚═╝  ╚═╝ ╚═════╝");
    log("                                ██████╗   ██████╗  ██████╗  ███████╗");
    log("                                ██╔═══╝  ██╔═══██╗ ██╔══██╗ ██╔════╝");
    log("                                ██║      ██║   ██║ ██████╔╝ █████╗");
    log("                                ██║      ██║   ██║ ██╔══██╗ ██╔══╝");
    log("                                ╚██████╗ ╚██████╔╝ ██║  ██║ ███████╗");
    log("                                 ╚═════╝  ╚═════╝  ╚═╝  ╚═╝ ╚══════╝");
    log("");
    log(Warhead::StringFormat("> Using configuration file:       %s", configName).c_str());
    log(Warhead::StringFormat("> Using SSL version:              %s (library: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION)).c_str());
    log(Warhead::StringFormat("> Using ACE version:              %s", ACE_VERSION).c_str());
    log(Warhead::StringFormat("> Using Boost version:            %i.%i.%i", BOOST_VERSION / 100000, BOOST_VERSION / 100 % 1000, BOOST_VERSION % 100).c_str());
    log("");
}
