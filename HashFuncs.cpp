#include <assert.h>

#include "HashFuncs.h"

unsigned int MurmurHash(const void* hashingArr, const size_t length, const unsigned int seed)
{
    assert(hashingArr);
    assert(length > 0);

    const unsigned int c1 = 0x5bd1e995; 
    const unsigned int c2 = 24;

    unsigned int hash = seed ^ (unsigned int)length;

    const unsigned char* data = (const unsigned char*)hashingArr;
    unsigned int word = 0;

    size_t len = length;
    while (len >= 4)
    {
        word  = data[0];
        word |= data[1] << 8;
        word |= data[2] << 16;
        word |= data[3] << 24;

        word *= c1;
        word ^= word >> c2;
        word *= c1;

        hash *= c1;
        hash ^= word;

        data += 4;
        len -= 4;
    }

    assert(len < 4);

    switch (len)
    { 
        case 3:
            hash ^= data[2] << 16;
            // fall through
        case 2:
            hash ^= data[1] << 8;
            // fall through
        case 1:
            hash ^= data[0];
            hash *= c1;
            break;
        default:
            break;
    };

    hash ^= hash >> 13;
    hash *= c1;
    hash ^= hash >> 15;

    return hash;
}
