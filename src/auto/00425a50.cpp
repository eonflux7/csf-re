#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0041f0a0_Serialize, 0x0041F0A0, (S_0041f0a0* self, void* edx, S_0041f0a0* stream));

// Refreshes the sync record `record` for the object `self`: re-evaluates the state byte at +0x00
// through the owner at +0x04, runs the serializer, then asks the owner for a final stop decision.
// Returns 1 when the record is marked active, 0 when the owner requested a stop.
static int __fastcall S_0041f0a0_SyncRecordState(S_0041f0a0* self, void* /*edx*/, void* record)
{
    // Pre-check: if the state byte is clear, ask the owner (vtable slot 0x80) whether it is blocked.
    bool newFlag = false;
    if (CSF_AT(uint8_t, record, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, record, 0x04);
        bool ownerBlocked = ((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, owner, 0)[0x80 / 4])(owner, nullptr, 0, record);
        newFlag = !ownerBlocked;
    }

    uint32_t savedField1C = CSF_AT(uint32_t, record, 0x1C);
    CSF_AT(uint8_t, record, 0x00) = newFlag ? 1 : 0;

    S_0041f0a0_Serialize(self, nullptr, static_cast<S_0041f0a0*>(record));

    // The original's vtable-reference for S_0041f0a0 is established by the Serialize call above.
    (void)CSF_VTBL(0x0041F0A0);

    CSF_AT(uint32_t, record, 0x1C) = 0;
    if (CSF_AT(uint8_t, record, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, record, 0x04);
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        void* subobject = reinterpret_cast<uint8_t*>(self) + 0x1C;
        int queryResult = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, owner, 0)[0x48 / 4])(owner, nullptr, subobject, record);
        CSF_AT(uint8_t, record, 0x00) = (queryResult == -1) ? 1 : 0;
    }

    CSF_AT(uint32_t, record, 0x1C) = savedField1C;
    if (CSF_AT(uint8_t, record, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, record, 0x04);
        bool ownerRequestsStop = ((bool(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, owner, 0)[0x7C / 4])(owner, nullptr, record);
        if (ownerRequestsStop)
        {
            CSF_AT(uint8_t, record, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, record, 0x00) = 1;
    return 1;
}

HOOK(0x00425A50, S_0041f0a0_SyncRecordState, "auto");
