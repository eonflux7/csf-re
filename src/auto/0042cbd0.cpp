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

CSF_FUNC(uint32_t*, __fastcall, sub_428680, 0x00428680, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, sub_42CA40, 0x0042CA40, (void* self, void* edx));

static char __fastcall CheatsMenu_ClearFieldAndRefresh(S_0042cbd0* self, void* /*edx*/, int a2, int /*a3*/)
{
    sub_428680(reinterpret_cast<void*>(self->field_0010), nullptr, 0, a2);
    return sub_42CA40(reinterpret_cast<C_00830578*>(reinterpret_cast<uint8_t*>(self) - 4), nullptr);
}

HOOK(0x0042CBD0, CheatsMenu_ClearFieldAndRefresh, "auto");
