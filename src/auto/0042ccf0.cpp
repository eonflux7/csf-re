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

CSF_FUNC(uint32_t*, __fastcall, sub_428730, 0x00428730, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_42CA40, 0x0042CA40, (void* self, void* edx));

// Original: char __thiscall sub_42CCF0(this, a2), ret 4.
// Passes the value stored at request+8 together with the subobject at this+0x10 to
// sub_428730, then forwards to sub_42CA40 on the parent object (this-4) and returns its result.
static char __fastcall CheatMenu_ApplyRequestAndRefresh(void* self, void* /*edx*/, int a2)
{
    uint8_t* menuBase = reinterpret_cast<uint8_t*>(self);
    uint8_t* request = reinterpret_cast<uint8_t*>(a2);

    uint32_t requestValue = CSF_AT(uint32_t, request, 0x08);
    void* subobject = reinterpret_cast<void*>(CSF_AT(uint32_t, menuBase, 0x10));

    sub_428730(subobject, nullptr, static_cast<int>(requestValue));

    void* parentMenu = menuBase - 4;
    return sub_42CA40(parentMenu, nullptr);
}

HOOK(0x0042CCF0, CheatMenu_ApplyRequestAndRefresh, "auto");
