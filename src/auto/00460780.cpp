#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
// Callee takes ecx = provider (a1), edi = caller-inherited (not modelled), and 5 stack args (ret 20).
CSF_FUNC(int, __fastcall, ComputeLerpBetweenPoints, 0x004F7B40,
         (void* self, void* edx, int a2, int a4, float a5, float* a6, float* a7));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __stdcall InterpolateBetweenTwoPositions(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    int globalValue = GetGlobalValue_4FBDE0();
    C_0085e7b8* firstElement =
        reinterpret_cast<C_0085e7b8*>(GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr));
    void* provider = GetCachedObjectProvider(firstElement, nullptr);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(provider));

    if (result != 0)
    {
        // a3[0] -> object -> +4 -> +4 ; a3[1] -> object -> +4 ; a3[2] -> object -> +4 -> +4 (float factor)
        uint32_t firstObject = a3[0];
        int v6 = CSF_AT(int, reinterpret_cast<void*>(firstObject), 0x4);
        uint32_t secondObject = a3[1];
        int v7 = CSF_AT(int, reinterpret_cast<void*>(secondObject), 0x4);
        uint32_t thirdObject = a3[2];
        uint32_t thirdInner = CSF_AT(uint32_t, reinterpret_cast<void*>(thirdObject), 0x4);
        float factor = CSF_AT(float, reinterpret_cast<void*>(thirdInner), 0x4);

        int v8 = v6 + 4;
        int v9 = v7 + 4;

        // Written as !(factor < 0) so that NaN takes the non-negative path, as the original's jp does.
        if (!(factor < g_fZero))
        {
            if (factor > g_TransitionOne || factor >= g_TransitionOne)
                factor = 0.99000001f;
            return ComputeLerpBetweenPoints(provider, nullptr, v8, v9, factor, nullptr, nullptr);
        }
        return ComputeLerpBetweenPoints(provider, nullptr, v8, v9, 0.0f, nullptr, nullptr);
    }
    return result;
}

HOOK(0x00460780, InterpolateBetweenTwoPositions, "auto");
