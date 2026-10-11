#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e248_ApplyEventData, 0x004E3E00, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_FindPlayerById, 0x0073E2C0, (uint32_t playerId));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));

static int __fastcall C_0083e248_ApplyPlayerEvent(
    C_0083e248* self, void* /*edx*/,
    int value1, int value2, int value3, int value4,
    int playerId, int value6, int value7, int value8)
{
    GetGlobalDword_73C020();
    int result = NetSession_FindPlayerById(static_cast<uint32_t>(playerId));
    if (!result)
        return result;

    result = GetValueIfNotFlagged(reinterpret_cast<void*>(result), nullptr);
    if (!result)
        return result;

    result = static_cast<int>(static_cast<uint32_t>(result) - 20);
    if (!result)
        return result;

    alignas(8) uint8_t eventBytes[0x2C];
    CSF_AT(int, eventBytes, 0x24) = value1;
    CSF_AT(int, eventBytes, 0x0C) = value3;
    CSF_AT(int, eventBytes, 0x14) = value2;
    CSF_AT(int, eventBytes, 0x18) = value6;
    CSF_AT(int, eventBytes, 0x10) = value4;
    CSF_AT(int, eventBytes, 0x20) = value8;
    CSF_AT(int, eventBytes, 0x1C) = value7;

    void* playerObject = reinterpret_cast<void*>(result);
    const uint32_t source1 = CSF_AT(uint32_t, playerObject, 0x20);
    const uint32_t source2 = CSF_AT(uint32_t, playerObject, 0x24);

    CSF_AT(void**, eventBytes, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(int, eventBytes, 0x28) = 0;
    CSF_AT(uint32_t, eventBytes, 0x04) = source1;
    CSF_AT(uint32_t, eventBytes, 0x08) = source2;

    C_0083e248_ApplyEventData(
        self, nullptr, reinterpret_cast<int>(eventBytes));

    result = SceneObj_IsInactive(
        reinterpret_cast<void*>(self->field_000c), nullptr);
    if (static_cast<uint8_t>(result)) {
        void* sceneObject = reinterpret_cast<void*>(self->field_000c);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, sceneObject, 0)[0x5C / 4])(
                sceneObject, nullptr, playerId);
    }
    return result;
}

HOOK(0x004E16C0, C_0083e248_ApplyPlayerEvent, "auto");
