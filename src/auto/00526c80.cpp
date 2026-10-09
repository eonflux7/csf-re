#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall C_008411a0_TailForwardArguments(
    C_008411a0*, void*, int, int)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax+0x68]
    }
}

static int __fastcall C_008411a0_StoreArgsAndForwardToChild(
    C_008411a0* self, void* /*edx*/, int firstArgument, int secondArgument)
{
    self->field_0058 = static_cast<uint32_t>(firstArgument);
    self->field_005c = static_cast<uint32_t>(secondArgument);

    auto* child = reinterpret_cast<C_008411a0*>(self->field_0060);
    if (child) {
        // The original leaves the second argument in EDX when forwarding.
        [[clang::musttail]] return C_008411a0_TailForwardArguments(
            child,
            reinterpret_cast<void*>(static_cast<uintptr_t>(
                static_cast<uint32_t>(secondArgument))),
            firstArgument,
            secondArgument);
    }

    return firstArgument;
}

HOOK(0x526C80, C_008411a0_StoreArgsAndForwardToChild, "auto");
