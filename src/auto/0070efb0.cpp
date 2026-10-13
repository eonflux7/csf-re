#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_00863800_BaseCleanup, 0x0070F2D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00863ba8_TrySetEffectMode, 0x00710850, (C_00863ba8* self, void*, int a2));
CSF_FUNC(int, __cdecl, RwResourcesEmptyArena, 0x007DB8B0, ());

static C_00863ac0* __fastcall C_00863ac0_DestructAndMaybeFree(C_00863ac0* self, void* /*edx*/, char a2)
{
    uint32_t savedEffectMode = self->field_001c;
    *(void***)self = CSF_VTBL(0x00863AC0);
    C_00863ba8_TrySetEffectMode(reinterpret_cast<C_00863ba8*>(self), nullptr, (int)savedEffectMode);
    RwResourcesEmptyArena();
    C_00863800_BaseCleanup(self, nullptr);
    if ((a2 & 1) != 0)
    {
        CoreMemory_Free(self);
    }
    return self;
}

HOOK(0x0070EFB0, C_00863ac0_DestructAndMaybeFree, "auto");
