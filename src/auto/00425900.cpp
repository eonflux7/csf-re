#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_SubmitSoundRequest, 0x005242B0, (S_00521df0* self, void* , void* request, int listenerInfo));
CSF_FUNC(int, __fastcall, ResolveSlotIndexForState, 0x00756810, (void* self, void* , int slotIndex, int stateCode));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall C_0082fdb8_SubmitStateSoundRequest(
    C_0082fdb8* self, void* /*edx*/)
{
    const uint32_t state = self->field_0020;
    if (state == 0)
        return 0;

    int slotIndex = 2;
    uint8_t enabled = 0;
    switch (state)
    {
    case 1:
        slotIndex = 2;
        enabled = 1;
        break;
    case 2:
        slotIndex = 1;
        break;
    case 3:
        slotIndex = 3;
        enabled = 1;
        break;
    default:
        break;
    }

    alignas(8) uint8_t requestBytes[0x38];
    alignas(8) uint8_t listenerBytes[0x14];

    CSF_AT(int, requestBytes, 0x28) = -1;
    CSF_AT(int, requestBytes, 0x2C) = -1;
    CSF_AT(int, requestBytes, 0x08) = -1;
    CSF_AT(int, requestBytes, 0x0C) = -1;

    void* owner = reinterpret_cast<void*>(self->field_0014);

    CSF_AT(int, requestBytes, 0x18) = 0;
    CSF_AT(int, requestBytes, 0x1C) = 0;
    CSF_AT(int, requestBytes, 0x20) = 0;
    CSF_AT(void**, requestBytes, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, requestBytes, 0x04) = 1;
    CSF_AT(int, requestBytes, 0x10) = 0;
    CSF_AT(int, requestBytes, 0x14) = 0;
    CSF_AT(uint8_t, requestBytes, 0x34) = 0;
    CSF_AT(uint8_t, requestBytes, 0x35) = 1;
    CSF_AT(uint8_t, requestBytes, 0x36) = 1;
    CSF_AT(int, requestBytes, 0x30) = 0;
    CSF_AT(int, requestBytes, 0x00) = 2;

    CSF_AT(int, requestBytes, 0x28) = CSF_AT(int, owner, 0x20);
    CSF_AT(int, requestBytes, 0x2C) = CSF_AT(int, owner, 0x24);
    const int stateCode = CSF_AT(int, owner, 0x50);

    auto* globals = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    CSF_AT(int, requestBytes, 0x08) =
        ResolveSlotIndexForState(globals + 0x22A4, nullptr, slotIndex, stateCode);

    CSF_AT(int, listenerBytes, 0x04) = CSF_AT(int, requestBytes, 0x28);
    CSF_AT(uint8_t, requestBytes, 0x36) = enabled;
    CSF_AT(void**, listenerBytes, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(int, listenerBytes, 0x08) = CSF_AT(int, requestBytes, 0x2C);
    CSF_AT(int, listenerBytes, 0x0C) = 1232348224;

    auto* soundManager =
        reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    const int result = S_00521df0_SubmitSoundRequest(
        soundManager, nullptr, requestBytes,
        reinterpret_cast<int>(listenerBytes));
    self->field_0028 = result;
    return result;
}

HOOK(0x00425900, C_0082fdb8_SubmitStateSoundRequest, "auto");
