#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, C_008656d0_ProcessWithFlag8, 0x006C68B0, (C_008656d0* self, void* , int a2));
CSF_FUNC(char, __fastcall, IsEmitterObjectInvalid, 0x0070B0E0, (void* self, void* edx));

// Processes the single attached emitter (+0x0C) and the emitter array (+0x10, count +0x18),
// each only when it is valid, using the global object's ProcessWithFlag8.
static char __fastcall ParticleAtomic_ProcessEmitterSlots(void* self, void* /*edx*/)
{
    uint8_t enabled = CSF_AT(uint8_t, self, 4);
    if (enabled == 0) {
        return 0;
    }

    uint32_t attachedEmitter = CSF_AT(uint32_t, self, 0x0C);
    if (attachedEmitter != 0 &&
        !IsEmitterObjectInvalid(reinterpret_cast<void*>(attachedEmitter), nullptr)) {
        auto* globalObj = reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
        C_008656d0_ProcessWithFlag8(globalObj, nullptr, static_cast<int>(attachedEmitter));
    }

    uint32_t count = CSF_AT(uint32_t, self, 0x18);
    for (uint32_t i = 0; i < count; ++i) {
        uint32_t entry = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x10))[i];
        if (entry != 0 && !IsEmitterObjectInvalid(reinterpret_cast<void*>(entry), nullptr)) {
            auto* globalObj = reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
            C_008656d0_ProcessWithFlag8(globalObj, nullptr, static_cast<int>(entry));
        }
    }

    return static_cast<char>(count);
}

HOOK(0x00702910, ParticleAtomic_ProcessEmitterSlots, "auto");
