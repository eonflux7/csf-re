#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, MpRules_DestroyEntries, 0x00559910, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall MpRules_InitializeState(void* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x04) = 0;
    CSF_AT(std::uint32_t, self, 0x08) = 0;
    CSF_AT(std::uint32_t, self, 0x0C) = 0;
    CSF_AT(std::uint32_t, self, 0x1D4) = 0;
    CSF_AT(std::uint32_t, self, 0x1D8) = 0;
    CSF_AT(std::uint32_t, self, 0x1DC) = 0;

    CSF_AT(std::uint32_t, self, 0x10) = 0;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint32_t, self, 0x24) = 0;

    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    CSF_AT(std::uint32_t, self, 0x30) = 0;
    CSF_AT(std::uint32_t, self, 0x34) = 0;
    CSF_AT(std::uint32_t, self, 0x38) = 0;
    CSF_AT(std::uint32_t, self, 0x3C) = 0;

    std::memset(static_cast<std::uint8_t*>(self) + 0x40, 0, 0xF0u);
    std::memset(static_cast<std::uint8_t*>(self) + 0x130, 0, 0x48u);

    CSF_AT(std::uint32_t, self, 0x178) = 0;
    CSF_AT(std::uint32_t, self, 0x17C) = 0;
    CSF_AT(std::uint32_t, self, 0x180) = 0;
    CSF_AT(std::uint32_t, self, 0x184) = 0;
    CSF_AT(std::uint32_t, self, 0x188) = 0;
    CSF_AT(std::uint32_t, self, 0x18C) = 0;
    CSF_AT(std::uint32_t, self, 0x190) = 0;
    CSF_AT(std::uint32_t, self, 0x194) = 0;
    CSF_AT(std::uint32_t, self, 0x198) = 0;
    CSF_AT(std::uint32_t, self, 0x19C) = 0;
    CSF_AT(std::uint32_t, self, 0x1A0) = 0;
    CSF_AT(std::uint32_t, self, 0x1A4) = 0;
    CSF_AT(std::uint32_t, self, 0x1A8) = 0;
    CSF_AT(std::uint32_t, self, 0x1AC) = 0;
    CSF_AT(std::uint32_t, self, 0x1B0) = 0;
    CSF_AT(std::uint32_t, self, 0x1B4) = 0;
    CSF_AT(std::uint32_t, self, 0x1B8) = 0;
    CSF_AT(std::uint32_t, self, 0x1BC) = 0;
    CSF_AT(std::uint32_t, self, 0x1C0) = 0;

    const int entries = CSF_AT(int, self, 0x1D4);
    if (entries)
    {
        MpRules_DestroyEntries(entries, CSF_AT(int, self, 0x1D8));
        if (CSF_AT(std::uint32_t, self, 0x1D4))
            Mem_FreeDebug(CSF_AT(void*, self, 0x1D4));
    }

    CSF_AT(std::uint32_t, self, 0x1D4) = 0;
    CSF_AT(std::uint32_t, self, 0x1D8) = 0;
    CSF_AT(std::uint32_t, self, 0x1DC) = 0;
    return static_cast<int>(reinterpret_cast<std::intptr_t>(self));
}

HOOK(0x0055A090, MpRules_InitializeState, "auto");
