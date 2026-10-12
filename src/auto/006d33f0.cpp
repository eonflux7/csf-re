#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));
CSF_FUNC(int, __fastcall, sub_6D2BF0, 0x006D2BF0, (void* self, void* edx));

static int* __fastcall CCamera_DestructAndMaybeFree(CCamera* self, void* /*edx*/, char freeFlag)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860FB8);
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_DATA(0x00860FB0));
    self->field_0008 = static_cast<uint32_t>(CSF_ADDR_DATA(0x00860F90));

    sub_6D2BF0(self, nullptr);

    GfxObject_DestroyEmbeddedState(
        reinterpret_cast<C_00860630*>(reinterpret_cast<uint8_t*>(self) + 0x10), nullptr);

    self->field_0008 = static_cast<uint32_t>(CSF_ADDR_DATA(0x0082D410));
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860F10);
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_DATA(0x00860F04));
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_DATA(0x0082D410));

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((freeFlag & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<int*>(self);
}

HOOK(0x006D33F0, CCamera_DestructAndMaybeFree, "auto");
