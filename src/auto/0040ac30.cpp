#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Copies a source array of 16-byte entries into a freshly allocated block.
// Source layout: [0] data pointer, [1] entry count to allocate,
// [2] byte-count factor used for the copy, [3] trailing value.
// Each newly allocated 16-byte entry is zeroed, with its last float set to -1.0f.
static void* __fastcall Block16Array_AssignCopy(void* self, void* /*edx*/, void* source)
{
    const uint32_t allocCount = CSF_AT(uint32_t, source, 4);
    uint8_t* blockData = nullptr;

    if (allocCount != 0)
    {
        uint8_t* allocated = static_cast<uint8_t*>(Mem_Alloc(static_cast<std::size_t>(allocCount) << 4));
        if (allocated != nullptr)
        {
            uint8_t* entry = allocated;
            for (uint32_t i = 0; i < allocCount; ++i, entry += 16)
            {
                CSF_AT(uint32_t, entry, 0) = 0;
                CSF_AT(uint32_t, entry, 4) = 0;
                CSF_AT(uint32_t, entry, 8) = 0;
                CSF_AT(float, entry, 12) = -1.0f;
            }
            blockData = allocated;
        }

        CSF_AT(uint8_t*, self, 0) = blockData;

        const uint32_t copyBytes = CSF_AT(uint32_t, source, 8) << 4;
        const uint8_t* src = CSF_AT(uint8_t*, source, 0);
        uint8_t* dst = blockData;
        for (uint32_t i = 0; i < copyBytes; ++i)
        {
            dst[i] = src[i];
        }
    }
    else
    {
        CSF_AT(uint8_t*, self, 0) = nullptr;
    }

    CSF_AT(uint32_t, self, 8) = CSF_AT(uint32_t, source, 8);
    CSF_AT(uint32_t, self, 4) = CSF_AT(uint32_t, source, 4);
    CSF_AT(uint32_t, self, 12) = CSF_AT(uint32_t, source, 12);
    return self;
}

HOOK(0x0040AC30, Block16Array_AssignCopy, "auto");
