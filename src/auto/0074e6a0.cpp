#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Tail jump: 0x74E6A0 pushes ebp, pops it, then jmp 0x753410 (__stdcall, one stack arg, ret 4).
// The stack argument and ecx/edx are forwarded unchanged; the callee pops its own 4-byte argument,
// so this wrapper keeps the original's "no ret immediate" caller contract.
CSF_FUNC(char, __stdcall, NetPlayerDDL_ProcessEmbeddedField, 0x00753410, (int a1));

static char __fastcall NetCogible_ProcessEmbeddedField(NetCogible* self, void* /*edx*/, void* a2)
{
    (void)self;
    return NetPlayerDDL_ProcessEmbeddedField(static_cast<int>(reinterpret_cast<std::uintptr_t>(a2)));
}

HOOK(0x0074E6A0, NetCogible_ProcessEmbeddedField, "auto");
