#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings

CSF_FUNC(int, __cdecl, DynamicObj_LookupHandlerByType, 0x00442A80, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, DevConsole_ResetState, 0x004F91C0, (void* self, void* /*edx*/, char preserveSecondaryState));
CSF_FUNC(void, __fastcall, C_0083f448_SetToggleMode, 0x004FC210, (C_0083f448* self, void* /*edx*/, int mode));

// Dispatches on the state type stored at +4. Types 0, 3-6 forward to a handler
// looked up by type (vtable slot 15). Type 1 resets the dev console, type 2 sets
// the toggle mode. Every path finally sets the "done" bit (0x2) in the flags at +12.
static int __stdcall DynamicObj_HandleStateTransition(int a1)
{
    void* obj = reinterpret_cast<void*>(a1);
    int stateType = CSF_AT(int, obj, 0x04);
    int result = stateType;

    switch (stateType)
    {
    case 0:
    case 3:
    case 4:
    case 5:
    case 6:
        if (CSF_AT(int, obj, 0x24) && (CSF_AT(uint8_t, obj, 0x0C) & 1) != 0)
        {
            result = DynamicObj_LookupHandlerByType(stateType);
            if (result)
            {
                void* handler = reinterpret_cast<void*>(result);
                auto handlerFn = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, handler, 0)[15]);
                result = handlerFn(handler, nullptr, CSF_AT(int, obj, 0x24));
            }
        }
        break;

    case 1:
    {
        void* globalObj = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        void* devConsole = CSF_AT(void*, globalObj, 0x74);
        result = DevConsole_ResetState(devConsole, nullptr, static_cast<char>(CSF_AT(uint8_t, obj, 0x20)));
        break;
    }

    case 2:
    {
        void* globalObj = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        C_0083f448_SetToggleMode(reinterpret_cast<C_0083f448*>(globalObj), nullptr, 0);
        result = 0;
        break;
    }

    default:
        break;
    }

    CSF_AT(uint32_t, obj, 0x0C) |= 2u;
    return result;
}

HOOK(0x00442C40, DynamicObj_HandleStateTransition, "auto");
