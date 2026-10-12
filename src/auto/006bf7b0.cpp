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

CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0, (int a1, int (__cdecl* a2)(int, int), int a3));

static int* __fastcall C_00860700_GetChildIterationResult(C_00860700* self, void* /*edx*/)
{
    int* foundResult = nullptr;
    RwFrameForAllChildren(static_cast<int>(self->field_000c),
                          reinterpret_cast<int (__cdecl*)(int, int)>(CSF_ADDR_FN(0x006EE5A0)),
                          static_cast<int>(reinterpret_cast<intptr_t>(&foundResult)));
    return foundResult;
}

HOOK(0x006BF7B0, C_00860700_GetChildIterationResult, "auto");
