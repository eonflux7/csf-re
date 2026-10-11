#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008344f0_ResetLocalState, 0x004A3670, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, AudioSoundManager_DispatchGlobalValueMessage, 0x00522330, (void* self, void* , const void* message));

static int __fastcall C_008344f0_ConfigureAndDispatchGlobalValue(C_008344f0* self, void* /*edx*/, uint32_t a2, uint32_t a3, uint32_t a4)
{
    self->field_0048 = a3;
    self->field_0040 = 1;
    self->field_0044 = a2;
    self->field_004c = a4;

    C_008344f0_ResetLocalState(self, nullptr);
    self->field_0060 = 0.0f;

    uint8_t* audioState = reinterpret_cast<uint8_t*>(self->field_0008) + 0x1C;

    alignas(8) uint32_t message[4];
    message[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    message[1] = CSF_AT(uint32_t, audioState, 0x4);
    message[2] = CSF_AT(uint32_t, audioState, 0x8);
    message[3] = 0x49742430;

    int globalValue = GetGlobalValue_521BB0();
    return AudioSoundManager_DispatchGlobalValueMessage(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr, message);
}

HOOK(0x004A3AE0, C_008344f0_ConfigureAndDispatchGlobalValue, "auto");
