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

CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

static void __fastcall GfxObject_DestroyEmbeddedState(C_00860630* self, void* /*edx*/)
{
    uint32_t frameHandle = self->field_000c;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860700);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x008606F8);
    self->field_0008 = (uint32_t)CSF_ADDR_DATA(0x008606DC);

    if (frameHandle && self->field_0010)
        RwFrameDestroy((int)frameHandle);

    self->field_000c = 0;
    self->field_0008 = (uint32_t)CSF_ADDR_DATA(0x0082D410);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860630);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x00860624);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0082D410);

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006BF180, GfxObject_DestroyEmbeddedState, "auto");
