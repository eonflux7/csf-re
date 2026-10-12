#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, sub_43FA70, 0x0043FA70, (int a1, int a2));

// Resets the four 15-slot record blocks (value dword, count dword, flag byte)
// at this+0x18/0x34/0x54/0x74/0x94/0xB0 and the 0x1C/0x38/0x58/0x78/0x98/0xB4
// capacity dwords, then calls sub_43FA70 with this in ecx.
static int __fastcall Campaign_ResetSlotFields(void* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 15;
    CSF_AT(uint8_t, self, 0x08) = 0;
    CSF_AT(int, self, 0x38) = 15;
    CSF_AT(int, self, 0x34) = 0;
    CSF_AT(uint8_t, self, 0x24) = 0;
    CSF_AT(int, self, 0x58) = 15;
    CSF_AT(int, self, 0x54) = 0;
    CSF_AT(uint8_t, self, 0x44) = 0;
    CSF_AT(int, self, 0x78) = 15;
    CSF_AT(int, self, 0x74) = 0;
    CSF_AT(uint8_t, self, 0x64) = 0;
    CSF_AT(int, self, 0x98) = 15;
    CSF_AT(int, self, 0x94) = 0;
    CSF_AT(uint8_t, self, 0x84) = 0;
    CSF_AT(int, self, 0xB4) = 15;
    CSF_AT(int, self, 0xB0) = 0;
    CSF_AT(uint8_t, self, 0xA0) = 0;

    // Original leaves edx unset here; pass 0 for the second register argument.
    sub_43FA70(static_cast<int>(reinterpret_cast<uintptr_t>(self)), 0);

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x00440C20, Campaign_ResetSlotFields, "auto");
