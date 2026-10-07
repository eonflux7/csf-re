#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00495560 (8 bytes):
//   mov ecx, [ecx + 0x44]
//   jmp 0x483370
// Tail-jump into a shared code block (0x483370 is not a function start), whose body is
//   return *(dword*)(this_ecx + 0x10) == 38;
// so the thunk evaluates the type code (offset 0x10) of the object stored at self+0x44.
static int32_t __fastcall C_00495560_InnerTypeIs38(void* self, void* /*edx*/)
{
    void* inner = CSF_AT(void*, self, 0x44);
    return CSF_AT(uint32_t, inner, 0x10) == 38;
}

HOOK(0x495560, C_00495560_InnerTypeIs38, "auto");
