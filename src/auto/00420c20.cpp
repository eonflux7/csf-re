#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee: __thiscall helper at 0x004DB440 (ret 4), bound as __fastcall per convention.
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Original: mov eax,[ecx+0xc]; add eax,0x20; push eax; call 0x4DB440; ret
static int __fastcall C_00834fa8_LookupAtFieldOffsetPlus32(C_00834fa8* self, void* /*edx*/)
{
    int fieldValue = CSF_AT(int, self, 0x0C);
    return CharTask_LookupByOffset(self, nullptr, fieldValue + 0x20);
}

HOOK(0x420C20, C_00834fa8_LookupAtFieldOffsetPlus32, "auto");
