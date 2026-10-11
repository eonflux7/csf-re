#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Float zero constant used by the heading comparison.
CSF_GLOBAL(float, g_fZero, 0x82D4CC);

// Pushes the cached heading (a float) to the provider, as the original's vtbl[16] call does.
// Only the ARG layout matters here; the provider's own signature is unknown.
static void CallProviderSetHeading(void* provider, float heading)
{
    typedef void(__fastcall* SetHeadingFn)(void*, void*, float);
    SetHeadingFn setHeading = (SetHeadingFn)CSF_AT(void**, provider, 0)[16];
    setHeading(provider, nullptr, heading);
}

// Reads the provider's current heading (vtbl[17], returns float in st0).
static float CallProviderGetHeading(void* provider)
{
    typedef float(__fastcall* GetHeadingFn)(void*, void*);
    GetHeadingFn getHeading = (GetHeadingFn)CSF_AT(void**, provider, 0)[17];
    return getHeading(provider, nullptr);
}

// __thiscall method: self = this (ecx), a2 = the single stack argument (pops 4).
static void __fastcall SetProviderHeadingCacheEnabled(void* self, void* /*edx*/, char a2)
{
    if (CSF_AT(char, self, 0x130) != a2)
    {
        if (a2)
        {
            void* provider = CSF_AT(void*, self, 0x4);
            CSF_AT(float, self, 0x134) = CallProviderGetHeading(provider);
            CSF_AT(char, self, 0x130) = a2;
        }
        else
        {
            if (CSF_AT(float, self, 0x134) > g_fZero)
            {
                void* provider = CSF_AT(void*, self, 0x4);
                CallProviderSetHeading(provider, CSF_AT(float, self, 0x134));
                CSF_AT(float, self, 0x134) = -1.0f;
            }
            CSF_AT(char, self, 0x130) = 0;
        }
    }
}

HOOK(0x4F5100, SetProviderHeadingCacheEnabled, "auto");
