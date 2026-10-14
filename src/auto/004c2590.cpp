#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, CoverAttack_ProcessRecord, 0x004C2380, (void* self, void* edx, int a2));

// Processes a cover-attack record: refreshes its state flag through the linked object, runs
// CoverAttack_ProcessRecord, serializes the embedded string when the record is still active,
// then reports whether the record is done (1) or still pending (0).
static int __fastcall CoverAttackState_SerializeRecord(S_004c2380* self, void* /*edx*/, int a2)
{
    // Code-address annotation: function value, not a vtable.
    [[maybe_unused]] const void* coverAttackVtbl = reinterpret_cast<void**>(CSF_ADDR_FN(0x004C2380));

    void* record = reinterpret_cast<void*>(a2);

    // Saved link pointer at +0x1C, restored after the serialization step.
    int savedLinkTarget = CSF_AT(int, record, 0x1C);

    // Flag at +0: set only when it was clear and the linked object's slot-32 check fails.
    bool keepState = false;
    if (CSF_AT(uint8_t, record, 0x00) == 0)
    {
        void* linkedObj = CSF_AT(void*, record, 0x04);
        bool linkedObjHasFlag =
            ((bool(__fastcall*)(void*, void*, int, int))(CSF_AT(void**, linkedObj, 0)[32]))(
                linkedObj, nullptr, 0, a2);
        keepState = !linkedObjHasFlag;
    }
    CSF_AT(uint8_t, record, 0x00) = keepState ? 1 : 0;

    CoverAttack_ProcessRecord(self, nullptr, a2);

    CSF_AT(int, record, 0x1C) = 0;
    if (CSF_AT(uint8_t, record, 0x00) == 0)
    {
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x14),
            reinterpret_cast<S_00691860*>(a2));
    }

    CSF_AT(int, record, 0x1C) = savedLinkTarget;

    if (CSF_AT(uint8_t, record, 0x00) != 0)
    {
        CSF_AT(uint8_t, record, 0x00) = 1;
        return 1;
    }

    void* linkedObj = CSF_AT(void*, record, 0x04);
    bool linkedObjIsActive =
        ((bool(__fastcall*)(void*, void*, int))(CSF_AT(void**, linkedObj, 0)[31]))(
            linkedObj, nullptr, a2);
    if (!linkedObjIsActive)
    {
        CSF_AT(uint8_t, record, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, record, 0x00) = 0;
    return 0;
}

HOOK(0x4C2590, CoverAttackState_SerializeRecord, "auto");
