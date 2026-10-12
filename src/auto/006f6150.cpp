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

// Array-of-pointers vtable referenced by the original at 0x6F618F (S_0041e5f0 embedded in C_00862874 at +0x8).
[[maybe_unused]] static const void* const kEmbeddedArrayVtbl = CSF_VTBL(0x0041E5F0);

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, InitEntryObject, 0x006F5E60, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(char, __fastcall, PrepareAppendState, 0x006F6100, (void* self, void* edx));

// __thiscall(C_00862874* this, uint32_t* a2), ret 4
static int __fastcall C_00862874_AllocAndAppendEntry(C_00862874* self, void* /*edx*/, uint32_t* a2)
{
    PrepareAppendState(self, nullptr);

    S_006e48a0* entry = static_cast<S_006e48a0*>(Mem_Alloc(64));
    int resultValue = 0;
    if (entry)
        resultValue = InitEntryObject(entry, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(self)), a2);

    S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(&self->field_0008);
    uint32_t usedCount = self->field_0010;
    uint32_t capacity = self->field_000c;
    if (usedCount >= capacity)
        S_0041e5f0_GrowArrayCapacity(list, nullptr, static_cast<int>(list->field_000c));

    uint32_t index = list->field_0008;
    int* slots = reinterpret_cast<int*>(list->field_0000);
    slots[index] = resultValue;
    list->field_0008 = index + 1;

    return resultValue;
}

HOOK(0x006F6150, C_00862874_AllocAndAppendEntry, "auto");
