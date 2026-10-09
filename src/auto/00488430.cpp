#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_479D30, 0x00479D30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_485A20, 0x00485A20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_487260, 0x00487260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));

static C_00833908* SlotNotFoundResult() {
    return reinterpret_cast<C_00833908*>(static_cast<intptr_t>(-1));
}

static C_00833908* __fastcall CCharActionFsm_ResolveActionTarget(CCharActionFsm* self, void* /*edx*/) {
    uint8_t* weaponFsm = reinterpret_cast<uint8_t*>(self->field_0008);

    if (CSF_AT(uint8_t, weaponFsm, 0x228) == 0) {
        return reinterpret_cast<C_00833908*>(self->field_0008);
    }

    if ((CSF_AT(uint8_t, weaponFsm, 0x1F0) & 2) != 0) {
        int slotIndex = sub_485A20(self, nullptr, 22);
        if (slotIndex == -1) {
            return SlotNotFoundResult();
        }
        uint32_t slotAddress = self->field_0078 + 4u * static_cast<uint32_t>(slotIndex);
        uint32_t entry = *reinterpret_cast<uint32_t*>(slotAddress);
        if (entry != 0) {
            return reinterpret_cast<C_00833908*>(
                sub_479D30(reinterpret_cast<void*>(entry), nullptr));
        }
        return reinterpret_cast<C_00833908*>(slotAddress);
    }

    int slotIndex = sub_485A20(self, nullptr, 28);
    if (slotIndex == -1) {
        return SlotNotFoundResult();
    }
    uint32_t slotAddress = self->field_0078 + 4u * static_cast<uint32_t>(slotIndex);
    uint8_t* entry = reinterpret_cast<uint8_t*>(*reinterpret_cast<uint32_t*>(slotAddress));
    if (CSF_AT(uint32_t, entry, 0x80) != 0 && entry != nullptr) {
        int hasField398 = C_004950e0_GetField398(reinterpret_cast<void*>(self->field_0008), nullptr);
        if (hasField398 == 0) {
            return nullptr;
        }
        int targetValue = sub_487260(self, nullptr);
        return reinterpret_cast<C_00833908*>(sub_479D30(reinterpret_cast<void*>(targetValue), nullptr));
    }
    return reinterpret_cast<C_00833908*>(slotAddress);
}

HOOK(0x488430, CCharActionFsm_ResolveActionTarget, "auto");
