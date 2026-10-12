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

CSF_FUNC(void*, __cdecl, sub_442B60, 0x00442B60, (int a1, char* a2));
CSF_FUNC(int, __fastcall, DevConsole_SetScaledValue, 0x004F9190, (void* self, void* , float value, int state));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void, __fastcall, C_0083f448_SetToggleMode, 0x004FC210, (C_0083f448* self, void* , int mode));

// Dispatches a dynamic-object action by its kind (a1+4): kinds 0,3-6 go to sub_442B60,
// kind 1 adjusts a scaled console value, kind 2 toggles a mode; every path sets the done flag at a1+12.
static C_0085e760* __stdcall DynamicObj_ApplyActionByKind(int a1)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(a1);
    uint32_t kind = CSF_AT(uint32_t, base, 4);
    C_0085e760* result = reinterpret_cast<C_0085e760*>(kind);

    switch (kind)
    {
    case 0u:
    case 3u:
    case 4u:
    case 5u:
    case 6u:
        result = reinterpret_cast<C_0085e760*>(
            sub_442B60(static_cast<int>(kind), CSF_AT(char*, base, 36)));
        break;
    case 1u:
    {
        uint8_t* globalObj = reinterpret_cast<uint8_t*>(GetGlobalValue_4FBDE0());
        float scaledDelta = CSF_AT(float, base, 24) - CSF_AT(float, base, 20);
        void* consoleSelf = CSF_AT(void*, globalObj, 0x74);
        int consoleState = CSF_AT(int, base, 0x1c);
        result = reinterpret_cast<C_0085e760*>(
            DevConsole_SetScaledValue(consoleSelf, nullptr, scaledDelta, consoleState));
        CSF_AT(uint32_t, base, 12) |= 1u;
        return result;
    }
    case 2u:
    {
        int toggleMode = CSF_AT(int, base, 40);
        C_0083f448* toggleSelf = reinterpret_cast<C_0083f448*>(GetGlobalValue_4FBDE0());
        C_0083f448_SetToggleMode(toggleSelf, nullptr, toggleMode);
        result = nullptr;
        CSF_AT(uint32_t, base, 12) |= 1u;
        return result;
    }
    default:
        break;
    }

    CSF_AT(uint32_t, base, 12) |= 1u;
    return result;
}

HOOK(0x442BB0, DynamicObj_ApplyActionByKind, "auto");
