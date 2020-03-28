/*
 * Copyright (C) since 2020 Andrei Guluaev (Winfidonarleyan/Kargatum) https://github.com/Winfidonarleyan
 * Licence MIT https://opensource.org/MIT
 */

#ifndef _ANTICHEAT_H_
#define _ANTICHEAT_H_

class AnticheatData
{
public:
    void SetLastOpcode(uint32 opcode);
    uint32 GetLastOpcode() const;

    const MovementInfo& GetLastMovementInfo() const;
    void SetLastMovementInfo(MovementInfo& moveInfo);

    void SetPosition(float x, float y, float z, float o);

private:
    uint32 lastOpcode = 0;
    MovementInfo lastMovementInfo;
};

class Anticheat
{
    Anticheat();
    ~Anticheat();

public:
    static Anticheat* instance();

    void StartHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
    void HandlePlayerLogin(Player* player);
    void HandlePlayerLogout(Player* player);

private:
    void SpeedHackDetection(Player* player, MovementInfo movementInfo);
    void FlyHackDetection(Player* player, MovementInfo movementInfo);
    void WalkOnWaterHackDetection(Player* player, MovementInfo movementInfo);
    void JumpHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
    void TeleportPlaneHackDetection(Player* player, MovementInfo);
    void ClimbHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);

    void BuildReport(Player* player);

    // GUIDLow is the key.
    typedef std::map<uint32, AnticheatData> AnticheatPlayersDataMap;

    AnticheatPlayersDataMap m_Players;                        ///< Player data
};

#define sAnticheat Anticheat::instance()

#endif /* _ANTICHEAT_H_ */
