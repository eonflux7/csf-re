#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdAtomicUpdate, 0x0057C2D0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void, __fastcall, C_00860a30_ClearField000c, 0x006C9FD0, (C_00860a30* self, void* , int));
CSF_FUNC(void, __fastcall, InitializeEmitterUpdateData, 0x006F35C0, (void* self, void* edx, int a2));

static int __fastcall C_00862748_ResetAndUpdateParticles(
    C_00862748* self, void* /*edx*/, int enabled)
{
    alignas(8) std::uint8_t emitterBytes[0x28]{};
    alignas(8) std::uint8_t updateBytes[0x8]{};
    alignas(8) std::uint8_t matrixBytes[0x8]{};

    if (self->field_0035)
    {
        const std::uint32_t existingMatrix = self->field_0028;

        ((void(__fastcall*)(void*, void*, void*))self->vtbl[0x4C / 4])(
            self, nullptr, matrixBytes);
        self->vSlot24();
        self->vSetFlagAndOrBits();

        if (existingMatrix)
        {
            ((void(__fastcall*)(void*, void*, std::uint32_t))self->vtbl[0x44 / 4])(
                self, nullptr, existingMatrix);
        }
        else
        {
            ((void(__fastcall*)(void*, void*, void*))self->vtbl[0x40 / 4])(
                self, nullptr, matrixBytes);
        }

        const bool destroyMatrix = CSF_AT(std::uint8_t, matrixBytes, 0x4) != 0;
        self->field_0035 = 0;
        if (destroyMatrix)
        {
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(
                CSF_AT(std::uint32_t, matrixBytes, 0x0)));
        }
    }

    C_00860a30_ClearField000c(self, nullptr, enabled);
    const int emitter = RpPrtStdAtomicGetEmitter(static_cast<int>(self->field_0024));
    InitializeEmitterUpdateData(emitterBytes, nullptr, emitter);

    if (self->field_002c == 1)
    {
        self->field_0040 = 0.0f;
    }
    else
    {
        CSF_AT(std::uint32_t,
            reinterpret_cast<void*>(CSF_AT(std::uint32_t, emitterBytes, 0x0)), 0x44) =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0030), 0xC);
        CSF_AT(std::uint32_t,
            reinterpret_cast<void*>(CSF_AT(std::uint32_t, emitterBytes, 0x0)), 0x8) = 0;
        CSF_AT(std::uint32_t,
            reinterpret_cast<void*>(CSF_AT(std::uint32_t, emitterBytes, 0x0)), 0xC) = 0;
        CSF_AT(std::uint32_t,
            reinterpret_cast<void*>(CSF_AT(std::uint32_t, emitterBytes, 0x0)), 0x40) = 0;
        CSF_AT(std::uint32_t,
            reinterpret_cast<void*>(CSF_AT(std::uint32_t, emitterBytes, 0x0)), 0x3C) = 0;

        if (static_cast<std::uint8_t>(enabled))
        {
            void* parameters = reinterpret_cast<void*>(self->field_0030);
            self->field_0040 =
                CSF_AT(float, parameters, 0x90) + CSF_AT(float, parameters, 0x18);
        }
        else
        {
            self->field_0040 = 0.0f;
        }
    }

    const int atomic = static_cast<int>(self->field_0024);
    self->field_003c = 0;
    CSF_AT(std::uint32_t, updateBytes, 0x0) = 0;
    CSF_AT(std::uint32_t, updateBytes, 0x4) =
        reinterpret_cast<std::uint32_t>(emitterBytes);
    return RpPrtStdAtomicUpdate(atomic, reinterpret_cast<int>(updateBytes));
}

HOOK(0x006F3BC0, C_00862748_ResetAndUpdateParticles, "auto");
