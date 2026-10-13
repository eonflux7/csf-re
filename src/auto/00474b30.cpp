#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

static int __fastcall C_0082d498_Clone(void* self, void* /*edx*/)
{
    const int result = CoreMemory_AllocUntagged(56);
    void* clone = reinterpret_cast<void*>(result);

    if (clone)
    {
        CSF_AT(void*, clone, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(int, clone, 0x0C) = -1;
        CSF_AT(int, clone, 0x10) = -1;
        CSF_AT(void*, clone, 0x14) = CSF_VTBL(0x0082D448);
        CSF_AT(int, clone, 0x18) = -1;
        CSF_AT(int, clone, 0x1C) = -1;
        CSF_AT(int, clone, 0x04) = 40;
        CSF_AT(int, clone, 0x20) = 0;
        CSF_AT(int, clone, 0x24) = 0;
        CSF_AT(int, clone, 0x2C) = 0;
        CSF_AT(int, clone, 0x28) = 0;
        CSF_AT(std::uint8_t, clone, 0x30) = 0;
        CSF_AT(void*, clone, 0x00) = CSF_VTBL(0x0082D498);
        CSF_AT(int, clone, 0x34) = 0;
    }

    CSF_AT(int, clone, 0x04) = CSF_AT(int, self, 0x04);
    CSF_AT(int, clone, 0x0C) = CSF_AT(int, self, 0x0C);
    CSF_AT(int, clone, 0x10) = CSF_AT(int, self, 0x10);
    CSF_AT(int, clone, 0x18) = CSF_AT(int, self, 0x18);
    CSF_AT(int, clone, 0x1C) = CSF_AT(int, self, 0x1C);
    CSF_AT(int, clone, 0x24) = CSF_AT(int, self, 0x24);
    CSF_AT(int, clone, 0x2C) = CSF_AT(int, self, 0x2C);
    CSF_AT(int, clone, 0x20) = CSF_AT(int, self, 0x20);
    CSF_AT(std::uint8_t, clone, 0x30) = CSF_AT(std::uint8_t, self, 0x30);
    CSF_AT(int, clone, 0x28) = CSF_AT(int, self, 0x28);
    CSF_AT(int, clone, 0x34) = CSF_AT(int, self, 0x34);
    return result;
}

HOOK(0x00474B30, C_0082d498_Clone, "auto");
