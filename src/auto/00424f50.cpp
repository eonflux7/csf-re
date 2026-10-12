#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));

// __thiscall with one stack argument (ret 4), written as __fastcall.
// ecx = owner object; its +0x14 holds the focus-state object (C_00834050).
static int __fastcall SceneLoader_DispatchFocusEntryByState(void* self, void* /*edx*/, int* arg0)
{
    C_00834050* v2 = CSF_AT(C_00834050*, self, 0x14);

    // Copy the three-dword entry locally (ebp-0xC .. ebp-0x4).
    int a2[3];
    a2[0] = arg0[0];
    a2[1] = arg0[1];
    a2[2] = arg0[2];

    int result = v2->field_0050 - 8;
    switch (v2->field_0050)
    {
    case 8u:
    case 9u:
    case 0xAu:
    case 0xBu:
    case 0xEu:
    case 0xFu:
    case 0x10u:
    case 0x11u:
        result = SceneLoader_ConfigureFocusEntry(v2, nullptr, a2);
        break;
    case 0x12u:
    {
        // Virtual slot 0x50 on the handler object (vtable[20]).
        void* handler = reinterpret_cast<void*>(v2->field_004c);
        auto slotFn = reinterpret_cast<int(__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, handler, 0)[0x50 / 4]);
        result = slotFn(handler, nullptr, a2);
        break;
    }
    default:
        return result;
    }
    return result;
}

HOOK(0x00424F50, SceneLoader_DispatchFocusEntryByState, "auto");
