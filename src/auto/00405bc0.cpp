#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Archive_SerializeObjectRef(archive, objectRecord), __cdecl, ret 0.
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

// Stores a 32-bit value and two flag bytes into the object-reference record, then serializes
// it unless the record's first byte is set. Returns the serializer's result when it runs,
// or the record pointer when skipped.
static int __cdecl SerializeObjectRefWithFlags(int a1, S_00691860* a2, int a3, char a4, char a5)
{
    int result = a1;

    CSF_AT(uint32_t, a1, 0x1C) = static_cast<uint32_t>(a3);

    if (CSF_AT(uint8_t, a1, 0x00) == 0)
    {
        CSF_AT(char, a1, 0x15) = a4;
        CSF_AT(char, a1, 0x16) = a5;
        return Archive_SerializeObjectRef(a2, reinterpret_cast<bool*>(a1));
    }

    return result;
}

HOOK(0x405BC0, SerializeObjectRefWithFlags, "auto");
