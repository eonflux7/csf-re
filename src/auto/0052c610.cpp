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
// but only when the flag byte at a3+56 is set. The original returns the whole eax, not
// a byte: the callee's full eax for the forwarded codes, the flag zero-extended for code 50.
// Other codes: the original computes eax = code - 1, and when (code - 1) <= 49 (unsigned)
// it looks up a jump-table slot index in a byte table (slot 2 = default) and overwrites eax
// with that index before jumping, so the default path returns 2 in eax. Codes with
// (code - 1) > 49 (unsigned) return code - 1 in full.
static int __fastcall MenuProgress_HandleInputCode(void* self, void* /*edx*/, int a2, int a3)
{
    int result = a2 - 1;

    switch (a2)
    {
    case 1:
    case 14:
    case 28:
    {
        // Original: eax = [ecx-4]; ecx = ecx-4; call [eax+0x4c]
        void* owner = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) - 4);
        void** vtbl = *reinterpret_cast<void***>(owner);
        result = reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[0x4c / 4])(owner, nullptr);
        break;
    }
    case 50:
    {
        result = CSF_AT(uint8_t, reinterpret_cast<void*>(static_cast<uintptr_t>(a3)), 56);
        if (result)
        {
            void* child = GetReferencedObject(self, nullptr);
            void** vtbl = *reinterpret_cast<void***>(child);
            result = reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[0x1c / 4])(child, nullptr);
        }
        break;
    }
    default:
        // Original: eax = code - 1; if ((unsigned)eax <= 49) eax = jump-table slot index (2 = default)
        if (static_cast<unsigned int>(a2 - 1) <= 49u)
            return 2;
        return result;
    }
    return result;
}

HOOK(0x0052C610, MenuProgress_HandleInputCode, "auto");
