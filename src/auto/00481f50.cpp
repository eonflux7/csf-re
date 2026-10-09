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

// Callee binds (names chosen here; signatures fixed by the original).
CSF_FUNC(uint32_t*, __cdecl, CharWeaponFsm_MakeSlotKey, 0x00475A40, (int a1, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, CharWeaponFsm_LookupSlotTable, 0x00481C90, (void* self, void* edx, uint32_t* a3));

// Dispatches to the slot object selected by vSlotC0 when one is valid; otherwise
// builds a lookup key, queries the embedded slot table (at +0x14) and, when the
// current selection is unset (field_0024 == -1), picks the last slot index
// through the virtual vSlotF8 wrapper.
static unsigned int __fastcall CCharWeaponFsm_DispatchOrResolveSlot(CCharWeaponFsm* self, void* /*edx*/, int requestedSlot, int /*unusedArg*/)
{
    int selectedIndex = self->vSlotC0(requestedSlot);
    if (selectedIndex != -1)
    {
        uint32_t* slotEntry = reinterpret_cast<uint32_t*>(self->field_0018 + 4u * static_cast<uint32_t>(selectedIndex));
        void* slotObject = reinterpret_cast<void*>(*slotEntry);
        void** slotVtable = CSF_AT(void**, slotObject, 0);
        return ((unsigned int(__fastcall*)(void*, void*, int))slotVtable[24])(slotObject, nullptr, 0);
    }

    uint32_t lookupKey = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(
        CharWeaponFsm_MakeSlotKey(2, requestedSlot, static_cast<int>(self->field_0008))));
    void* slotTable = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x14);
    unsigned int lookupResult = CharWeaponFsm_LookupSlotTable(slotTable, nullptr, &lookupKey);

    if (self->field_0024 == 0xFFFFFFFFu)
    {
        uint32_t slotBegin = self->field_0018;
        if (slotBegin != 0)
        {
            uint32_t slotEnd = CSF_AT(uint32_t, self, 0x1C);
            int32_t slotSpan = static_cast<int32_t>(slotEnd - slotBegin);
            int lastIndex = (slotSpan >> 2) - 1;
            return static_cast<unsigned int>(self->vSlotF8(lastIndex));
        }
        return static_cast<unsigned int>(self->vSlotF8(-1));
    }

    return lookupResult;
}

HOOK(0x00481F50, CCharWeaponFsm_DispatchOrResolveSlot, "auto");
