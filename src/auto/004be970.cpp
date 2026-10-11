#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

// Vtable reference recorded from the call sites at 0x4BE9C1 / 0x4BE9E6 (not a behavioral store).
[[maybe_unused]] static const void* const s_ListVtableRef = CSF_VTBL(0x0041E5F0);

// Resolves the per-object base used by the AI core: linked value minus 4, or 0 when unlinked.
static uintptr_t AiCore_ResolveLinkedBase(int linkedObject)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(linkedObject)), nullptr);
    if (linkedValue)
        return static_cast<uintptr_t>(linkedValue - 4);
    return 0;
}

// Appends the marker values 5 and 9 to the list embedded at base+0x50C, growing it when full.
static uint32_t __stdcall AiCore_AppendMarkerPairToLinkedList(int linkedObject)
{
    uintptr_t base = AiCore_ResolveLinkedBase(linkedObject);
    // this is the linked object itself (`mov ecx, esi`); the resolved base + 0x440 is the argument
    UnnamedApply_4DC180(reinterpret_cast<void*>(static_cast<uintptr_t>(linkedObject)), nullptr,
                        static_cast<int>(base + 0x440));

    base = AiCore_ResolveLinkedBase(linkedObject);
    S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(base + 0x50C);

    if (list->field_0008 >= list->field_0004)
        S_0041e5f0_GrowArrayCapacity(list, nullptr, static_cast<int>(list->field_000c));

    uint32_t slotIndex = list->field_0008;
    uint32_t* slot = reinterpret_cast<uint32_t*>(list->field_0000 + 4 * slotIndex);
    list->field_0008 = slotIndex + 1;
    *slot = 5;

    if (list->field_0008 >= list->field_0004)
        S_0041e5f0_GrowArrayCapacity(list, nullptr, static_cast<int>(list->field_000c));

    uint32_t secondIndex = list->field_0008;
    uint32_t* secondSlot = reinterpret_cast<uint32_t*>(list->field_0000 + 4 * secondIndex);
    uint32_t result = secondIndex + 1;
    list->field_0008 = result;
    *secondSlot = 9;
    return result;
}

HOOK(0x004BE970, AiCore_AppendMarkerPairToLinkedList, "auto");
