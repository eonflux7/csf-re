#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original: 0x00506BD0, __cdecl, returns the global value object (int-sized pointer).
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
// Original: 0x0050D190, __cdecl, returns the global state object.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
// Original: 0x00428650, __thiscall (ecx = handler object), ret 8. Returns an int code.
CSF_FUNC(int, __fastcall, sub_428650, 0x00428650, (void* self, void* edx, int a2, int a3));
// Original: 0x004286E0, __thiscall (ecx = handler object), ret 8. Returns a pointer.
CSF_FUNC(uint32_t*, __fastcall, sub_4286E0, 0x004286E0, (void* self, void* edx, int a2, int a3));

// Reimplements 0x00428540 (__cdecl, ret 0).
// Checks the global value's handler object (+0x1C) and the global state flag (+0xD0).
// If both are set, forwards the event (code 3, a1) to sub_428650; when it returns 23,
// returns the result of sub_4286E0 with the same arguments.
static int __cdecl MenuOptions_ProcessValue_428540(int a1)
{
    int result = GetGlobalValue_8B056C();
    void* globalValue = reinterpret_cast<void*>(static_cast<uintptr_t>(result));

    int handlerObject = CSF_AT(int, globalValue, 0x1C);
    if (handlerObject != 0)
    {
        result = GetGlobalState_50D190();
        void* globalState = reinterpret_cast<void*>(static_cast<uintptr_t>(result));

        if (CSF_AT(uint8_t, globalState, 0xD0) != 0)
        {
            void* handler = reinterpret_cast<void*>(static_cast<uintptr_t>(handlerObject));

            result = sub_428650(handler, nullptr, 3, a1);
            if (result == 23)
            {
                uint32_t* secondResult = sub_4286E0(handler, nullptr, 3, a1);
                return static_cast<int>(reinterpret_cast<uintptr_t>(secondResult));
            }
        }
    }
    return result;
}

HOOK(0x00428540, MenuOptions_ProcessValue_428540, "auto");
