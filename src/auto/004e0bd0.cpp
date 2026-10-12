#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x004E0830: __fastcall, one stack argument, ret 4.
CSF_FUNC(int, __fastcall, DispatchToSubobjectHandler, 0x004E0830, (void* self, void* edx, int a2));

// Original: push ecx; mov ecx, [ecx+8]; call 0x4E0830; ret
// Pushes `this` as the stack argument and passes the pointer stored at +8 as `this`.
static int __fastcall ForwardToFieldHandler(void* self, void* /*edx*/)
{
    void* fieldObject = CSF_AT(void*, self, 0x8);
    int thisAsArg = static_cast<int>(reinterpret_cast<uintptr_t>(self));
    return DispatchToSubobjectHandler(fieldObject, nullptr, thisAsArg);
}

HOOK(0x004E0BD0, ForwardToFieldHandler, "auto");
