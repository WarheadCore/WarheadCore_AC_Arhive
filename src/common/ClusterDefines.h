#ifndef _CLUSTERDEFINES_H
#define _CLUSTERDEFINES_H

enum NodeTypes
{
    // handles most of the stuff, used in main cities
    NODE_TYPE_MASTER = 1,                   // Master-Node (only this Node should be write data on our DB, if smt. is global or logon couldn't write it)
    NODE_TYPE_EASTERN_KINGDOMS = 2,         // Map node: Eastern Kingdoms
    NODE_TYPE_KALIMDOR = 3,                 // Map node: Kalimdor
    NODE_TYPE_OUTLAND = 4,                  // Map node: Outland
    NODE_TYPE_NORTHREND = 5,                // Map node: Northrend
    NODE_TYPE_PVE = 6,                      // Map node: Raids and Dungeons
    NODE_TYPE_PVP = 7,                      // Map node: Battlegrounds and Arenas
};

//Defines at which point our init fails
enum NodeAck
{
    NODE_INIT_ACK_FAIL              = 0,
    NODE_INIT_ACK_ITEM_GUID_FAIL    = 1,
    NODE_INIT_ACK_MAIL_GUID_FAIL    = 2,
    NODE_INIT_ACK_CORPSE_GUID_FAIL  = 3,
    NODE_INIT_ACK_OK                = 254
};

// send packets from cluster(proxy) to nodes
enum ProxyToNodes
{
    PTN_GUILD_INVITE = 0, 
    PTN_DEFAULT,
};

enum AchievementData
{
    CL_DEF_COMPLETED_ACHIEVEMENT = 0,   //uint32(ID)
    CL_DEF_CRITERIA_UPDATE,             //uint32 entry, changeValue, uint8 ptype;
    CL_DEF_RESET_ACHIEVEMENTS,
    CL_DEF_RESET_ACHIEVEMENT_CRITERIA,  //uint32 type,miscvalue1,miscvalue2,bool evenIfCriteriaComplete;
};

enum PlayerClusterData
{
    CL_PLR_SEND_HEALTH_AMOUNT = 1000,
    CL_PLR_SEND_HEALTH_MAX_AMOUNT,
    CL_PLR_SEND_POWER_AMOUNT,
    CL_PLR_SEND_MAX_POWER_AMOUNT,
    CL_PLR_SEND_POWER_TYPE,
    CL_PLR_SEND_FLAG_UPDATE,
    CL_PLR_SEND_SET_DEATHSTATE,
    CL_PLR_SEND_POSITION_UPDATE,
    CL_PLR_SEND_GUILD_ID,
    
    CL_PLR_SEND_NODE_CHANGED_MSG,
    
    // misc
    CL_PLR_GROUP_UPDATE_ID,
};

enum MiscData
{
    //Syncs L/N
    CL_DEF_GUID_SYNC,                   //Logon sends Highest GUID, Node sends if any GUID was taken
    CL_DEF_SHUTDOWN,                    //Sends shutdown in uint32 Seconds
    CL_DEF_GUILDBANK_ITEM_ADD,          //Add an item to GUILD-BANK
    CL_DEF_GUILDBANK_ITEM_REMOVE,       //remove an item from GUILD-BANK

    CL_DEF_TRANSFER_TO_NODE,           //uint32 (NODE)
    CL_DEF_TRANSFER_OK,
    CL_DEF_TRANSFER_ACC,

    CL_DEF_REP_SET_REPUTATION = 0,
    CL_DEF_REP,
    CL_DEF_TEST,
    CL_DEF_TELE,

};

enum EntityIDType
{
    ENTITYID_ITEM = 1,
    ENTITYID_MAIL,
    ENTITYID_CORPSE,
    ENTITYID_GROUP
};
#endif
