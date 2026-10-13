#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Returns the attachment record owned by this object, creating a zeroed 0x30-byte
// record on first use. Returns nullptr (and clears the stored pointer) if allocation fails.
static uint32_t* __fastcall SceneLoader_EnsureAttachmentRecord(S_006b8020* self, void* /*edx*/)
{
    uint32_t* record = reinterpret_cast<uint32_t*>(self->field_0010);
    if (record != nullptr)
        return record;

    int allocated = CoreMemory_AllocUntagged(0x30);
    record = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(allocated));
    if (record == nullptr) {
        self->field_0010 = 0;
        return nullptr;
    }

    CSF_AT(uint32_t, record, 0x00) = 0;
    CSF_AT(uint32_t, record, 0x04) = 0;
    CSF_AT(uint8_t, record, 0x0C) = 0;
    CSF_AT(uint32_t, record, 0x14) = 0;
    CSF_AT(uint32_t, record, 0x18) = 0;
    CSF_AT(uint32_t, record, 0x1C) = 0;
    CSF_AT(uint32_t, record, 0x20) = 0;
    CSF_AT(uint32_t, record, 0x24) = 0;
    CSF_AT(uint32_t, record, 0x28) = 0;
    CSF_AT(uint32_t, record, 0x2C) = 0;
    CSF_AT(uint32_t, record, 0x08) = 0xFFFFFFFFu;
    CSF_AT(uint8_t, record, 0x0D) = 2;
    CSF_AT(uint32_t, record, 0x10) = 0x20;

    self->field_0010 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(record));
    return record;
}

HOOK(0x006B8020, SceneLoader_EnsureAttachmentRecord, "auto");
