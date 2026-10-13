#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083c388_Construct, 0x004D64B0, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_0083c478_InitCategoryObject(C_0083c478* self, void* /*edx*/, int a2)
{
    C_0083c388_Construct(self, nullptr, a2);

    self->field_0044 = static_cast<uint32_t>(-1);
    self->field_0048 = static_cast<uint32_t>(-1);
    self->field_004c = 0;
    self->field_0056 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;

    self->vtbl = CSF_VTBL(0x0083C478);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083C458));

    self->field_0050 = 2;
    self->field_0054 = 1;
    self->field_0055 = 1;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004D67C0, C_0083c478_InitCategoryObject, "auto");
