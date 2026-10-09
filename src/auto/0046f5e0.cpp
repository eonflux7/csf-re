#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, sub_46EDD0, 0x0046EDD0, (int a1, int a2, void* a3));

// Returns a2. If a3 already equals a4, just stores a3 into *a2. Otherwise rebuilds
// the element range starting at sub_46EDD0(a4, field_0008, a3), destroys the old
// range (12-byte elements, vtable slot 0 with arg 0), stores the new begin in
// field_0008, then stores a3 into *a2.
static C_008313f8** __fastcall MissionScript_RebuildRangeAndAssign(S_0046f5e0* self, void* /*edx*/, C_008313f8** a2, C_008313f8* a3, int a4)
{
    if (a3 == reinterpret_cast<C_008313f8*>(a4)) {
        *a2 = a3;
        return a2;
    }

    C_008313f8* newBegin = reinterpret_cast<C_008313f8*>(sub_46EDD0(a4, static_cast<int>(self->field_0008), a3));
    uint32_t oldEnd = self->field_0008;
    uint32_t newBeginValue = reinterpret_cast<uint32_t>(newBegin);

    for (C_008313f8* element = newBegin;
         element != reinterpret_cast<C_008313f8*>(oldEnd);
         element = reinterpret_cast<C_008313f8*>(reinterpret_cast<uint8_t*>(element) + 12)) {
        // element->vDestroy(element, 0): vtable slot 0
        ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, element, 0)[0]))(element, nullptr, 0);
    }

    self->field_0008 = newBeginValue;
    *a2 = a3;
    return a2;
}

HOOK(0x0046F5E0, MissionScript_RebuildRangeAndAssign, "auto");
