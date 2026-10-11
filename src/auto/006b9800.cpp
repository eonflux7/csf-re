#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Ensures the attachment record exists (called when a model index is set).
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_EnsureAttachmentRecord, 0x006B8020, (void* self, void* edx));
// Releases/detaches the attachment record (called when clearing the model index).
CSF_FUNC(void, __fastcall, SceneLoader_ReleaseAttachmentRecord, 0x006B97A0, (void* self, void* edx));

// Original references S_006b8020's vtable via the call at 0x6B980F; kept as a
// non-behavioral binding so the address is present in the file.
[[maybe_unused]] static const void* const kSceneLoaderVtableRef = CSF_VTBL(0x006B8020);

static void __fastcall SceneLoader_SetAttachmentModelIndex(S_006b8020* self, void* /*edx*/, int modelIndex)
{
    if (modelIndex == -1)
    {
        // Clearing: mark the record's index as -1 and release it.
        uint8_t* record = reinterpret_cast<uint8_t*>(self->field_0010);
        if (record)
        {
            CSF_AT(int, record, 0x08) = -1;
            SceneLoader_ReleaseAttachmentRecord(self, nullptr);
        }
    }
    else
    {
        // Setting: make sure the record exists, then store the index into it.
        SceneLoader_EnsureAttachmentRecord(self, nullptr);
        uint8_t* record = reinterpret_cast<uint8_t*>(self->field_0010);
        CSF_AT(int, record, 0x08) = modelIndex;
    }
}

HOOK(0x006B9800, SceneLoader_SetAttachmentModelIndex, "auto");
