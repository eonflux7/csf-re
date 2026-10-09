#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Releases the three owned sub-objects at this+0x84, +0x88, +0x8C.
// Each non-null object gets its vtable slot 0 called with flag 1 (deleting-destructor style),
// then its slot is cleared. Returns the eax value of the last release call.
static int __fastcall CharActionFsm_ReleaseThreeOwnedObjects(void* self, void* /*edx*/) {
    int result = 0;

    void* first = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x84));
    if (first) {
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, first, 0)[0])(first, nullptr, 1);
        CSF_AT(uint32_t, self, 0x84) = 0;
    }

    void* second = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x88));
    if (second) {
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, second, 0)[0])(second, nullptr, 1);
        CSF_AT(uint32_t, self, 0x88) = 0;
    }

    void* third = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8C));
    if (third) {
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, third, 0)[0])(third, nullptr, 1);
        CSF_AT(uint32_t, self, 0x8C) = 0;
    }

    return result;
}

HOOK(0x482A00, CharActionFsm_ReleaseThreeOwnedObjects, "auto");
