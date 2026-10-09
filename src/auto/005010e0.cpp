#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, sub_4525E0, 0x004525E0, (void* self, void* edx, int a2));

// Copies a range of 0x58-byte string records backwards (from rangeEnd down to rangeBegin)
// into the destination block that ends at destEnd. Each record: the string is reassigned
// from the source, then the trailing fields are copied and a sub-object is copied via sub_4525E0.
// Returns the final destination cursor (the destination pointer after the last step).
static void* __cdecl SceneLoader_CopyRecordsBackward(void* rangeBegin, void* rangeEnd, void* destEnd, int /*unused*/)
{
    std::uint8_t* srcCur = static_cast<std::uint8_t*>(rangeEnd);
    std::uint8_t* dstCur = static_cast<std::uint8_t*>(destEnd);

    if (rangeBegin != rangeEnd)
    {
        do
        {
            srcCur -= 0x58;
            dstCur -= 0x58;

            AssignStringSubstring(dstCur, nullptr, srcCur, 0, 0xFFFFFFFFu);

            // The original copies the two leading flag bytes (record offsets 0x1C and 0x1D)
            // as a single 16-bit move; copy them together at 0x1C.
            CSF_AT(std::uint16_t, dstCur, 0x1C) = CSF_AT(std::uint16_t, srcCur, 0x1C);
            CSF_AT(std::uint32_t, dstCur, 0x20) = CSF_AT(std::uint32_t, srcCur, 0x20);
            CSF_AT(std::uint32_t, dstCur, 0x28) = CSF_AT(std::uint32_t, srcCur, 0x28);
            CSF_AT(std::uint32_t, dstCur, 0x2C) = CSF_AT(std::uint32_t, srcCur, 0x2C);
            CSF_AT(std::uint32_t, dstCur, 0x34) = CSF_AT(std::uint32_t, srcCur, 0x34);
            CSF_AT(std::uint32_t, dstCur, 0x38) = CSF_AT(std::uint32_t, srcCur, 0x38);
            CSF_AT(std::uint32_t, dstCur, 0x40) = CSF_AT(std::uint32_t, srcCur, 0x40);
            CSF_AT(std::uint32_t, dstCur, 0x44) = CSF_AT(std::uint32_t, srcCur, 0x44);

            sub_4525E0(dstCur + 0x48, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(srcCur + 0x48)));
        }
        while (srcCur != rangeBegin);
    }

    return dstCur;
}

HOOK(0x005010E0, SceneLoader_CopyRecordsBackward, "auto");
