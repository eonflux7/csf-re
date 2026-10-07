#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int** __fastcall FindUnsignedWordTreeEntry(
    int** self, void* /*edx*/, int** result, std::uint16_t* key)
{
    int* candidate = CSF_AT(int*, self, 0x04);
    int* node = CSF_AT(int*, candidate, 0x04);

    if (CSF_AT(std::uint8_t, node, 0x49) == 0)
    {
        const std::uint16_t searchKey = *key;
        do
        {
            if (CSF_AT(std::uint16_t, node, 0x0C) >= searchKey)
            {
                candidate = node;
                node = CSF_AT(int*, node, 0x00);
            }
            else
            {
                node = CSF_AT(int*, node, 0x08);
            }
        }
        while (CSF_AT(std::uint8_t, node, 0x49) == 0);
    }

    int* sentinel = CSF_AT(int*, self, 0x04);
    if (candidate == sentinel ||
        *key < CSF_AT(std::uint16_t, candidate, 0x0C))
    {
        *result = sentinel;
    }
    else
    {
        *result = candidate;
    }
    return result;
}

HOOK(0x00739FA0, FindUnsignedWordTreeEntry, "auto");
