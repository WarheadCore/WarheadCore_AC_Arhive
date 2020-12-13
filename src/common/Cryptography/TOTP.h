#ifndef _TOTP_H
#define _TOTP_H

#include "Define.h"
#include <ctime>
#include <vector>

namespace Crypto
{
    struct TC_COMMON_API TOTP
    {
        static constexpr size_t RECOMMENDED_SECRET_LENGTH = 20;
        using Secret = std::vector<uint8>;

        static uint32 GenerateToken(Secret const& key, time_t timestamp);
        static bool ValidateToken(Secret const& key, uint32 token);
    };
}

#endif
