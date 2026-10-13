#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(double, g_FrameCountIncrement, 0x0084C8A8);
CSF_GLOBAL(double, g_FrameRateTimeScale, 0x0085E910);
CSF_GLOBAL(float, g_FrameRateWindow, 0x00861A2C);
CSF_GLOBAL(float, g_MinimumFrameRateWindow, 0x00861A30);

static void __fastcall GfxIm3d_UpdateFrameRates(void* self, void* /*edx*/)
{
    void* timer = CSF_AT(void*, self, 0x3C);
    if (((bool(__fastcall*)(void*, void*))
             CSF_AT(void**, timer, 0)[3])(timer, nullptr))
    {
        timer = CSF_AT(void*, self, 0x3C);
        ((void(__fastcall*)(void*, void*))
             CSF_AT(void**, timer, 0)[1])(timer, nullptr);

        timer = CSF_AT(void*, self, 0x3C);
        ((void(__fastcall*)(void*, void*))
             CSF_AT(void**, timer, 0)[2])(timer, nullptr);
    }

    timer = CSF_AT(void*, self, 0x3C);
    const double elapsed = ((double(__fastcall*)(void*, void*))
                                CSF_AT(void**, timer, 0)[5])(timer, nullptr);
    const float elapsedFloat = static_cast<float>(elapsed);

    const double totalTime = elapsed + CSF_AT(double, self, 0x20);
    CSF_AT(double, self, 0x20) = totalTime;

    const float windowTime = elapsedFloat + CSF_AT(float, self, 0x14);
    CSF_AT(float, self, 0x14) = windowTime;

    const float minimumWindowTime = elapsedFloat + CSF_AT(float, self, 0x18);
    const double totalFrames =
        CSF_AT(double, self, 0x28) + g_FrameCountIncrement;
    CSF_AT(float, self, 0x18) = minimumWindowTime;
    CSF_AT(double, self, 0x28) = totalFrames;

    const float windowFrames =
        CSF_AT(float, self, 0x30) + g_TransitionOne;
    CSF_AT(float, self, 0x30) = windowFrames;

    const float minimumWindowFrames =
        CSF_AT(float, self, 0x34) + g_TransitionOne;
    CSF_AT(float, self, 0x34) = minimumWindowFrames;

    if (windowTime > g_FrameRateWindow)
    {
        CSF_AT(float, self, 0x30) = 0.0f;
        CSF_AT(float, self, 0x14) = 0.0f;
        CSF_AT(float, self, 0x08) =
            windowFrames * g_FrameRateWindow / windowTime;
    }

    if (minimumWindowTime > g_MinimumFrameRateWindow)
    {
        const int32_t warmupWindows = CSF_AT(int32_t, self, 0x38);
        if (warmupWindows > 0)
        {
            CSF_AT(int32_t, self, 0x38) = warmupWindows - 1;
        }
        else
        {
            const float candidate =
                minimumWindowFrames * g_MinimumFrameRateWindow /
                minimumWindowTime * g_SoundActivityThreshold;
            const float current = CSF_AT(float, self, 0x0C);

            // The original selects the candidate for unordered comparisons too.
            CSF_AT(float, self, 0x0C) =
                current < candidate ? current : candidate;
        }

        CSF_AT(float, self, 0x34) = 0.0f;
        CSF_AT(float, self, 0x18) = 0.0f;
    }

    CSF_AT(float, self, 0x10) =
        static_cast<float>(totalFrames / totalTime * g_FrameRateTimeScale);
}

HOOK(0x006E2D50, GfxIm3d_UpdateFrameRates, "auto");
