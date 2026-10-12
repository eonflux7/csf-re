#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, returns block pointer or null).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Copies a 4-dword MissionScript entry block into self.
// Layout: [0] data pointer, [1] element count, [2] copied element count, [3] extra value.
// If the source count is nonzero, a fresh buffer is allocated from the game heap and the
// first (copied count) dwords of the source data are copied into it.
static uint32_t* __fastcall DynamicObj_CopyMissionScriptEntry(uint32_t* self, void* /*edx*/, uint32_t* source)
{
    uint32_t* srcEntry = source;
    uint32_t allocCount = CSF_AT(uint32_t, srcEntry, 0x04);

    if (allocCount != 0) {
        void* newBuffer = Mem_Alloc(4 * static_cast<std::size_t>(allocCount));
        CSF_AT(void*, self, 0x00) = newBuffer;

        std::size_t copyBytes = 4 * static_cast<std::size_t>(CSF_AT(uint32_t, srcEntry, 0x08));
        std::memcpy(newBuffer, reinterpret_cast<const void*>(CSF_AT(uintptr_t, srcEntry, 0x00)), copyBytes);
        srcEntry = source;
    } else {
        CSF_AT(uint32_t, self, 0x00) = 0;
    }

    CSF_AT(uint32_t, self, 0x08) = CSF_AT(uint32_t, srcEntry, 0x08);
    CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, srcEntry, 0x04);
    CSF_AT(uint32_t, self, 0x0C) = CSF_AT(uint32_t, srcEntry, 0x0C);
    return self;
}

HOOK(0x004419F0, DynamicObj_CopyMissionScriptEntry, "auto");
