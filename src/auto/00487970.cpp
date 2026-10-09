#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Base-class handler (__thiscall, 1 stack arg -> ret 4)
CSF_FUNC(int, __fastcall, CCharActionFsm_RunBaseHandler, 0x00480C40, (void* self, void* edx, int a2));
// Looks up a slot index by id (__thiscall, 1 stack arg -> ret 4); returns -1 when absent
CSF_FUNC(int, __fastcall, CCharActionFsm_FindSlotIndex, 0x00485A20, (void* self, void* edx, int a2));

// Forwards an event to the base handler, the active component (field_009c),
// and the child objects bound to slots 22 and 28.
static int __fastcall CCharActionFsm_ForwardEventToSlots(CCharActionFsm* self, void* /*edx*/, int a2)
{
    CCharActionFsm_RunBaseHandler(self, nullptr, a2);

    // Active component: vtable slot 0xB4 (index 45) with the event argument.
    void* activeComponent = reinterpret_cast<void*>(self->field_009c);
    if (activeComponent) {
        void** activeVtbl = CSF_AT(void**, activeComponent, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(activeVtbl[180 / 4])(activeComponent, nullptr, a2);
    }

    // Slot 22 child.
    int slotIndex = CCharActionFsm_FindSlotIndex(self, nullptr, 22);
    if (slotIndex != -1) {
        uint32_t* slots = reinterpret_cast<uint32_t*>(self->field_0078);
        void* child = reinterpret_cast<void*>(slots[slotIndex]);
        if (child) {
            void** childVtbl = CSF_AT(void**, child, 0);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(childVtbl[180 / 4])(child, nullptr, a2);
        }
    }

    // Slot 28 child: only forwarded when its flag at +0x80 is set.
    int result = CCharActionFsm_FindSlotIndex(self, nullptr, 28);
    if (result != -1) {
        uint32_t slotBase = self->field_0078;
        void* target = reinterpret_cast<void*>(reinterpret_cast<uint32_t*>(slotBase)[result]);
        result = static_cast<int>(slotBase + 4 * static_cast<uint32_t>(result));
        if (CSF_AT(uint32_t, target, 0x80)) {
            result = static_cast<int>(reinterpret_cast<uintptr_t>(target));
            if (target) {
                void** targetVtbl = CSF_AT(void**, target, 0);
                return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(targetVtbl[180 / 4])(target, nullptr, a2);
            }
        }
    }
    return result;
}

HOOK(0x487970, CCharActionFsm_ForwardEventToSlots, "auto");
