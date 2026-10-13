#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __fastcall, NetSpawnMessages_DestroyBase, 0x0064EBC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_00867fc0_Destroy(C_00867fc0* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00867FC0);

    // These no-argument calls ignore the original ECX subobject addresses.
    SceneLoader_NoOp(); // +0x300
    SceneLoader_NoOp(); // +0x2E8
    SceneLoader_NoOp(); // +0x2DC
    SceneLoader_NoOp(); // +0x2D6
    SceneLoader_NoOp(); // +0x2D0
    SceneLoader_NoOp(); // +0x2CA
    SceneLoader_NoOp(); // +0x2C4
    SceneLoader_NoOp(); // +0x2B8

    if (self->field_021c >= 0x10) {
        void* buffer = CSF_AT(void*, self, 0x208);
        if (buffer)
            Mem_FreeDebug(buffer);
    }

    self->field_021c = 15;
    CSF_AT(uint32_t, self, 0x218) = 0;
    CSF_AT(uint8_t, self, 0x208) = 0;

    SceneLoader_NoOp(); // +0x64
    SceneLoader_NoOp(); // +0x5F
    SceneLoader_NoOp(); // +0x5C
    SceneLoader_NoOp(); // +0x50
    SceneLoader_NoOp(); // +0x44

    return NetSpawnMessages_DestroyBase(self, nullptr);
}

HOOK(0x00748780, C_00867fc0_Destroy, "auto");
