#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_SubmitSoundRequest, 0x005242B0, (S_00521df0* self, void* , void* request, int listenerInfo));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static int __fastcall S_004d1e30_SubmitSoundRequest(
    S_004d1e30* self, void* /*edx*/, int mode)
{
    void* lookupManager = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC0);
    void* soundManager = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC0);

    const uint32_t soundId = self->field_0088;
    void** soundManagerVtable = CSF_AT(void**, soundManager, 0);
    void** lookupManagerVtable = CSF_AT(void**, lookupManager, 0);

    int resolvedId =
        reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
            lookupManagerVtable[0xC / 4])(lookupManager, nullptr, soundId);
    void* soundDefinition = reinterpret_cast<void*>(
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            soundManagerVtable[8 / 4])(soundManager, nullptr, resolvedId));

    alignas(8) uint8_t requestBytes[0x38];
    alignas(8) uint8_t listenerBytes[0x14];
    S_00526d40& request =
        *reinterpret_cast<S_00526d40*>(requestBytes);

    uint32_t priority = 0;
    int listenerId = -1;
    int listenerChannel = -1;

    CSF_AT(int, requestBytes, 0x18) = 0;
    CSF_AT(int, requestBytes, 0x1C) = 0;
    CSF_AT(int, requestBytes, 0x20) = 0;
    CSF_AT(void**, requestBytes, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, requestBytes, 0x28) = -1;
    CSF_AT(int, requestBytes, 0x2C) = -1;
    request.field_0000 = 1;
    CSF_AT(int, requestBytes, 0x04) = 1;
    CSF_AT(int, requestBytes, 0x10) = 0;
    CSF_AT(int, requestBytes, 0x14) = 0;
    CSF_AT(uint8_t, requestBytes, 0x34) = 0;
    CSF_AT(uint8_t, requestBytes, 0x35) = 1;
    CSF_AT(uint8_t, requestBytes, 0x36) = 1;
    CSF_AT(int, requestBytes, 0x08) = -1;
    CSF_AT(int, requestBytes, 0x0C) = -1;
    CSF_AT(int, requestBytes, 0x30) = 0;

    if (!mode)
    {
        void* listener = reinterpret_cast<void*>(self->field_0014);
        request.field_0000 = 2;
        CSF_AT(int, requestBytes, 0x04) = 1;
        CSF_AT(int, requestBytes, 0x08) =
            CSF_AT(int, soundDefinition, 0x130);
        CSF_AT(float, requestBytes, 0x10) = -1.0f;
        listenerId = CSF_AT(int, listener, 0x20);
        CSF_AT(int, requestBytes, 0x28) = listenerId;
        listenerChannel = CSF_AT(int, listener, 0x24);
        CSF_AT(int, requestBytes, 0x2C) = listenerChannel;
        priority = 1000002;
        self->field_0098 = CSF_AT(float, soundDefinition, 0x128);
    }

    CSF_AT(void**, listenerBytes, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(int, listenerBytes, 0x04) = listenerId;
    CSF_AT(int, listenerBytes, 0x08) = listenerChannel;
    CSF_AT(float, listenerBytes, 0x0C) = static_cast<float>(priority);
    CSF_AT(void**, listenerBytes, 0x10) = soundManagerVtable;

    auto* soundSystem =
        reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    return S_00521df0_SubmitSoundRequest(
        soundSystem, nullptr, requestBytes,
        reinterpret_cast<int>(listenerBytes));
}

HOOK(0x004D2910, S_004d1e30_SubmitSoundRequest, "auto");
