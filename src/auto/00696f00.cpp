#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignDwordRange_696BD0, 0x00696BD0, (void* self, void* edx, uint32_t* a2, unsigned int a3, unsigned int a4));

static S_004f21d0* __fastcall S_004f21d0_AssignDwordValues(
    S_004f21d0* self, void* /*edx*/, uint32_t* values)
{
    AssignDwordRange_696BD0(self, nullptr, values, 0, 0xFFFFFFFF);
    return self;
}

HOOK(0x00696F00, S_004f21d0_AssignDwordValues, "auto");
