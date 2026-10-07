#include <cstdint>
#include "hook.h"

// 0x0052C6C0 (__thiscall, callee pops 4 bytes -> one ignored stack argument)
// Adjusts the incoming `this` pointer down by 4 bytes (secondary base -> complete
// object), loads the vtable pointer stored at that address and dispatches the
// virtual method at vtable byte offset 0x4C on the adjusted object.
static int __fastcall CallVirtualOnAdjustedThis(void* self, void* /*edx*/, int /*unusedArg*/)
{
    uint8_t* adjustedObject = static_cast<uint8_t*>(self) - 4;
    void** vtbl = *reinterpret_cast<void***>(adjustedObject);

    // __fastcall callee = __thiscall callee with no stack args (ret 0)
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[0x4C / 4])(adjustedObject, nullptr);
}

HOOK(0x52C6C0, CallVirtualOnAdjustedThis, "auto");
