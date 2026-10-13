#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original tail-jumps to 0x69DFA0 with the caller's args unchanged.
// Forwarding the same self/Block keeps the stack pop (4 bytes for Block) in the callee.
CSF_FUNC(void, __fastcall, ForwardBlockHandler, 0x0069DFA0, (void* self, void* edx, void* Block));

static void __fastcall C_0085eac0_ForwardBlock(C_0085eac0* self, void* /*edx*/, void* Block)
{
    ForwardBlockHandler(self, nullptr, Block);
}

HOOK(0x0069E040, C_0085eac0_ForwardBlock, "auto");
