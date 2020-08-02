/*
* This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _EXTERNALMAIL_H_
#define _EXTERNALMAIL_H_

#include "Common.h"
#include <unordered_map>

struct ExMail
{
    uint32 ID;
    uint64 PlayerGuid;
    std::string Subject;
    std::string Body;
    uint32 Money;
    uint32 CreatureEntry;
    std::list<std::pair<uint32, uint32>> Items;
    std::list<std::list<std::pair<uint32, uint32>>> _overCountItems;

    bool AddItems(uint32 itemID, uint32 itemCount);
};

class ExternalMail
{
public:
    static ExternalMail* instance();

    void SendMails();
    void GetMailsFromDB();
    void AddMail(std::string charName, std::string const thanksSubject, std::string const thanksText, uint32 itemID, uint32 itemCount, uint32 creatureEntry);
private:
    std::unordered_map<uint32, ExMail> _store;
};

#define sEM ExternalMail::instance()

#endif /* _EXTERNALMAIL_H_ */
