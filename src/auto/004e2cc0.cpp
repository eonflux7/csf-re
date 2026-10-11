#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083e160_GetLinkedValueDC, 0x004E1110, (C_0083e160* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float, __fastcall, MenuSystem_GetTransitionScale_50D860, 0x0050D860, (void* self, void*));

static uint64_t __fastcall C_0083e248_GetScaledLinkedValue(C_0083e248* self, void* /*edx*/)
{
    uint32_t transitionStateAddr = self->field_000c;
    float transitionScale = 1.0f;

    if (transitionStateAddr != 0)
    {
        uint8_t* transitionState = reinterpret_cast<uint8_t*>(transitionStateAddr);
        if (CSF_AT(int, transitionState, 0x50) == 1)
        {
            int globalState = GetGlobalState_50D190();
            transitionScale = MenuSystem_GetTransitionScale_50D860(
                reinterpret_cast<void*>(static_cast<intptr_t>(globalState)), nullptr);
        }
    }

    int linkedValue = C_0083e160_GetLinkedValueDC(self, nullptr);
    double scaled = static_cast<double>(linkedValue) * static_cast<double>(transitionScale);
    return static_cast<uint64_t>(static_cast<int64_t>(scaled));
}

HOOK(0x004E2CC0, C_0083e248_GetScaledLinkedValue, "auto");
