#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using CurrentHandleFn = void* (__stdcall*)();
using GetProcessPriorityFn = std::uint32_t (__stdcall*)(void*);
using GetThreadPriorityFn = int (__stdcall*)(void*);
using GetAffinityFn = int (__stdcall*)(void*, std::uint32_t*, std::uint32_t*);
using SetProcessPriorityFn = int (__stdcall*)(void*, std::uint32_t);
using SetThreadPriorityFn = int (__stdcall*)(void*, int);
using SetAffinityFn = int (__stdcall*)(void*, std::uint32_t);
using SleepFn = void (__stdcall*)(std::uint32_t);

// The import consumes one stack argument. The register parameters preserve
// the original call site's ECX and EDX without changing its stack ABI.
using QueryFrequencyFn =
    int (__fastcall*)(C_0085e92c*, void*, std::uint64_t*);

CSF_GLOBAL(CurrentHandleFn, g_GetCurrentProcessIat, 0x0082D248);
CSF_GLOBAL(CurrentHandleFn, g_GetCurrentThreadIat, 0x0082D24C);
CSF_GLOBAL(GetProcessPriorityFn, g_GetPriorityClassIat, 0x0082D250);
CSF_GLOBAL(GetThreadPriorityFn, g_GetThreadPriorityIat, 0x0082D254);
CSF_GLOBAL(GetAffinityFn, g_GetProcessAffinityMaskIat, 0x0082D258);
CSF_GLOBAL(SetProcessPriorityFn, g_SetPriorityClassIat, 0x0082D25C);
CSF_GLOBAL(SetThreadPriorityFn, g_SetThreadPriorityIat, 0x0082D208);
CSF_GLOBAL(SetAffinityFn, g_SetProcessAffinityMaskIat, 0x0082D260);
CSF_GLOBAL(SleepFn, g_SleepImportSlot, 0x0082D05C);
CSF_GLOBAL(QueryFrequencyFn, g_QueryPerformanceFrequencyIat, 0x0082D0C4);

CSF_GLOBAL(std::uint32_t, g_CalibratedFrequencyLow, 0x008BAFB8);
CSF_GLOBAL(std::uint32_t, g_CalibratedFrequencyHigh, 0x008BAFBC);

CSF_FUNC(uint64_t, __fastcall, C_0085e92c_ReadCounter, 0x0069C9B0,
         (void* self, void* edx));

static int __fastcall C_0085e92c_InitializeFrequency(
    C_0085e92c* self, void* /*edx*/)
{
    alignas(8) std::uint8_t frequencyBytes[0x8]{};
    auto& frequency = *reinterpret_cast<std::uint64_t*>(frequencyBytes);
    auto* frequencyWords =
        reinterpret_cast<std::uint32_t*>(&self->field_0018);

    if (self->field_003c)
    {
        if ((g_CalibratedFrequencyLow | g_CalibratedFrequencyHigh) == 0)
        {
            alignas(8) std::uint8_t affinityBytes[0xC]{};
            alignas(8) std::uint8_t processAffinityBytes[0x4]{};

            auto& systemAffinity =
                *reinterpret_cast<std::uint32_t*>(affinityBytes);
            auto& previousPriorityClass =
                *reinterpret_cast<std::uint32_t*>(affinityBytes + 0x4);
            auto& previousThreadPriority =
                *reinterpret_cast<int*>(affinityBytes + 0x8);
            auto& processAffinity =
                *reinterpret_cast<std::uint32_t*>(processAffinityBytes);

            void* process = g_GetCurrentProcessIat();
            void* thread = g_GetCurrentThreadIat();
            previousPriorityClass = g_GetPriorityClassIat(process);
            previousThreadPriority = g_GetThreadPriorityIat(thread);
            g_GetProcessAffinityMaskIat(
                process, &processAffinity, &systemAffinity);
            g_SetPriorityClassIat(process, 0x100u);
            g_SetThreadPriorityIat(thread, 15);
            g_SetProcessAffinityMaskIat(process, 1u);

            frequency = C_0085e92c_ReadCounter(self, nullptr);
            g_SleepImportSlot(1000);

            const std::uint64_t elapsed =
                C_0085e92c_ReadCounter(self, nullptr) - frequency;
            g_CalibratedFrequencyLow = static_cast<std::uint32_t>(elapsed);
            g_CalibratedFrequencyHigh =
                static_cast<std::uint32_t>(elapsed >> 32);

            g_SetProcessAffinityMaskIat(process, processAffinity);
            g_SetThreadPriorityIat(thread, previousThreadPriority);
            g_SetPriorityClassIat(process, previousPriorityClass);
        }

        const std::uint32_t low = g_CalibratedFrequencyLow;
        frequencyWords[0] = low;
        frequencyWords[1] = g_CalibratedFrequencyHigh;
        return static_cast<int>(low);
    }

    const bool available =
        g_QueryPerformanceFrequencyIat(self, &frequency, &frequency) != 0;
    self->field_003d = static_cast<std::uint8_t>(available);

    if (available)
    {
        const std::uint32_t low = static_cast<std::uint32_t>(frequency);
        frequencyWords[0] = low;
        frequencyWords[1] = static_cast<std::uint32_t>(frequency >> 32);
        return static_cast<int>(low);
    }

    frequencyWords[0] = 1000;
    frequencyWords[1] = 0;
    return 0;
}

HOOK(0x0069CBF0, C_0085e92c_InitializeFrequency, "auto");
