#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x69E050 (bind as given by the packet).
CSF_FUNC(uint32_t*, __fastcall, sub_69E050, 0x0069E050, (void* self, void* edx, uint32_t* Block, uint32_t Size, int a4, int a5));

// Wrapper at 0x69E200. Original is __thiscall (this in ecx), 5 stack args, ret 20.
// Asm: ecx stays this, edx = [ebp+8] = Block, and the stack gets Block, Size, a5, a6 (top to bottom).
static uint32_t* __fastcall C_0085eac0_ForwardBlockArgs(C_0085eac0* self, void* /*edx*/, uint32_t* Block, uint32_t Size, int a4, int a5, int a6)
{
    return sub_69E050(self, Block, Block, Size, a5, a6);
}

HOOK(0x0069E200, C_0085eac0_ForwardBlockArgs, "auto");
