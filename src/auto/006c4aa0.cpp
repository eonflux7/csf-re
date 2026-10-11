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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

static C_00860894* __fastcall C_00860894_Destroy(C_00860894* self, void* /*edx*/, char freeSelfFlag) {
    uint32_t* block = reinterpret_cast<uint32_t*>(self->field_0008);
    self->vtbl = CSF_VTBL(0x00860894);
    self->field_000c = 0;
    if (block) {
        if (block[0]) {
            Mem_Free(reinterpret_cast<void*>(block[0]));
            block[0] = 0;
        }
        Mem_Free(block);
        self->field_0008 = 0;
    }
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
    if (freeSelfFlag & 1) {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x006C4AA0, C_00860894_Destroy, "auto");
