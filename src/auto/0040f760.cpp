#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_0082de40_ReleaseAndFreeBlock(C_0082de40* self, void* /*edx*/) {
    int result;

    self->vtbl = CSF_VTBL(0x0082DE40);
    C_0082deb8_ReleaseChildBase_41E110(self, nullptr);
    result = self->field_0010;
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0010));
        self->field_0010 = 0;
    }
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0040F760, C_0082de40_ReleaseAndFreeBlock, "auto");
