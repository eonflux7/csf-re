#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Archive_SerializeString(int archiveAddress, S_00691860* string), __cdecl, ret 0.
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

// Stores a 32-bit value and two flag bytes into the string record, then serializes it
// unless the record's first byte is set. Returns the record pointer when skipped, or
// the serializer's result when it runs.
static S_00691860* __cdecl SerializeStringWithFlags(S_00691860* a2, int a1, uint32_t a3, char a4, char a5)
{
    S_00691860* result = a2;

    CSF_AT(uint32_t, a2, 0x1C) = a3;

    if (CSF_AT(uint8_t, a2, 0x00) == 0)
    {
        CSF_AT(char, a2, 0x15) = a4;
        CSF_AT(char, a2, 0x16) = a5;
        return reinterpret_cast<S_00691860*>(Archive_SerializeString(a1, a2));
    }

    return result;
}

HOOK(0x413540, SerializeStringWithFlags, "auto");
