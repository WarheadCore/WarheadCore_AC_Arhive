/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

#ifndef __ADDONHANDLER_H
#define __ADDONHANDLER_H

#include "Common.h"
#include "Config.h"
#include "WorldPacket.h"

class AddonHandler
{
public:
    AddonHandler();
    ~AddonHandler();

    static AddonHandler* instance();
    
    //build addon packet
    bool BuildAddonPacket(WorldPacket* Source, WorldPacket* Target);
};

#define sAddOnHandler AddonHandler::instance()

#endif
