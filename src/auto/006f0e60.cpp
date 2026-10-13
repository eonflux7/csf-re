#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee at 0x6FD640: __fastcall-style, self in ecx, edx unused, two stack args (ret 8).
CSF_FUNC(char, __fastcall, OwnedObject_ProcessArgWithZeroTail, 0x006FD640,
         (void* self, void* edx, int a2, int a3));

// __thiscall method: this->field_0020 is the owned object; forwards a2 with a zero second argument.
static int __fastcall C_00862390_ForwardArgToOwnedObject(C_00862390* self, void* /*edx*/, int a2)
{
    return OwnedObject_ProcessArgWithZeroTail(
        reinterpret_cast<void*>(self->field_0020), nullptr, a2, 0);
}

HOOK(0x006F0E60, C_00862390_ForwardArgToOwnedObject, "auto");
