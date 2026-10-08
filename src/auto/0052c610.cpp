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

// Callee at 0x6683F0: __thiscall with no stack args, returns a sub-object pointer in eax.
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x6683F0, (void* self, void* edx));

// Dispatches a menu input/command code for a progress bar / text input control.
// Codes 1, 14 and 28 are forwarded to the object one dword before `self`
// (vtable slot 19). Code 50 is forwarded to a child obtained from `self` (vtable slot 7),
// but only when the flag byte at a3+56 is set. Every other code leaves the
// result as (code - 1) truncated to a byte.
static char __fastcall MenuProgress_HandleInputCode(void* self, void* /*edx*/, int a2, int a3)
{
    char result = static_cast<char>(a2 - 1);

    switch (a2)
    {
    case 1:
    case 14:
    case 28:
    {
        // Original: eax = [ecx-4]; ecx = ecx-4; call [eax+0x4c]
        void* owner = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) - 4);
        void** vtbl = *reinterpret_cast<void***>(owner);
        result = reinterpret_cast<char(__fastcall*)(void*, void*)>(vtbl[0x4c / 4])(owner, nullptr);
        break;
    }
    case 50:
    {
        result = CSF_AT(uint8_t, reinterpret_cast<void*>(static_cast<uintptr_t>(a3)), 56);
        if (result)
        {
            void* child = GetReferencedObject(self, nullptr);
            void** vtbl = *reinterpret_cast<void***>(child);
            result = reinterpret_cast<char(__fastcall*)(void*, void*)>(vtbl[0x1c / 4])(child, nullptr);
        }
        break;
    }
    default:
        return result;
    }
    return result;
}

HOOK(0x0052C610, MenuProgress_HandleInputCode, "auto");
