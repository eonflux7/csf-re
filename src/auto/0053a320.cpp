#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x7A8ED0 (__cdecl, caller-cleaned): condition test for an action id.
// Only its low byte is inspected by the original (test al, al).
CSF_FUNC(int, __cdecl, MenuMain_TestCondition, 0x7A8ED0, (int value));

// 0x539D10 (__thiscall): command handler of the owning menu object. The owner
// lives 4 bytes before the sub-object passed in as `self` (lea ecx, [esi-4]).
CSF_FUNC(int, __fastcall, CMenuPause_HandleCommand, 0x539D10, (void* self, void* edx, int command));

// 0x53A320
// If the condition for action id a2 holds and this->field_10 == -1, forwards
// command 1 to the owning menu; otherwise returns the raw predicate result.
static int __fastcall CMenuPause_HandleAction(void* self, void* /*edx*/, int a2, int /*a3*/)
{
    int result = MenuMain_TestCondition(a2);
    if ((result & 0xFF) != 0)
    {
        if (CSF_AT(int, self, 0x10) == -1)
        {
            void* owner = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) - 4);
            return CMenuPause_HandleCommand(owner, nullptr, 1);
        }
    }
    return result;
}

HOOK(0x53A320, CMenuPause_HandleAction, "auto");
