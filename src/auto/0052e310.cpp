#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x521BB0, ());
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x506AC0, ());
CSF_FUNC(void, __fastcall, GlobalSetValue_5231D0, 0x5231D0, (int globalValue, void* edx, int deltaBits));
CSF_FUNC(void, __fastcall, CMenuScenarioSelect_RefreshState, 0x52DF20, (CMenuScenarioSelect* self, void* edx));
CSF_FUNC(void, __fastcall, CMenuScenarioSelect_UpdateAnimations, 0x52DD80, (CMenuScenarioSelect* self, void* edx, float deltaTime));
CSF_FUNC(int, __fastcall, CMenuScenarioSelect_MapProxyCodeToGlobalMode, 0x52D9C0, (CMenuScenarioSelect* self, void* edx));
CSF_FUNC(int, __fastcall, Sub_521F10, 0x521F10, (int globalValue, void* edx));
CSF_GLOBAL(const char*, g_pszMenu_P_LbMePr, 0x899B40);

// Stores the frame delta, refreshes the screen, then dispatches on the screen state.
static int __fastcall CMenuScenarioSelect_UpdateWithDelta(CMenuScenarioSelect* self, void* /*edx*/, float deltaTime)
{
    // Store the raw float bits of the delta.
    std::memcpy(&self->field_0074, &deltaTime, sizeof(float));

    // The raw bits are passed to the helper as an int argument.
    std::int32_t deltaBits = 0;
    std::memcpy(&deltaBits, &deltaTime, sizeof(float));

    GlobalSetValue_5231D0(GetGlobalValue_521BB0(), nullptr, deltaBits);
    Nullsub_506AC0();
    CMenuScenarioSelect_RefreshState(self, nullptr);
    CMenuScenarioSelect_UpdateAnimations(self, nullptr, deltaTime);

    const std::uint32_t stateCode = self->field_0038;
    if (stateCode == 1)
    {
        void* firstTarget = reinterpret_cast<void*>(self->field_0014);
        auto setModeFn = reinterpret_cast<void (__fastcall*)(void*, void*, CMenuScenarioSelect*, int)>(
            CSF_AT(void**, firstTarget, 0)[24]);
        setModeFn(firstTarget, nullptr, self, 0);

        void* secondTarget = reinterpret_cast<void*>(self->field_0014);
        auto setTextFn = reinterpret_cast<void (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, secondTarget, 0)[22]);
        setTextFn(secondTarget, nullptr, g_pszMenu_P_LbMePr);

        return Sub_521F10(GetGlobalValue_521BB0(), nullptr);
    }
    if (stateCode == 2)
    {
        return CMenuScenarioSelect_MapProxyCodeToGlobalMode(self, nullptr);
    }
    return static_cast<int>(stateCode - 2);
}

HOOK(0x0052E310, CMenuScenarioSelect_UpdateWithDelta, "auto");
