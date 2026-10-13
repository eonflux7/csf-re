#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

CSF_GLOBAL(uint8_t, g_LoadStateTrailingFlag, 0x008BAF89);
CSF_GLOBAL(uint8_t, g_LoadStateLevelByte, 0x008BAF88);

// Resets the 256-entry table at +0x118 (entries of 8 bytes: dword 2, byte 0),
// allocates the 20 KB working buffer, zeroes the state fields, and sets two globals.
static void** __fastcall LoadState_Construct(S_00694cd0* self, void* /*edx*/) {
    constexpr int kEntryCount = 256;
    constexpr std::size_t kEntryStride = 8;
    constexpr std::size_t kTableOffset = 0x118;

    for (int index = 0; index < kEntryCount; ++index) {
        std::size_t entryOffset = kTableOffset + static_cast<std::size_t>(index) * kEntryStride;
        CSF_AT(uint32_t, self, entryOffset) = 2;
        CSF_AT(uint8_t, self, entryOffset + 4) = 0;
    }

    self->field_0000 = static_cast<uint32_t>(CoreMemory_AllocUntagged(0x5000));
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    CSF_AT(uint8_t, self, 0x14) = 0;

    g_LoadStateTrailingFlag = 0;
    g_LoadStateLevelByte = 32;

    return reinterpret_cast<void**>(self);
}

HOOK(0x00694C80, LoadState_Construct, "auto");
