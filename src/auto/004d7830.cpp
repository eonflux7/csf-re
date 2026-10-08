#include <cstdint>
#include "globals.h"
#include "hook.h"

// char* __thiscall sub_4E12B0(char* this)  ->  ret 0 (no stack args popped)
CSF_FUNC(char*, __fastcall, C_0083e500_GetEmbeddedDataPointer, 0x4E12B0, (char* self, void* edx));

// int __thiscall sub_4D7830(char* this)
// Thunk that rewinds `this` by one pointer (secondary base -> primary object),
// then forwards the call to 0x4E12B0 (original does a tail jmp).
static int __fastcall ForwardToObjectAtMinus4(char* self, void* /*edx*/)
{
    return static_cast<int>(reinterpret_cast<uintptr_t>(
        C_0083e500_GetEmbeddedDataPointer(self - 4, nullptr)));
}

HOOK(0x4D7830, ForwardToObjectAtMinus4, "auto");
