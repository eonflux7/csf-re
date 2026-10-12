#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(void, __fastcall, C_00860a30_SetFlag0C, 0x006C9FE0, (C_00860a30* self, void* , int));
CSF_FUNC(void, __fastcall, RpPrtStdEmitterInit, 0x006F35C0, (void* self, void* edx, int a2));

static void __fastcall C_00862748_SetupAtomicEmitter(C_00862748* self, void* /*edx*/, char a2)
{
    uint32_t atomicHandle = self->field_0024;
    self->field_003c = 1;

    int emitter = RpPrtStdAtomicGetEmitter(static_cast<int>(atomicHandle));

    alignas(8) uint8_t emitterBytes[0x28];
    RpPrtStdEmitterInit(emitterBytes, nullptr, emitter);

    if (self->field_002c != 1) {
        uint32_t objPtr = reinterpret_cast<uint32_t*>(emitterBytes)[0];
        void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(objPtr));
        CSF_AT(uint32_t, target, 0x44) = 0;
    }

    if (a2 != 0) {
        C_00860a30_SetFlag0C(self, nullptr, 1);
    }
}

HOOK(0x6F3CC0, C_00862748_SetupAtomicEmitter, "auto");
