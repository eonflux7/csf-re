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

CSF_FUNC(uint32_t*, __fastcall, sub_4286E0, 0x004286E0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, sub_42CA40, 0x0042CA40, (void* self, void* edx));

static char __fastcall CheatsMenu_ApplyFieldValueAndRefresh(void* self, void* /*edx*/, int a2, int /*a3*/)
{
    sub_4286E0(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10)), nullptr, 0, a2);
    return sub_42CA40(reinterpret_cast<C_00830578*>(reinterpret_cast<uint8_t*>(self) - 4), nullptr);
}

HOOK(0x0042CC00, CheatsMenu_ApplyFieldValueAndRefresh, "auto");
