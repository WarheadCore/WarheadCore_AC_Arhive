/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

#ifndef OPENSSL_CRYPTO_H
#define OPENSSL_CRYPTO_H

#include "Define.h"

/**
* A group of functions which setup openssl crypto module to work properly in multithreaded enviroment
* If not setup properly - it will crash
*/
namespace OpenSSLCrypto
{
    /// Needs to be called before threads using openssl are spawned
    AC_COMMON_API void threadsSetup();
    /// Needs to be called after threads using openssl are despawned
    AC_COMMON_API void threadsCleanup();
}

#endif
