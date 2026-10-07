#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00506E40: trivial getter returning *(this + 0x1704) (field index 1473).
// __thiscall -> __fastcall with unused edx; callee pops 0 stack bytes (ret).
static int __fastcall C_00506e40_GetField1704(void* self, void* /*edx*/)
{
    return CSF_AT(int32_t, self, 0x1704);
}

HOOK(0x506E40, C_00506e40_GetField1704, "auto");
