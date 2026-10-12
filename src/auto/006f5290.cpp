#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, PickSmallerFloatPointer, 0x0040A620, (float* a1, float* a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, ProcessContextStageHandlers, 0x006CA6F0, (void* self, void* , int contextAddress));
CSF_FUNC(void, __fastcall, GfxPrtStdParticles_UpdateEmitter, 0x006F4590, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall GfxPrtStdParticles_UpdateAtomic(
    S_006ca6f0* self, void* /*edx*/, void* context)
{
    (void)CSF_VTBL(0x006CA6F0);

    alignas(8) std::uint8_t matrixBytes[0xC]{};
    alignas(8) std::uint8_t temporaryBytes[0x14]{};
    CSF_AT(S_006ca6f0*, temporaryBytes, 0xC) = self;

    bool failed = false;
    if (CSF_AT(std::uint8_t, context, 0) == 0) {
        void* handler = CSF_AT(void*, context, 4);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, handler, 0)[0x80 / 4])(
                handler, nullptr, 0, context);
    }

    CSF_AT(int, matrixBytes, 8) = CSF_AT(int, context, 0x1C);
    CSF_AT(std::uint8_t, context, 0) = failed;
    ProcessContextStageHandlers(
        self, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(context)));

    bool active = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active) {
        void* handler = CSF_AT(void*, context, 4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, handler, 0)[0x44 / 4])(
                    handler, nullptr, reinterpret_cast<std::uint8_t*>(self) + 0x34,
                    context) == -1;
    }

    active = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active) {
        void* handler = CSF_AT(void*, context, 4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, handler, 0)[0x54 / 4])(
                    handler, nullptr, reinterpret_cast<std::uint8_t*>(self) + 0x38,
                    context) == -1;
    }

    CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, matrixBytes, 4) = 1;

    if (CSF_AT(std::uint8_t, context, 0x14)) {
        void* emitter = reinterpret_cast<std::uint8_t*>(self) - 8;
        void* result =
            reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, emitter, 0)[0x4C / 4])(
                    emitter, nullptr, temporaryBytes);
        std::memcpy(CSF_AT(void*, matrixBytes, 0),
                    CSF_AT(const void*, result, 0), 0x40);
        if (CSF_AT(std::uint8_t, temporaryBytes, 4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, temporaryBytes, 0));
        self = CSF_AT(S_006ca6f0*, temporaryBytes, 0xC);
    }

    active = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active) {
        void* handler = CSF_AT(void*, context, 4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, handler, 0)[0x14 / 4])(
                    handler, nullptr, matrixBytes, context) == -1;
    }

    if (!CSF_AT(std::uint8_t, context, 0x14) &&
        !CSF_AT(std::uint8_t, context, 0) &&
        !CSF_AT(std::uint8_t, self, 4)) {
        auto* emitter = reinterpret_cast<C_00862748*>(
            reinterpret_cast<std::uint8_t*>(self) - 8);
        CSF_AT(C_00862748*, temporaryBytes, 8) = emitter;
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, emitter, 0)[0x40 / 4])(
                emitter, nullptr, matrixBytes);

        void* state = CSF_AT(void*, self, 0x10);
        if (!CSF_AT(std::uint8_t, state, 0x4C)) {
            void* settings = CSF_AT(void*, self, 0x28);
            bool hasPositiveLimit = CSF_AT(float, settings, 0x14) > g_fZero;
            float endTime = CSF_AT(float, settings, 0x90) +
                            CSF_AT(float, settings, 0x18);
            if (hasPositiveLimit)
                endTime = *PickSmallerFloatPointer(
                    &CSF_AT(float, settings, 0x14), &endTime);
            CSF_AT(float, temporaryBytes, 0xC) = endTime;

            float timeStep = CSF_AT(float, settings, 0x10);
            int denominator = CSF_AT(int, settings, 0xC) +
                              CSF_AT(int, settings, 0x88);
            int quotient = CSF_AT(int, settings, 8) / denominator;
            int maximumIteration = static_cast<int>(
                static_cast<std::uint32_t>(quotient) + 1);
            CSF_AT(int, temporaryBytes, 4) = maximumIteration;
            CSF_AT(float, temporaryBytes, 0x10) = timeStep;

            if (timeStep <= CSF_AT(float, temporaryBytes, 0xC)) {
                int iteration = 0;
                while (iteration <= maximumIteration) {
                    CSF_AT(float, self, 0x38) = timeStep;
                    GfxPrtStdParticles_UpdateEmitter(
                        CSF_AT(C_00862748*, temporaryBytes, 8), nullptr);
                    float elapsed =
                        CSF_AT(float, temporaryBytes, 0x10) + timeStep;
                    ++iteration;
                    CSF_AT(float, temporaryBytes, 0x10) = elapsed;
                    if (!(elapsed <= CSF_AT(float, temporaryBytes, 0xC)))
                        break;
                    maximumIteration = CSF_AT(int, temporaryBytes, 4);
                }
            }

            if (CSF_AT(std::uint8_t, self, 0x34)) {
                C_00862748* updateEmitter =
                    CSF_AT(C_00862748*, temporaryBytes, 8);
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, updateEmitter, 0)[0x2C / 4])(
                        updateEmitter, nullptr, 0);
            }
        }
    }

    if (CSF_AT(std::uint8_t, matrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    active = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(int, context, 0x1C) = CSF_AT(int, matrixBytes, 8);
    if (active) {
        void* handler = CSF_AT(void*, context, 4);
        if (reinterpret_cast<bool (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, handler, 0)[0x7C / 4])(
                    handler, nullptr, context)) {
            CSF_AT(std::uint8_t, context, 0) = 0;
            return 0;
        }
    }

    CSF_AT(std::uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x006F5290, GfxPrtStdParticles_UpdateAtomic, "auto");
