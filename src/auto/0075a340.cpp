#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, sub_75A1D0, 0x0075A1D0, (void* self, void* edx, int a2, uint32_t* a3));

// Forwards to the sub-object at this+4 with a zero first argument and the caller's pointer as the second.
static int __fastcall ForwardToInnerWithZeroFlag(void* self, void* /*edx*/, uint32_t* a2)
{
    return sub_75A1D0((uint8_t*)self + 4, nullptr, 0, a2);
}

HOOK(0x0075A340, ForwardToInnerWithZeroFlag, "auto");
