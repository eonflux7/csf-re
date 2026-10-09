#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));
CSF_FUNC(char, __cdecl, sub_4FDDA0, 0x004FDDA0, (int a1, int a2));

// Compares String1 (case-insensitive) against the name stored in this slot.
// On mismatch, resets the 0xF0 record, asks sub_4FDDA0 to resolve String1 into it,
// and if that fails marks the record with 1s and reports success.
static char __fastcall S_004ff0f0_CheckNameAndLoad(S_004ff0f0* self, void* /*edx*/, char* String1)
{
    const char* storedName;
    if (self->field_0078 < 0x10)
        storedName = reinterpret_cast<const char*>(&self->field_0064);
    else
        storedName = reinterpret_cast<const char*>(static_cast<uintptr_t>(self->field_0064));

    char result = 0;
    int comparison = __stricmp(String1, storedName);
    if (comparison != 0)
    {
        CSF_AT(uint32_t, self, 0xF0) = 0;
        CSF_AT(uint8_t, self, 0xF4) = 0;
        CSF_AT(uint8_t, self, 0xF5) = 0;
        CSF_AT(uint8_t, self, 0xF6) = 0;
        CSF_AT(uint8_t, self, 0xF7) = 0;
        CSF_AT(uint8_t, self, 0xF8) = 0;

        result = sub_4FDDA0(static_cast<int>(reinterpret_cast<uintptr_t>(String1)),
                            static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0xF0)));
        if (!result)
        {
            CSF_AT(uint32_t, self, 0xF0) = 1;
            CSF_AT(uint8_t, self, 0xF4) = 1;
            CSF_AT(uint8_t, self, 0xF5) = 1;
            CSF_AT(uint8_t, self, 0xF6) = 1;
            CSF_AT(uint8_t, self, 0xF7) = 1;
            CSF_AT(uint8_t, self, 0xF8) = 1;
            result = 1;
        }
    }
    return result;
}

HOOK(0x004FF0F0, S_004ff0f0_CheckNameAndLoad, "auto");
