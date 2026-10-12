#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Binds to the original helpers by address.
CSF_FUNC(char, __fastcall, ConvertNarrowSourceToTemp, 0x00697070, (void* self, void* edx, uint8_t* a2));
CSF_FUNC(void*, __fastcall, AppendTempToString, 0x00696BD0, (void* self, void* edx, uint32_t* a2, unsigned int a3, unsigned int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Reimplements 0x697150: converts a narrow source string into a temporary
// variant value, appends it to *self, then releases the temporary's buffer.
// `self` is untyped because S_004f21d0 has no declared struct in auto_classes.h.
static void* __fastcall AppendNarrowToWideString(void* self, void* /*edx*/, uint8_t* a2)
{
    // 0x1C-byte stack temporary (same frame layout as the original):
    //   +0x00 : object passed as `this` to the converter / append helper
    //   +0x04 : Block pointer (only the low word is initialized)
    //   +0x14 : v6 (initialized to 0)
    //   +0x18 : v7 (ownership/state marker, initialized to 7)
    alignas(8) uint8_t tempBytes[0x1C];
    uint8_t* temp = tempBytes;

    CSF_AT(uint32_t, temp, 0x18) = 7;
    CSF_AT(uint32_t, temp, 0x14) = 0;
    CSF_AT(uint16_t, temp, 0x04) = 0;

    ConvertNarrowSourceToTemp(temp, nullptr, a2);
    AppendTempToString(self, nullptr, reinterpret_cast<uint32_t*>(temp), 0, 0xFFFFFFFFu);

    if (CSF_AT(uint32_t, temp, 0x18) >= 8)
    {
        uint32_t block = CSF_AT(uint32_t, temp, 0x04);
        if (block != 0)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(block));
        }
    }

    return self;
}

HOOK(0x00697150, AppendNarrowToWideString, "auto");
