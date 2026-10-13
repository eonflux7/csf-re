#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082e090_UpdateGroundAdjustedFocus, 0x004162E0, (C_0082e090* self, void* , float* position, float* direction, int traceState));
CSF_FUNC(void, __fastcall, C_0082e090_PlayActivitySound, 0x00416980, (C_0082e090* self, void* , uint32_t* eventData));
CSF_FUNC(void, __fastcall, C_0082e090_HandleCollision, 0x00416AE0, (C_0082e090* self, void* , const float* displacement, uint32_t* collision, int));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, S_0049cfa0_SyncPositionAndNotifyChild, 0x0049CFA0, (S_0049cfa0* self, void* , uint32_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

// Preserve the additional vtable binding listed in the packet.
static const auto kSyncPositionVtable = CSF_VTBL(0x0049CFA0);

static void __fastcall C_0082e090_ProcessMovementEvent(
    C_0082e090* self,
    void* /*edx*/,
    const float* displacement,
    uint32_t* eventData,
    int collisionMode)
{
    void* collisionObject = CSF_AT(void*, eventData, 0x4C);
    if (!collisionObject)
    {
        C_0082e090_UpdateGroundAdjustedFocus(
            self, nullptr,
            reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(eventData) + 0x08),
            reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(eventData) + 0x2C),
            1);
        C_0082e090_PlayActivitySound(self, nullptr, eventData);
        return;
    }

    const int entry = CSF_AT(int, collisionObject, 0x50);
    if (static_cast<uint8_t>(GameConfig_IsResolvedEntryActive(entry)) == 0 &&
        entry != 12)
    {
        C_0082e090_HandleCollision(
            self, nullptr, displacement, eventData, collisionMode);
        return;
    }

    void* focusEntry = reinterpret_cast<void*>(self->field_0004);
    self->field_0077 = 0;

    alignas(8) uint8_t focusPositionBytes[0xC];
    CSF_AT(float, focusPositionBytes, 0x00) =
        CSF_AT(float, eventData, 0x08) + CSF_AT(float, eventData, 0x2C);
    CSF_AT(float, focusPositionBytes, 0x04) =
        CSF_AT(float, eventData, 0x0C) + CSF_AT(float, eventData, 0x30);
    CSF_AT(float, focusPositionBytes, 0x08) =
        CSF_AT(float, eventData, 0x10) + CSF_AT(float, eventData, 0x34);

    SceneLoader_ConfigureFocusEntry(
        focusEntry, nullptr, reinterpret_cast<int*>(focusPositionBytes));

    void* returnedObject = C_0082dd78_DispatchReturnedObject(self, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, returnedObject, 0)[0x28 / 4])(
            returnedObject, nullptr);

    void* globalState = reinterpret_cast<void*>(
        static_cast<uint32_t>(GetGlobalState_50D190()));
    if (!CSF_AT(uint8_t, globalState, 0xD0))
    {
        S_0049cfa0_SyncPositionAndNotifyChild(
            reinterpret_cast<S_0049cfa0*>(self->field_0004),
            nullptr,
            CSF_AT(uint32_t*, eventData, 0x4C));
    }

    C_0082e090_PlayActivitySound(self, nullptr, eventData);
}

HOOK(0x00416D10, C_0082e090_ProcessMovementEvent, "auto");
