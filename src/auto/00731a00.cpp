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

// Callees of the original, bound by address.
CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0, (void* self, void* edx, int effect));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(uint8_t*, __cdecl, CreateTypedObject, 0x0073B930, (int a1, int a2));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* edx, int additionalCapacity));

// Button effect dispatcher: registers a new effect entry in the button's effect list.
// Original: __thiscall, one stack argument, ret 4.
static int __fastcall CGuiButton_AddFxEntry(CGuiButton* self, void* /*edx*/, int a2)
{
    int result = CanConfigureEffect(self, nullptr, a2);
    if (result != 0)
        return result;

    SetAllocationMode(3);
    uint8_t* typedObject = CreateTypedObject(a2, static_cast<int>(self->field_0004));

    uint32_t count = self->field_0010;
    S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(&self->field_0008);
    if (count >= list->field_0004)
    {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(list, nullptr, static_cast<int>(list->field_000c));
    }

    uint32_t index = list->field_0008;
    uint32_t* slot = reinterpret_cast<uint32_t*>(list->field_0000 + 4 * index);
    list->field_0008 = index + 1;
    *slot = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(typedObject));

    return SetAllocationMode(0);
}

HOOK(0x731A00, CGuiButton_AddFxEntry, "auto");
