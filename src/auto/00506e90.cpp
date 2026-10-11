#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(char, __fastcall, CNetProfile_NeedsCleanup, 0x00506E20,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuAccountEdit_ResetInnerObject, 0x00518890,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall CNetProfile_Destroy(CNetProfile* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083F6C0);

    if (reinterpret_cast<char (__fastcall*)(void*, void*)>(
            CSF_VTBL(0x0083F6CC)[0])(self, nullptr))
    {
        SceneLoader_NoOp();
        self->field_0008 = 0;
    }

    if (self->field_175c >= 8 && self->field_1748)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_1748));

    self->field_175c = 7;
    self->field_1758 = 0;
    *reinterpret_cast<uint16_t*>(&self->field_1748) = 0;

    if (self->field_1740 >= 8 && self->field_172c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_172c));

    self->field_1740 = 7;
    self->field_173c = 0;
    *reinterpret_cast<uint16_t*>(&self->field_172c) = 0;

    if (self->field_1724 >= 0x10 && self->field_1710)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_1710));

    self->field_1720 = 0;
    self->field_1724 = 15;
    *reinterpret_cast<uint8_t*>(&self->field_1710) = 0;

    auto* accountEnd = reinterpret_cast<uint8_t*>(&self->field_1704);
    int remaining = 10;
    int result;
    do
    {
        accountEnd -= 0x24C;
        result = CMenuAccountEdit_ResetInnerObject(accountEnd, nullptr);
    }
    while (--remaining);

    self->vtbl = CSF_VTBL(0x0083F630);
    return result;
}

HOOK(0x00506E90, CNetProfile_Destroy, "auto");
