#include "CryptoRandom.h"
#include "Errors.h"
#include <openssl/rand.h>

void Crypto::GetRandomBytes(uint8* buf, size_t len)
{
    int result = RAND_bytes(buf, len);
    ASSERT(result == 1, "Not enough randomness in OpenSSL's entropy pool. What in the world are you running on?");
}
