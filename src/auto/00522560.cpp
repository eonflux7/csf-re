#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int*, __fastcall, sub_526CF0, 0x00526CF0, (void* self, void* edx, unsigned int* a2));

// Dispatches a notification event (identified by the id at event+4) to the primary
// sound object (field_0048) and to every matching entry of the 24-byte entry list
// at +0x20 (count at +0x28). Matching entries get a notify call, then sub_526CF0
// is invoked on the list owner (field_0040) with a pointer to the entry's +4 slot.
static int __fastcall S_00521df0_DispatchEventToMatchingEntries(S_00521df0* self, void* /*edx*/, int a2)
{
    void* event = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));

    // Primary object: notify when its id at +0x2C matches the event id.
    void* primary = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0048));
    if (self->field_0048 && CSF_AT(uint32_t, primary, 0x2C) == CSF_AT(uint32_t, event, 4))
    {
        ((void(__fastcall*)(void*, void*))(CSF_AT(void**, primary, 0)[11]))(primary, nullptr);
    }

    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x28));
    if (CSF_AT(uint32_t, self, 0x28) > 0)
    {
        uint32_t remaining = CSF_AT(uint32_t, self, 0x28);
        uint32_t entryOffset = 0;
        do
        {
            uint8_t* entries = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x20)));
            uint32_t entryObject = CSF_AT(uint32_t, entries + entryOffset, 4);
            result = static_cast<int>(reinterpret_cast<uintptr_t>(entries + entryOffset));

            if (entryObject != 0 &&
                CSF_AT(uint32_t, entries + entryOffset, 12) == CSF_AT(uint32_t, event, 4))
            {
                void* entryObj = reinterpret_cast<void*>(static_cast<uintptr_t>(entryObject));
                ((void(__fastcall*)(void*, void*))(CSF_AT(void**, entryObj, 0)[11]))(entryObj, nullptr);

                uint8_t* entriesAfter = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x20)));
                unsigned int* elementArg = reinterpret_cast<unsigned int*>(entriesAfter + entryOffset + 4);
                void* listOwner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0040));
                unsigned int* notifyResult = sub_526CF0(listOwner, nullptr, elementArg);
                result = static_cast<int>(reinterpret_cast<uintptr_t>(notifyResult));
            }

            entryOffset += 24;
            --remaining;
        } while (remaining != 0);
    }

    return result;
}

HOOK(0x00522560, S_00521df0_DispatchEventToMatchingEntries, "auto");
