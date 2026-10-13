#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char, __fastcall, CInputDirectInput_CreateDirectInputObject, 0x00712E50, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, CInputDirectInput_CreateInputDevices, 0x007132F0, (void* self, void* edx, void* hWnd, int a3));
CSF_FUNC(int, __cdecl, ParticleSystemDefs_ConvertAndShowMessage, 0x00701EC0, (int a1, int a2));

static char __fastcall CInputDirectInput_InitializeWithWindow(CInputDirectInput* self, void* /*edx*/, void* hWnd, int a3)
{
    char result = CInputDirectInput_CreateDirectInputObject(self, nullptr,
        static_cast<int>(reinterpret_cast<intptr_t>(hWnd)), a3);
    if (result)
    {
        char succeeded = CInputDirectInput_CreateInputDevices(self, nullptr, hWnd, a3);
        if (!succeeded)
        {
            ParticleSystemDefs_ConvertAndShowMessage(
                reinterpret_cast<int>(CSF_STR(0x008612F8, "g025")),
                reinterpret_cast<int>(CSF_STR(0x00863F54, "g030")));
        }
        return succeeded;
    }
    return result;
}

HOOK(0x00713920, CInputDirectInput_InitializeWithWindow, "auto");
