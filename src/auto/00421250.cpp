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

// Binds to the original task-state helper at 0x4DB440 (__fastcall, one stack arg).
CSF_FUNC(int, __fastcall, sub_4DB440, 0x004DB440, (void* self, void* edx, int a2));

// Returns the value of the field at this+0x0C, offset by 0x3C, passed to the helper.
// Original: mov eax,[ecx+0xc]; add eax,0x3c; push eax; call 0x4db440; ret
static int __fastcall GetTaskStateFromOffsetField(C_00834fa8* self, void* /*edx*/)
{
    int fieldOffset = CSF_AT(int, self, 0x0C);
    return sub_4DB440(self, nullptr, fieldOffset + 60);
}

HOOK(0x00421250, GetTaskStateFromOffsetField, "auto");
