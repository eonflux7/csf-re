#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class update routine (original 0x004E4910), __thiscall-style via ecx=this.
CSF_FUNC(int, __fastcall, C_0083e7d8_RunBaseUpdate, 0x004E4910, (void* self, void* edx));

// Resets the two state fields, forwards the argument to the object held at +0x0C
// (virtual slot 22), then runs the base update on this object.
static int __fastcall C_0083e7d8_ResetFieldsAndForwardArg(C_0083e7d8* self, void* /*edx*/, int a2)
{
    uint32_t forwardTarget = self->field_000c;

    self->field_0008 = 0;
    self->field_0024 = 0;

    void* forwardObj = reinterpret_cast<void*>(static_cast<uintptr_t>(forwardTarget));
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, forwardObj, 0)[22])(forwardObj, nullptr, a2);

    return C_0083e7d8_RunBaseUpdate(self, nullptr);
}

HOOK(0x004E4CE0, C_0083e7d8_ResetFieldsAndForwardArg, "auto");
