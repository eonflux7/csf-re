#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, S_006fdfe0_FindWorldFromClump, 0x0058DF40, (int a1));
CSF_FUNC(int, __cdecl, RpWorldRemoveClump, 0x0058E310, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static char __fastcall S_006fdfe0_FinalizeState(S_006fdfe0* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_0004);
    self->field_0000 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x00862FEC));

    if (result)
    {
        const std::uint32_t clump = self->field_0008;
        result = static_cast<char>(clump);
        self->field_0004 = 0;
        if (clump)
        {
            const int world = S_006fdfe0_FindWorldFromClump(static_cast<int>(clump));
            result = static_cast<char>(world);
            if (world)
                result = static_cast<char>(
                    RpWorldRemoveClump(world, static_cast<int>(self->field_0008)));
        }
    }

    if (self->field_00bc)
        result = static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(self->field_00b8)));

    if (self->field_00b0 >= 0x10)
    {
        const std::uint32_t buffer = self->field_009c;
        result = static_cast<char>(buffer);
        if (buffer)
            result = static_cast<char>(Mem_FreeDebug(reinterpret_cast<void*>(buffer)));
    }

    self->field_00b0 = 15;
    self->field_00ac = 0;
    *reinterpret_cast<std::uint8_t*>(&self->field_009c) = 0;

    if (self->field_0054 >= 0x10)
    {
        const std::uint32_t buffer = self->field_0040;
        result = static_cast<char>(buffer);
        if (buffer)
            result = static_cast<char>(Mem_FreeDebug(reinterpret_cast<void*>(buffer)));
    }

    self->field_0050 = 0;
    self->field_0054 = 15;
    *reinterpret_cast<std::uint8_t*>(&self->field_0040) = 0;

    return result;
}

HOOK(0x006FE010, S_006fdfe0_FinalizeState, "auto");
