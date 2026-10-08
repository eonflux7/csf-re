#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Ported from 0x00543960 (menu_controls / CMenuControls).
// The original pushes its stack argument (a3) to 4F9B00 across the cdecl call to
// GetValue8B055C, which leaves it on the stack for 4F9B00 to pop. The C++ version
// passes a3 directly as the float argument, which is the same effect.

CSF_FUNC(unsigned int, __cdecl, __controlfp, 0x007B070F, (unsigned int newValue, unsigned int mask));
CSF_FUNC(void, __fastcall, MenuControls_InitDisplayState, 0x00536B90, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_518A50, 0x00518A50, ());
CSF_FUNC(int, __fastcall, GetGlobalValueTarget_518A60, 0x00518A60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0584, 0x00518FD0, ());
CSF_FUNC(void, __fastcall, Nullsub_506AC0, 0x00506AC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0588, 0x00519040, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0560, 0x004FA6B0, ());
CSF_FUNC(char, __fastcall, ApplyGlobalValue_4FA7C0, 0x004FA7C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B055C, 0x004F9660, ());
CSF_FUNC(char, __fastcall, ApplyFloatValue_4F9B00, 0x004F9B00, (void* self, void* edx, float value));

static char __fastcall CMenuControls_ApplyValueAndRefresh(void* self, void* edx, int a3)
{
    __controlfp(0x10000u, 0x30300u);
    MenuControls_InitDisplayState(self, edx, a3);

    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_518A50());
    GetGlobalValueTarget_518A60(globalObject, nullptr);

    void* displayObject = reinterpret_cast<void*>(GetGlobalValue_8B0584());
    Nullsub_506AC0(displayObject, nullptr);

    void* secondaryObject = reinterpret_cast<void*>(GetValue8B0588());
    Nullsub_506AC0(secondaryObject, nullptr);

    void* tertiaryObject = reinterpret_cast<void*>(GetGlobalValue_8B0560());
    ApplyGlobalValue_4FA7C0(tertiaryObject, nullptr);

    int valueObject = GetValue8B055C();
    float floatArg;
    std::memcpy(&floatArg, &a3, sizeof(floatArg));
    return ApplyFloatValue_4F9B00(reinterpret_cast<void*>(valueObject), nullptr, floatArg);
}

HOOK(0x00543960, CMenuControls_ApplyValueAndRefresh, "auto");
