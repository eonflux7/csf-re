#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, WeaponDefs_CopyBaseEntry, 0x00783B50, (void* self, void* edx, int a2));

// Copy-assigns a weapon definition record from another one.
static void* __fastcall WeaponDefs_CopyFrom(void* self, void* /*edx*/, void* source)
{
    uint8_t* dst = static_cast<uint8_t*>(self);
    uint8_t* src = static_cast<uint8_t*>(source);

    CSF_AT(int, dst, 0x0) = CSF_AT(int, src, 0x0);
    WeaponDefs_CopyBaseEntry(dst + 4, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(src + 4)));

    CSF_AT(int, dst, 0x1D8) = CSF_AT(int, src, 0x1D8);
    CSF_AT(int, dst, 0x1DC) = CSF_AT(int, src, 0x1DC);
    CSF_AT(int, dst, 0x1E0) = CSF_AT(int, src, 0x1E0);
    CSF_AT(int, dst, 0x1E4) = CSF_AT(int, src, 0x1E4);
    CSF_AT(int, dst, 0x1E8) = CSF_AT(int, src, 0x1E8);
    CSF_AT(int, dst, 0x1EC) = CSF_AT(int, src, 0x1EC);

    CSF_AT(int, dst, 0x1F0) = CSF_AT(int, src, 0x1F0);
    AssignStringSubstring(dst + 0x1F4, nullptr, src + 0x1F4, 0, static_cast<std::uint32_t>(-1));

    CSF_AT(int, dst, 0x210) = CSF_AT(int, src, 0x210);
    CSF_AT(int, dst, 0x214) = CSF_AT(int, src, 0x214);
    CSF_AT(int, dst, 0x218) = CSF_AT(int, src, 0x218);
    CSF_AT(int, dst, 0x21C) = CSF_AT(int, src, 0x21C);
    CSF_AT(int, dst, 0x220) = CSF_AT(int, src, 0x220);
    CSF_AT(int, dst, 0x224) = CSF_AT(int, src, 0x224);

    return self;
}

HOOK(0x784150, WeaponDefs_CopyFrom, "auto");
