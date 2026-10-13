#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Calls the object's slot-0 virtual (deleting destructor) with the "free memory" flag set.
static void C_00861d48_DeleteViaSlot0(void* object) {
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
}

// Releases the owned sub-object at +0x24 and every 16-byte entry in the table at +0x28
// (count at +0x30), then clears the table count. Returns the last count value read.
static unsigned int __fastcall C_00861d48_ReleaseOwnedEntries(C_00861d48* self, void* /*edx*/) {
    void* ownedObject = CSF_AT(void*, self, 0x24);
    uint32_t entryIndex = 0;
    uint32_t entryOffset = 0;

    if (ownedObject) {
        C_00861d48_DeleteViaSlot0(ownedObject);
        CSF_AT(void*, self, 0x24) = nullptr;
    }

    unsigned int result = CSF_AT(uint32_t, self, 0x30);
    uint32_t loopCount = 0;

    if (result != 0) {
        do {
            void* entry = CSF_AT(void*, CSF_AT(uint8_t*, self, 0x28), entryOffset);
            if (entry) {
                C_00861d48_DeleteViaSlot0(entry);
                CSF_AT(void*, CSF_AT(uint8_t*, self, 0x28), entryOffset) = nullptr;
            }
            result = CSF_AT(uint32_t, self, 0x30);
            ++loopCount;
            entryOffset += 16;
        } while (loopCount < result);
        CSF_AT(uint32_t, self, 0x30) = 0;
    } else {
        CSF_AT(uint32_t, self, 0x30) = 0;
    }

    (void)entryIndex;
    return result;
}

HOOK(0x6E6C70, C_00861d48_ReleaseOwnedEntries, "auto");
