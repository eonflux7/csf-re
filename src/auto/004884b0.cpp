#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharAction_ActivateSlotEntry, 0x00479C60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharAction_FindSlotIndex, 0x00485A20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharAction_GetActiveHandle, 0x00487260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));

static C_00833908* SlotNotFoundResult() {
    return reinterpret_cast<C_00833908*>(static_cast<uintptr_t>(-1));
}

static C_00833908* __fastcall CCharActionFsm_ResolveActionTarget(CCharActionFsm* self, void* /*edx*/) {
    uint8_t* weaponFsm = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008));
    C_00833908* result = reinterpret_cast<C_00833908*>(static_cast<uintptr_t>(self->field_0008));

    if (CSF_AT(uint8_t, weaponFsm, 0x228) == 0) {
        return result;
    }

    if ((CSF_AT(uint8_t, weaponFsm, 0x1F0) & 2) != 0) {
        int slotIndex = CharAction_FindSlotIndex(self, nullptr, 22);
        if (slotIndex == -1) {
            return SlotNotFoundResult();
        }
        uintptr_t slotAddress = static_cast<uintptr_t>(self->field_0078)
                              + 4u * static_cast<uintptr_t>(static_cast<uint32_t>(slotIndex));
        uint32_t entry = *reinterpret_cast<uint32_t*>(slotAddress);
        if (entry != 0) {
            int activated = CharAction_ActivateSlotEntry(
                reinterpret_cast<void*>(static_cast<uintptr_t>(entry)), nullptr);
            return reinterpret_cast<C_00833908*>(static_cast<uintptr_t>(activated));
        }
        return reinterpret_cast<C_00833908*>(slotAddress);
    }

    int slotIndex = CharAction_FindSlotIndex(self, nullptr, 28);
    if (slotIndex == -1) {
        return SlotNotFoundResult();
    }
    uintptr_t slotAddress = static_cast<uintptr_t>(self->field_0078)
                          + 4u * static_cast<uintptr_t>(static_cast<uint32_t>(slotIndex));
    uint32_t entryValue = *reinterpret_cast<uint32_t*>(slotAddress);
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryValue));
    result = reinterpret_cast<C_00833908*>(slotAddress);

    uint32_t entryFlag = CSF_AT(uint32_t, entry, 0x80);
    if (entryFlag != 0 && entry != nullptr) {
        int hasField398 = C_004950e0_GetField398(
            reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr);
        result = reinterpret_cast<C_00833908*>(static_cast<uintptr_t>(hasField398));
        if (result != nullptr) {
            int activeHandle = CharAction_GetActiveHandle(self, nullptr);
            int activated = CharAction_ActivateSlotEntry(
                reinterpret_cast<void*>(static_cast<uintptr_t>(activeHandle)), nullptr);
            return reinterpret_cast<C_00833908*>(static_cast<uintptr_t>(activated));
        }
    }
    return result;
}

HOOK(0x4884B0, CCharActionFsm_ResolveActionTarget, "auto");
