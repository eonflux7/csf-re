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

static C_00831414* __fastcall C_00831414_Destroy(C_00831414* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x00831414);

    void* child = reinterpret_cast<void*>(self->field_0020);
    if (child != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0])(child, nullptr, 1);
        self->field_0020 = 0;
    }

    const uint32_t emptyVtblAddr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
    self->field_005c = emptyVtblAddr;
    self->field_0038 = emptyVtblAddr;
    self->field_002c = emptyVtblAddr;
    self->field_0024 = emptyVtblAddr;
    self->field_0004 = emptyVtblAddr;
    self->vtbl = CSF_VTBL(0x0082D410);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x461970, C_00831414_Destroy, "auto");
