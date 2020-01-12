/*
 *  Copyright (C) 2019+ WarheadCore
 */

#include "Logo.h"
#include "GitRevision.h"
#include "StringFormat.h"
#include <ace/Version.h>
#include <openssl/opensslv.h>
#include <openssl/crypto.h>
#include <boost/version.hpp>

void acore::Logo::Show(std::string const& logger, std::string const& applicationName, std::string configName)
{
    LOG_INFO(logger, "%s (%s)", GitRevision::GetFullVersion(), applicationName.c_str());
    LOG_INFO(logger, "<Ctrl-C> to stop");
    LOG_INFO(logger, "");
    LOG_INFO(logger, "   █████╗ ███████╗███████╗██████╗  ██████╗ ████████╗██╗  ██╗");
    LOG_INFO(logger, "  ██╔══██╗╚══███╔╝██╔════╝██╔══██╗██╔═══██╗╚══██╔══╝██║  ██║");
    LOG_INFO(logger, "  ███████║  ███╔╝ █████╗  ██████╔╝██║   ██║   ██║   ███████║");
    LOG_INFO(logger, "  ██╔══██║ ███╔╝  ██╔══╝  ██╔══██╗██║   ██║   ██║   ██╔══██║");
    LOG_INFO(logger, "  ██║  ██║███████╗███████╗██║  ██║╚██████╔╝   ██║   ██║  ██║");
    LOG_INFO(logger, "  ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝ ╚═════╝    ╚═╝   ╚═╝  ╚═╝");
    LOG_INFO(logger, "                                ██████╗ ██████╗ ██████╗ ███████╗");
    LOG_INFO(logger, "                                ██╔════╝██╔═══██╗██╔══██╗██╔═══╝");
    LOG_INFO(logger, "                                ██║     ██║   ██║██████╔╝█████╗");
    LOG_INFO(logger, "                                ██║     ██║   ██║██╔══██╗██╔══╝");
    LOG_INFO(logger, "                                ╚██████╗╚██████╔╝██║  ██║███████╗");
    LOG_INFO(logger, "                                 ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝");
    LOG_INFO(logger, "");
    LOG_INFO(logger, "  	  AzerothCore 3.3.5a  -  www.azerothcore.org");
    LOG_INFO(logger, "");
    LOG_INFO(logger, "> Using configuration file:       %s", configName.c_str());
    LOG_INFO(logger, "> Using SSL version:              %s (library: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION));
    LOG_INFO(logger, "> Using ACE version:              %s", ACE_VERSION);
    LOG_INFO(logger, "> Using Boost version:            %i.%i.%i", BOOST_VERSION / 100000, BOOST_VERSION / 100 % 1000, BOOST_VERSION % 100);
    LOG_INFO(logger, "");
}
