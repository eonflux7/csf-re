#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (by original address)
CSF_FUNC(void, __fastcall, AdvanceLinkedRecord, 0x004E3710, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));

// Returns the record that the owner scene object points to at +0x18 (record = root - 184), or null.
static C_00858180* GetLinkedRecordFromOwner(C_0083e330* self) {
    uint32_t ownerRootPointer = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_000c), 0x18);
    if (ownerRootPointer == 0) {
        return nullptr;
    }
    return reinterpret_cast<C_00858180*>(ownerRootPointer - 184);
}

static char __fastcall C_0083e330_UpdateSceneBinding(C_0083e330* self, void* /*edx*/, float a2) {
    if (self->field_004c != 0) {
        C_00858180* record = GetLinkedRecordFromOwner(self);
        self->field_004c = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(record));
        if (record == nullptr) {
            return 0;
        }

        AdvanceLinkedRecord(self, nullptr, a2);

        // The record may have been changed by the callee, so reload the field.
        uint8_t* recordSubObject = reinterpret_cast<uint8_t*>(self->field_004c) + 0xB8;
        bool recordReady = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, recordSubObject, 0)[2])(recordSubObject, nullptr);
        if (!recordReady) {
            return 0;
        }

        if (static_cast<uint8_t>(SceneObj_IsInactive(reinterpret_cast<void*>(self->field_000c), nullptr)) == 0) {
            return 0;
        }

        if (self->field_0050 != 0) {
            return static_cast<char>(self->field_0050);
        }

        uint8_t activationResultLow = static_cast<uint8_t>(self->vSlot9C());
        self->field_0050 = 1;
        return static_cast<char>(activationResultLow);
    }

    C_00858180* record = GetLinkedRecordFromOwner(self);
    self->field_004c = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(record));
    if (record != nullptr) {
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(record), 0xD4) =
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));
    }
    return static_cast<char>(reinterpret_cast<uintptr_t>(record));
}

HOOK(0x004E1520, C_0083e330_UpdateSceneBinding, "auto");
