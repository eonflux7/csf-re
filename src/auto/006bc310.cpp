#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, RpWorldRender, 0x0058CBB0, (int a1));
CSF_FUNC(int, __cdecl, RpWorldForAllWorldSectors, 0x0058CFF0, (int a1, int (__cdecl* a2)(int*, int), int a3));
CSF_FUNC(int, __cdecl, RpWorldForAllClumps, 0x0058CEF0, (int a1, int (__cdecl* a2)(uint32_t*, int), int a3));

// Once the stage passes 5 the world is rendered directly. Before that, the
// world sectors and clumps are visited with their callbacks, and the stage
// counter advances by one.
static int __fastcall Environment_RenderWorldStage(S_006bc310* self, void* /*edx*/)
{
    if (static_cast<int>(self->field_0190) > 5)
        return RpWorldRender(static_cast<int>(self->field_0004));

    RpWorldForAllWorldSectors(
        static_cast<int>(self->field_0004),
        reinterpret_cast<int (__cdecl*)(int*, int)>(CSF_ADDR_FN(0x006BC2B0)),
        0);
    RpWorldForAllClumps(
        static_cast<int>(self->field_0004),
        reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x006BC2F0)),
        0);

    int result = static_cast<int>(self->field_0190) + 1;
    self->field_0190 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x006BC310, Environment_RenderWorldStage, "auto");
