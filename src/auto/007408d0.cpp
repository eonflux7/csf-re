#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, ClearDwordField4, 0x0073F760, (void* self, void*));

static C_00867890* __fastcall C_00867890_DestroyAndMaybeFree(C_00867890* self, void* /*edx*/, char freeFlags) {
    uint32_t cachedField = CSF_AT(uint32_t, self, 0x08);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00867890);
    if (cachedField) {
        ClearDwordField4(reinterpret_cast<void*>(cachedField), nullptr);
    }
    if ((freeFlags & 1) != 0) {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x7408D0, C_00867890_DestroyAndMaybeFree, "auto");
