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

// 0x004E1200  base-class initializer for C_0083e500 (ret 8, two stack args)
CSF_FUNC(uint32_t*, __fastcall, C_0083e160_InitBase, 0x4E1200, (void* self, void* edx, int a2, int a3));

// 0x004E1910  C_0083e500 constructor: runs the base init, then installs this class's vtable
// and the base-class vtable in the secondary slot.
static C_0083e500* __fastcall C_0083e500_Construct(C_0083e500* self, void* /*edx*/, int a2, int a3)
{
    C_0083e160_InitBase(self, nullptr, a2, a3);
    *reinterpret_cast<void**>(self) = CSF_VTBL(0x0083E500);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0083E4E4)));
    return self;
}

HOOK(0x4E1910, C_0083e500_Construct, "auto");
