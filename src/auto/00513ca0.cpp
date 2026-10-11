#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CSaveGameManager_ReleaseResources, 0x00511FF0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, ProfilesSavegames_DestroyManagerBase, 0x00513B10,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall CSaveGameManager_Destroy(
    CSaveGameManager* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00840920);
    CSaveGameManager_ReleaseResources(self, nullptr);

    if (self->field_00ac >= 0x10 && self->field_0098)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0098));

    self->field_00ac = 15;
    self->field_00a8 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0098) = 0;

    if (self->field_0090 >= 8 && self->field_007c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_007c));

    self->field_0090 = 7;
    self->field_008c = 0;
    *reinterpret_cast<uint16_t*>(&self->field_007c) = 0;

    if (self->field_0074 >= 8 && self->field_0060)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0060));

    self->field_0074 = 7;
    self->field_0070 = 0;
    *reinterpret_cast<uint16_t*>(&self->field_0060) = 0;

    if (self->field_0058 >= 8 && self->field_0044)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0044));

    self->field_0054 = 0;
    self->field_0058 = 7;
    *reinterpret_cast<uint16_t*>(&self->field_0044) = 0;

    return ProfilesSavegames_DestroyManagerBase(self, nullptr);
}

HOOK(0x00513CA0, CSaveGameManager_Destroy, "auto");
