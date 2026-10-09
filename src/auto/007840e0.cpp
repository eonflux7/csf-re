#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, WeaponDefs_CopyEmbeddedBlock, 0x007837B0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, WeaponDefs_CopyTrailingBlock, 0x007835C0, (void* self, void* edx, int a2));

// Copies one weapon definition record (src) into self: first dword, an embedded
// block at +4, six dwords at +0x1D8..+0x1EC, and a trailing block at +0x1F0.
static char* __fastcall WeaponDefs_CopyEntry(char* self, void* /*edx*/, uint32_t* src)
{
    CSF_AT(uint32_t, self, 0) = src[0];

    WeaponDefs_CopyEmbeddedBlock(self + 4, nullptr, reinterpret_cast<int>(src + 1));

    CSF_AT(uint32_t, self, 0x1D8) = CSF_AT(uint32_t, src, 0x1D8);
    CSF_AT(uint32_t, self, 0x1DC) = CSF_AT(uint32_t, src, 0x1DC);
    CSF_AT(uint32_t, self, 0x1E0) = CSF_AT(uint32_t, src, 0x1E0);
    CSF_AT(uint32_t, self, 0x1E4) = CSF_AT(uint32_t, src, 0x1E4);
    CSF_AT(uint32_t, self, 0x1E8) = CSF_AT(uint32_t, src, 0x1E8);
    CSF_AT(uint32_t, self, 0x1EC) = CSF_AT(uint32_t, src, 0x1EC);

    WeaponDefs_CopyTrailingBlock(self + 0x1F0, nullptr,
                                 reinterpret_cast<int>(reinterpret_cast<uint8_t*>(src) + 0x1F0));
    return self;
}

HOOK(0x7840E0, WeaponDefs_CopyEntry, "auto");
