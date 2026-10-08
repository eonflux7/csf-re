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

// Callee: int __thiscall sub_539D10(std::uint32_t **this, int a2)
CSF_FUNC(int, __fastcall, MenuMain_HandleAcceptCommand, 0x00539D10, (void* self, void* edx, int a2));

// Callee: sub_6683F0(this) - receives 'this' in ecx, returns object pointer in eax
CSF_FUNC(void*, __fastcall, MenuMain_GetCommandTarget, 0x006683F0, (void* self, void* edx));

static int __fastcall MenuMain_HandleCommand(void* self, void* /*edx*/, int commandId, int commandArg)
{
    // Commands 1 and 14: when the field at +0x10 is -1, forward to the handler on the
    // sub-object at self-4 with argument 1. Otherwise the original returns 0.
    if (commandId == 1 || commandId == 14)
    {
        if (CSF_AT(int32_t, self, 0x10) == -1)
        {
            void* subObject = reinterpret_cast<uint8_t*>(self) - 4;
            return MenuMain_HandleAcceptCommand(subObject, nullptr, 1);
        }
        return 0;
    }

    // Command 50: if the argument object's byte at +0x38 is set, dispatch through
    // the vtable slot 7 (offset 0x1C) of the command target.
    if (commandId == 50)
    {
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(commandArg), 0x38) != 0)
        {
            void* target = MenuMain_GetCommandTarget(self, nullptr);
            void** vtbl = *reinterpret_cast<void***>(target);
            return reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[7])(target, nullptr);
        }
        return commandArg;
    }

    // Any other command: result is commandId - 50 (the original's eax left by the subtractions).
    return commandId - 50;
}

HOOK(0x0053A2D0, MenuMain_HandleCommand, "auto");
