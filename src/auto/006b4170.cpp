#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Ensures the object's cached data block exists (original: sub_6B3A90).
CSF_FUNC(int, __fastcall, EnsureAtomicDataInitialized, 0x006B3A90, (void* self, void* edx));

// Copies the 60-byte cached data block at field_0060 into the caller's buffer and returns the buffer.
static void* __fastcall C_00860288_CopyCachedDataBlock(C_00860288* self, void* /*edx*/, void* destination) {
    if (!self->field_0060) {
        EnsureAtomicDataInitialized(self, nullptr);
    }
    // Field is re-read after the possible initialization call.
    std::memcpy(destination, reinterpret_cast<const void*>(self->field_0060), 0x3C);
    return destination;
}

HOOK(0x006B4170, C_00860288_CopyCachedDataBlock, "auto");
