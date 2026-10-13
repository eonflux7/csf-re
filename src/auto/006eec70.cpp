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

// Forwards this->+8 (as an address) to virtual slot 31 (offset 0x7C) of the object passed in.
// Returns whatever that virtual returns.
static int __fastcall C_00862110_InvokeVirtualWithFieldPointer(C_00862110* self, void* /*edx*/, int a2)
{
    void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    uint32_t* fieldPointer = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x8);
    return (reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*)>(CSF_AT(void**, target, 0)[0x7C / 4]))(target, nullptr, fieldPointer);
}

HOOK(0x006EEC70, C_00862110_InvokeVirtualWithFieldPointer, "auto");
