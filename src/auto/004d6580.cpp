#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083c388_DispatchRecordAndOptionallyRemoveObject(
    C_0083c388* self, void* /*edx*/, char removeObject)
{
    void* context = reinterpret_cast<void*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));

    alignas(8) uint8_t record[0x34];
    CSF_AT(int, record, 0x0C) = -1;
    CSF_AT(int, record, 0x10) = -1;
    CSF_AT(int, record, 0x18) = -1;
    CSF_AT(int, record, 0x1C) = -1;

    auto* object = reinterpret_cast<C_0083b958*>(self->field_0014);

    CSF_AT(void**, record, 0x00) = CSF_VTBL(0x0082D478);
    CSF_AT(void**, record, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, record, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(int, record, 0x04) = 28;
    CSF_AT(int, record, 0x20) = 0;
    CSF_AT(int, record, 0x24) = 0;
    CSF_AT(int, record, 0x2C) = 0;
    CSF_AT(int, record, 0x28) = 0;
    CSF_AT(uint8_t, record, 0x30) = 0;

    CSF_AT(uint32_t, record, 0x0C) = object->field_0020;
    CSF_AT(uint32_t, record, 0x10) = object->field_0024;
    CSF_AT(uint32_t, record, 0x18) = CSF_AT(uint32_t, context, 0x20);
    CSF_AT(uint32_t, record, 0x1C) = CSF_AT(uint32_t, context, 0x24);

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    int result = static_cast<int>(
        DispatchRecordToGlobalObject(dispatcher, nullptr, record, 0));

    if (removeObject)
    {
        uint8_t* objectSlot =
            reinterpret_cast<uint8_t*>(self->field_0014) + 0x1C;
        result = CScene_RemoveObjectSlot(
            reinterpret_cast<CScene*>(g_pCurrentScene),
            nullptr,
            reinterpret_cast<int>(objectSlot));
    }

    self->field_0040 = 1;
    return result;
}

HOOK(0x004D6580, C_0083c388_DispatchRecordAndOptionallyRemoveObject, "auto");
