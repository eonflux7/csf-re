#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int** __fastcall FindTreeNodeBySignedKey(
    int** self, void* /*edx*/, int** result, const int32_t* key)
{
    int* candidate = CSF_AT(int*, self, 0x4);
    int* node = CSF_AT(int*, candidate, 0x4);

    if (!CSF_AT(uint8_t, node, 0x11))
    {
        const int32_t searchKey = *key;
        do
        {
            if (CSF_AT(int32_t, node, 0xC) >= searchKey)
            {
                candidate = node;
                node = CSF_AT(int*, node, 0x0);
            }
            else
            {
                node = CSF_AT(int*, node, 0x8);
            }
        } while (!CSF_AT(uint8_t, node, 0x11));
    }

    int* sentinel = CSF_AT(int*, self, 0x4);
    if (candidate == sentinel || *key < CSF_AT(int32_t, candidate, 0xC))
        *result = sentinel;
    else
        *result = candidate;

    return result;
}

HOOK(0x00734880, FindTreeNodeBySignedKey, "auto");
