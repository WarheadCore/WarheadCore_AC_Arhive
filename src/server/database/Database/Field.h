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

#ifndef WH_FIELD_H
#define WH_FIELD_H

#include "Common.h"
#include "Log.h"
#include <string_view>
#include <mysql.h>

class WH_DATABASE_API Field
{
    friend class ResultSet;
    friend class PreparedResultSet;

public:

    bool GetBool() const;
    uint8 GetUInt8() const;
    int8 GetInt8() const;
    uint16 GetUInt16() const;
    int16 GetInt16() const;
    uint32 GetUInt32() const;
    int32 GetInt32() const;
    uint64 GetUInt64() const;
    int64 GetInt64() const;
    float GetFloat() const;
    double GetDouble() const;
    char const* GetCString() const;
    std::string GetString() const;
    std::string_view GetStringView() const;
    bool IsNull() const;

protected:
    Field();
    ~Field();

#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif
    struct
    {
        uint32 length;          // Length (prepared strings only)
        void* value;            // Actual data in memory
        enum_field_types type;  // Field type
        bool raw;               // Raw bytes? (Prepared statement or ad hoc)
    } data;
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

    void SetByteValue(void const* newValue, size_t const newSize, enum_field_types newType, uint32 length);
    void SetStructuredValue(char* newValue, enum_field_types newType);

    void CleanUp();

    static size_t SizeForType(MYSQL_FIELD* field);
    bool IsType(enum_field_types type) const;
    bool IsNumeric() const;

private:

#ifdef ACORE_DEBUG
    static char const* FieldTypeToString(enum_field_types type);
#endif
};

#endif

