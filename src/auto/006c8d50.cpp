#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bound exactly as given in the packet (stdcall, 2 stack args; ret 8).
CSF_FUNC(void*, __stdcall, ComputeEntryNameHash, 0x006C8CC0, (uint8_t* a2, uint8_t* a3));

// Walks the 36-byte entry array of C_00860954 and, for every entry whose cached
// slot (+0x1C) is still zero and whose name length (+0x14) is non-zero, computes
// the cached value from the name text (inline buffer at +0x04 when capacity < 0x10,
// otherwise the heap pointer stored at +0x04) and the inline buffer at +0x20.
// Returns the number of entries computed by the last size check.
static unsigned int __fastcall C_00860954_CacheEntryNameHashes(C_00860954* self, void* /*edx*/)
{
    unsigned int entryIndex = 0;
    unsigned int result = 0;

    for (int offset = 0; ; offset += 36)
    {
        result = self->field_0008;
        if (result == 0)
            break;

        int32_t spanBytes = (int32_t)(self->field_000c - self->field_0008);
        result = (unsigned int)(spanBytes / 36);
        if (entryIndex >= result)
            break;

        uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_0008) + offset;

        int32_t cachedValue = CSF_AT(int32_t, entry, 0x1C);
        if (cachedValue == 0 && CSF_AT(uint32_t, entry, 0x14) != 0)
        {
            uint8_t* nameText;
            if (CSF_AT(uint32_t, entry, 0x18) < 0x10u)
                nameText = entry + 4;
            else
                nameText = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, entry, 0x04));

            CSF_AT(void*, entry, 0x1C) = ComputeEntryNameHash(nameText, entry + 0x20);
        }

        ++entryIndex;
    }
    return result;
}

HOOK(0x006C8D50, C_00860954_CacheEntryNameHashes, "auto");
