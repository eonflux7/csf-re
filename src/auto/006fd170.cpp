#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x6FD170  (__thiscall, ret 0)
// Adds 0x10 to `this` (-> C_00862f70::field_0010), then tail-calls the virtual
// method at vtable slot 4 (byte offset 0x10) of the object referenced by that
// field, passing that object as the callee's `this`.
static int __fastcall CallInnerObjectVtblSlot4(C_00862f70* self, void* /*edx*/)
{
    // add ecx, 0x10: the inner object is embedded at +0x10 (its address, not a pointer stored there)
    void* inner = reinterpret_cast<uint8_t*>(self) + 0x10;
    void** innerVtbl = *reinterpret_cast<void***>(inner);
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(innerVtbl[4])(inner, nullptr);
}

HOOK(0x6FD170, CallInnerObjectVtblSlot4, "auto");
