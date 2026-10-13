#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, PickSmallerFloatPointer, 0x0040A620, (float* a1, float* a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, ProcessContextStageHandlers, 0x006CA6F0, (void* self, void* , int contextAddress));
CSF_FUNC(void, __fastcall, C_00862748_UpdateParticles_6F4590, 0x006F4590, (C_00862748* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
static const auto g_ContextStageHandlersVtable = CSF_VTBL(0x006CA6F0);

static int __fastcall GfxPrtStdParticles_UpdateAtomic(
    S_006ca6f0* self, void* /*edx*/, void* context)
{
    alignas(8) std::uint8_t matrixState[0xC];
    alignas(8) std::uint8_t matrixResult[0x14];

    bool failed = false;
    if (CSF_AT(std::uint8_t, context, 0) == 0)
    {
        void* handler = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, handler, 0)[0x80 / 4])(
                handler, nullptr, 0, context);
    }

    CSF_AT(std::uint32_t, matrixState, 0x8) =
        CSF_AT(std::uint32_t, context, 0x1C);
    CSF_AT(std::uint8_t, context, 0) = failed;

    ProcessContextStageHandlers(
        self, nullptr, static_cast<int>(reinterpret_cast<std::intptr_t>(context)));

    bool runStage = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (runStage)
    {
        void* handler = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, handler, 0)[0x44 / 4])(
                    handler, nullptr, self->pad_0034, context) == -1;
    }

    runStage = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (runStage)
    {
        void* handler = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, handler, 0)[0x54 / 4])(
                    handler, nullptr, &self->pad_0034[4], context) == -1;
    }

    CSF_AT(std::uint32_t*, matrixState, 0) = RwMatrixCreate();
    const bool copyEmitterMatrix = CSF_AT(std::uint8_t, context, 0x14) != 0;
    CSF_AT(std::uint8_t, matrixState, 0x4) = 1;

    if (copyEmitterMatrix)
    {
        auto* emitter = reinterpret_cast<C_00862748*>(
            reinterpret_cast<std::uint8_t*>(self) - 8);
        void* result =
            reinterpret_cast<void* (__fastcall*)(C_00862748*, void*, void*)>(
                emitter->vtbl[0x4C / 4])(emitter, nullptr, matrixResult);
        std::memcpy(
            CSF_AT(std::uint32_t*, matrixState, 0),
            CSF_AT(const void*, result, 0), 0x40);
        if (CSF_AT(std::uint8_t, matrixResult, 0x4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResult, 0));
    }

    runStage = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (runStage)
    {
        void* handler = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, handler, 0)[0x14 / 4])(
                    handler, nullptr, matrixState, context) == -1;
    }

    if (!CSF_AT(std::uint8_t, context, 0x14) &&
        !CSF_AT(std::uint8_t, context, 0) &&
        !self->pad_0000[4])
    {
        auto* emitter = reinterpret_cast<C_00862748*>(
            reinterpret_cast<std::uint8_t*>(self) - 8);
        reinterpret_cast<void (__fastcall*)(C_00862748*, void*, void*)>(
            emitter->vtbl[0x40 / 4])(emitter, nullptr, matrixState);

        if (!CSF_AT(std::uint8_t, CSF_AT(void*, self, 0x10), 0x4C))
        {
            void* timing = CSF_AT(void*, self, 0x28);
            const bool hasTimeLimit = CSF_AT(float, timing, 0x14) > g_fZero;
            float updateDuration =
                CSF_AT(float, timing, 0x90) + CSF_AT(float, timing, 0x18);
            float durationLimit;
            if (hasTimeLimit)
                durationLimit = *PickSmallerFloatPointer(
                    &CSF_AT(float, timing, 0x14), &updateDuration);
            else
                durationLimit = updateDuration;

            const float step = CSF_AT(float, timing, 0x10);
            const std::int32_t divisorBase = CSF_AT(std::int32_t, timing, 0xC);
            const std::int32_t particleCount = CSF_AT(std::int32_t, timing, 0x8);
            const std::int32_t iterationLimit =
                particleCount /
                    (divisorBase + CSF_AT(std::int32_t, timing, 0x88)) + 1;
            float elapsed = step;

            if (step <= durationLimit)
            {
                for (std::int32_t iteration = 0;
                     iteration <= iterationLimit; ++iteration)
                {
                    CSF_AT(float, self, 0x38) = step;
                    C_00862748_UpdateParticles_6F4590(emitter, nullptr);
                    elapsed += step;
                    if (!(elapsed <= durationLimit))
                        break;
                }
            }

            if (self->pad_0034[0])
            {
                reinterpret_cast<void (__fastcall*)(C_00862748*, void*, int)>(
                    emitter->vtbl[0x2C / 4])(emitter, nullptr, 0);
            }
        }
    }

    if (CSF_AT(std::uint8_t, matrixState, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixState, 0));

    const bool canFinish = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) =
        CSF_AT(std::uint32_t, matrixState, 0x8);
    if (canFinish)
    {
        void* handler = CSF_AT(void*, context, 0x4);
        if (reinterpret_cast<bool (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, handler, 0)[0x7C / 4])(
                    handler, nullptr, context))
        {
            CSF_AT(std::uint8_t, context, 0) = 0;
            return 0;
        }
    }

    CSF_AT(std::uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x006F5290, GfxPrtStdParticles_UpdateAtomic, "auto");
