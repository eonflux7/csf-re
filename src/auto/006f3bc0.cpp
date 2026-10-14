#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdAtomicUpdate, 0x0057C2D0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void, __fastcall, C_00860a30_ClearField000c, 0x006C9FD0, (C_00860a30* self, void* , int));
CSF_FUNC(void, __fastcall, GfxPrtStdParticles_CacheEmitterProperties, 0x006F35C0, (void* self, void* , void* emitter));

__attribute__((flatten))
static int __fastcall C_00862748_ResetAndUpdateParticles(
    C_00862748* self, void* /*edx*/, int resetTiming)
{
    alignas(8) std::uint8_t emitterPropertiesBytes[0x28];
    alignas(8) std::uint8_t updateBytes[0x8];
    alignas(8) std::uint8_t matrixStateBytes[0x8];

    C_00860a30* base = self;

    if (self->field_0035)
    {
        const std::uint32_t pendingMatrix = self->field_0028;

        // 006F3BD3 PUSH EDI is a save. Only LEA/PUSH [ebp-8] is
        // passed to slot 0x4C, whose target 006F3F40 ends in RET 4.
        ((void*(__fastcall*)(C_00860a30*, void*, void*))
            CSF_AT(void**, self, 0)[0x4C / 4])(
                base, nullptr, matrixStateBytes);

        base->vSlot24();
        base->vSetFlagAndOrBits();

        if (pendingMatrix)
        {
            ((void(__fastcall*)(C_00860a30*, void*, std::uint32_t))
                CSF_AT(void**, self, 0)[0x44 / 4])(
                    base, nullptr, pendingMatrix);
        }
        else
        {
            ((void(__fastcall*)(C_00860a30*, void*, void*))
                CSF_AT(void**, self, 0)[0x40 / 4])(
                    base, nullptr, matrixStateBytes);
        }

        const bool ownsMatrix =
            CSF_AT(std::uint8_t, matrixStateBytes, 0x4) != 0;
        self->field_0035 = 0;
        if (ownsMatrix)
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixStateBytes, 0));
    }

    C_00860a30_ClearField000c(base, nullptr, resetTiming);

    const int emitter =
        RpPrtStdAtomicGetEmitter(static_cast<int>(self->field_0024));
    GfxPrtStdParticles_CacheEmitterProperties(
        emitterPropertiesBytes, nullptr, reinterpret_cast<void*>(emitter));

    if (self->field_002c == 1)
    {
        self->field_0040 = 0.0f;
    }
    else
    {
        CSF_AT(std::uint32_t,
            CSF_AT(void*, emitterPropertiesBytes, 0), 0x44) =
            CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(self->field_0030), 0xC);

        CSF_AT(std::uint32_t,
            CSF_AT(void*, emitterPropertiesBytes, 0), 0x8) = 0;
        CSF_AT(std::uint32_t,
            CSF_AT(void*, emitterPropertiesBytes, 0), 0xC) = 0;
        CSF_AT(std::uint32_t,
            CSF_AT(void*, emitterPropertiesBytes, 0), 0x40) = 0;
        CSF_AT(std::uint32_t,
            CSF_AT(void*, emitterPropertiesBytes, 0), 0x3C) = 0;

        if (static_cast<std::uint8_t>(resetTiming))
        {
            void* properties = reinterpret_cast<void*>(self->field_0030);
            self->field_0040 =
                CSF_AT(float, properties, 0x90) +
                CSF_AT(float, properties, 0x18);
        }
        else
        {
            self->field_0040 = 0.0f;
        }
    }

    const int atomic = static_cast<int>(self->field_0024);
    self->field_003c = 0;
    CSF_AT(std::uint32_t, updateBytes, 0) = 0;
    CSF_AT(void*, updateBytes, 0x4) = emitterPropertiesBytes;

    return RpPrtStdAtomicUpdate(atomic, reinterpret_cast<int>(updateBytes));
}

HOOK(0x006F3BC0, C_00862748_ResetAndUpdateParticles, "auto");
