#include <cstdint>
#include "globals.h"
#include "types/auto_classes.h"
#include "hook.h"

// Tail-call forwarder (thunk): fetches the object stored at field_0020 and
// tail-jumps to its virtual slot at vtable byte offset 0x48 (index 18),
// passing no stack arguments and returning the callee's result unchanged.
static int __fastcall C_00862390_CallInnerVirtualSlot18(C_00862390* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(self->field_0020);
    return ((int(__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[0x48 / 4])(inner, nullptr);
}

HOOK(0x006F0E80, C_00862390_CallInnerVirtualSlot18, "auto");
