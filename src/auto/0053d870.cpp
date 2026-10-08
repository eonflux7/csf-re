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

// Original: int __cdecl sub_506BD0(void) -> returns the global context pointer.
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());

// Original: 0x53D740, __thiscall-style (ecx = this), one stack argument, ret 4.
CSF_FUNC(int, __fastcall, CMenuMissionInfo_EnterPanelState, 0x53D740, (void* self, void* edx, int flag));

// Hook target 0x53D870 (__fastcall-style, one stack argument, ret 4).
// Behaviour:
//   ctx = GetGlobalValue_8B056C();
//   v4  = ctx->field_1C;
//   if (*(v4 + 0x1D0) == 1 && *(v4 + 0x1D4) == 1) {
//       ptr = self->field_10;
//       if (ptr->byte_4) return CMenuMissionInfo_EnterPanelState(self - 4, edi, 1);
//       return ptr;
//   }
//   return v4 + 0x1D0;
// Note: the original forwards edi (a2) unchanged into the callee; this C++
// version cannot pin edi, so the callee receives whatever edi holds at the call.
static int __fastcall CMenuMissionInfo_ConditionalRefreshOnAltEntry(void* self, void* edx, int a3)
{
    (void)edx;
    (void)a3;

    uint8_t* ctx = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8B056C()));
    uint32_t v4 = CSF_AT(uint32_t, ctx, 0x1C);
    uint8_t* v4Ptr = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(v4));

    uint32_t v5 = CSF_AT(uint32_t, v4Ptr, 0x1D0);
    int result = static_cast<int>(v4 + 0x1D0);

    if (v5 == 1 && CSF_AT(uint32_t, v4Ptr, 0x1D4) == 1)
    {
        uint32_t ptrValue = CSF_AT(uint32_t, self, 0x10);
        result = static_cast<int>(ptrValue);
        uint8_t flagByte = CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(ptrValue)), 4);
        if (flagByte)
        {
            uint8_t* selfMinus4 = reinterpret_cast<uint8_t*>(self) - 4;
            return CMenuMissionInfo_EnterPanelState(selfMinus4, nullptr, 1);
        }
    }
    return result;
}

HOOK(0x0053D870, CMenuMissionInfo_ConditionalRefreshOnAltEntry, "auto");
