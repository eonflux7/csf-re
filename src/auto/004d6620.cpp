#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, S_00486c30_NotifyAndRecompute, 0x004882C0, (S_00486c30* self, void* , float a2, int a3));
CSF_FUNC(int, __fastcall, S_00486c30_NotifyThenRecompute, 0x004882F0, (S_00486c30* self, void* , float a2, int a3));
CSF_FUNC(int, __cdecl, GameConfig_IsSceneObjectRemovable, 0x007558C0, (int objectType));

static char __fastcall C_0083c388_NotifySceneAndRemoveObject(
    C_0083c388* self, void* /*edx*/, float amount, char removeObject)
{
    auto* context = reinterpret_cast<uint8_t*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    auto* object = reinterpret_cast<C_00833908*>(self->field_0014);
    const uint32_t objectType = CSF_AT(uint32_t, object, 0x50);
    auto* notifier = CSF_AT(S_00486c30*, context, 0x44);

    if (objectType == 22)
    {
        S_00486c30_NotifyAndRecompute(
            notifier, nullptr, amount, reinterpret_cast<int>(object));
    }
    else if (static_cast<uint8_t>(
                 GameConfig_IsSceneObjectRemovable(static_cast<int>(objectType))))
    {
        S_00486c30_NotifyThenRecompute(notifier, nullptr, amount, 1);
    }
    else
    {
        S_00486c30_NotifyAndRecompute(notifier, nullptr, amount, 0);
    }

    alignas(8) uint8_t recordBytes[0x34];
    CSF_AT(int, recordBytes, 0x0C) = -1;
    CSF_AT(int, recordBytes, 0x10) = -1;
    CSF_AT(int, recordBytes, 0x18) = -1;
    CSF_AT(int, recordBytes, 0x1C) = -1;

    object = reinterpret_cast<C_00833908*>(self->field_0014);
    CSF_AT(void**, recordBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, recordBytes, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, recordBytes, 0x20) = 0;
    CSF_AT(uint32_t, recordBytes, 0x24) = 0;
    CSF_AT(uint32_t, recordBytes, 0x2C) = 0;
    CSF_AT(uint32_t, recordBytes, 0x28) = 0;
    CSF_AT(uint8_t, recordBytes, 0x30) = 0;
    CSF_AT(void**, recordBytes, 0x00) = CSF_VTBL(0x0082D478);
    CSF_AT(uint32_t, recordBytes, 0x04) = 28;
    CSF_AT(uint32_t, recordBytes, 0x0C) = object->field_0020;
    CSF_AT(uint32_t, recordBytes, 0x10) = object->field_0024;
    CSF_AT(uint32_t, recordBytes, 0x18) = CSF_AT(uint32_t, context, 0x20);
    CSF_AT(uint32_t, recordBytes, 0x1C) = CSF_AT(uint32_t, context, 0x24);

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    DispatchRecordToGlobalObject(dispatcher, nullptr, recordBytes, 0);

    char result = removeObject;
    if (removeObject)
    {
        auto* objectSlot =
            reinterpret_cast<uint8_t*>(self->field_0014) + 0x1C;
        result = static_cast<char>(CScene_RemoveObjectSlot(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
            reinterpret_cast<int>(objectSlot)));
    }

    self->field_0040 = 1;
    return result;
}

HOOK(0x004D6620, C_0083c388_NotifySceneAndRemoveObject, "auto");
