#include <cstdint>

#include "globals.h"
#include "hook.h"

// Stores the low byte of the value on this+0x70, then forwards the full value
// to the object at this+0x6c through its vtable slot 0x34 (slot 13).
static int __fastcall GlobalValue_PushControlValue(void* self, void* /*edx*/, int a2)
{
    CSF_AT(uint8_t, self, 0x70) = static_cast<uint8_t>(a2);

    void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x6c)));
    using TargetSetFn = int(__fastcall*)(void*, void*, int);
    TargetSetFn setter = reinterpret_cast<TargetSetFn>(CSF_AT(void**, target, 0)[0x34 / 4]);
    return setter(target, nullptr, a2);
}

HOOK(0x004FBFE0, GlobalValue_PushControlValue, "auto");
