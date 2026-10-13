#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Forwards the call to the inner sub-object at this+4 through its vtable slot 32 (+0x80).
// The inner sub-object receives (this+8, a2, a3) as its stack arguments.
static int __fastcall C_0083dfa0_ForwardWithStateToInner(C_0083dfa0* self, void* /*edx*/, int a2, int a3)
{
    uint32_t* innerObj = self ? &self->field_0004 : nullptr;
    C_00833908** innerState = reinterpret_cast<C_00833908**>(reinterpret_cast<uintptr_t>(self) + 8);

    auto innerSlot80 = reinterpret_cast<int(__fastcall*)(void*, void*, C_00833908**, int, int)>(
        CSF_AT(void**, innerObj, 0)[0x80 / 4]);

    return innerSlot80(innerObj, nullptr, innerState, a2, a3);
}

HOOK(0x004DBC80, C_0083dfa0_ForwardWithStateToInner, "auto");
