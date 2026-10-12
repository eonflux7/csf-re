#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bound by address. Both callees take the subject in ecx and ignore edx.
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4E0B60, 0x004E0B60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetFirstLinkedObject_4E0BD0, 0x004E0BD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetSecondLinkedObject_4E0BE0, 0x004E0BE0, (void* self, void* edx));

// Returns 1 when either flag output is set (a linked object is missing or has bit 0 of its flag byte set).
static int __stdcall ComputeLinkedPairBlockFlags(int subject, bool* leftBlocked, bool* rightBlocked)
{
    *leftBlocked = false;
    *rightBlocked = false;

    if (subject != 0) {
        int firstLinkedRaw = GetFirstLinkedObject_4E0BD0(
            reinterpret_cast<void*>(static_cast<uintptr_t>(subject)), nullptr);
        int firstLinked = GetLinkedObjectValue_4E0B60(
            reinterpret_cast<void*>(static_cast<uintptr_t>(firstLinkedRaw)), nullptr);

        int secondLinkedRaw = GetSecondLinkedObject_4E0BE0(
            reinterpret_cast<void*>(static_cast<uintptr_t>(subject)), nullptr);
        int secondLinked = GetLinkedObjectValue_4E0B60(
            reinterpret_cast<void*>(static_cast<uintptr_t>(secondLinkedRaw)), nullptr);

        bool leftFlag = (firstLinked == 0) ||
            ((CSF_AT(uint8_t, reinterpret_cast<void*>(static_cast<uintptr_t>(firstLinked)), 4) & 1) != 0);
        *leftBlocked = leftFlag;

        bool rightFlag = (secondLinked == 0) ||
            ((CSF_AT(uint8_t, reinterpret_cast<void*>(static_cast<uintptr_t>(secondLinked)), 4) & 1) != 0);
        *rightBlocked = rightFlag;
    }

    return (*leftBlocked || *rightBlocked) ? 1 : 0;
}

HOOK(0x004DF140, ComputeLinkedPairBlockFlags, "auto");
