#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, Scene_ResetSubsystem184, 0x00475530, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __fastcall C_00831698_ReleaseOwnedBlocks(C_00831698* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00831698);
    Scene_ResetSubsystem184(self, nullptr);

    uint32_t firstBlock = self->field_0028;
    self->field_002c = 0;
    self->field_0030 = 0;
    if (firstBlock) {
        CoreMemory_Free(reinterpret_cast<void*>(firstBlock));
        self->field_0028 = 0;
    }
    if (self->field_0028) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0028));
        self->field_0028 = 0;
    }
    if (self->field_0014) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }

    int result = static_cast<int>(self->field_0004);
    if (result) {
        result = CoreMemory_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x475790, C_00831698_ReleaseOwnedBlocks, "auto");
