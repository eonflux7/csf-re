#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, CProfileManager_DestroyBase, 0x00516A10, (void* self, void* edx));

static int __fastcall CProfileManager_Destroy(CProfileManager* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008409F0);

    if (self->field_009c >= 8 && self->field_0088)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0088));
    self->field_009c = 7;
    self->field_0098 = 0;
    *reinterpret_cast<std::uint16_t*>(&self->field_0088) = 0;

    if (self->field_0080 >= 8 && self->field_006c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_006c));
    self->field_0080 = 7;
    self->field_007c = 0;
    *reinterpret_cast<std::uint16_t*>(&self->field_006c) = 0;

    if (self->field_0064 >= 8 && self->field_0050)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0050));
    self->field_0064 = 7;
    self->field_0060 = 0;
    *reinterpret_cast<std::uint16_t*>(&self->field_0050) = 0;

    if (self->field_0048 >= 8 && self->field_0034)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0034));
    self->field_0048 = 7;
    self->field_0044 = 0;
    *reinterpret_cast<std::uint16_t*>(&self->field_0034) = 0;

    if (self->field_002c >= 0x10 && self->field_0018)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0018));
    self->field_0028 = 0;
    self->field_002c = 15;
    *reinterpret_cast<unsigned char*>(&self->field_0018) = 0;

    return CProfileManager_DestroyBase(self, nullptr);
}

HOOK(0x00515710, CProfileManager_Destroy, "auto");
