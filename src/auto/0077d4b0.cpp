#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Inserts a single 72-byte entry at `position` in the array. Calls the real
// insertion routine (not analysed here, marked "todo").
CSF_FUNC(char, __fastcall, GameDataDefs_InsertEntry72At, 0x0077D1C0,
         (void* self, void* edx, int a1, unsigned int a3, int a2));

// Inserts one 72-byte entry at `position` and writes the address of the
// (possibly reallocated) entry into `result`, which is also returned.
static std::uint32_t* __fastcall GameDataDefs_InsertEntry72(
    void* self, void* /*edx*/, std::uint32_t* result,
    int position, int element)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    std::int32_t index = 0;

    if (begin != 0)
    {
        const std::int32_t sizeBytes = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x08) - begin);

        if (sizeBytes / 72 != 0)
        {
            index = static_cast<std::int32_t>(
                static_cast<std::uint32_t>(position) - begin) / 72;
        }
    }

    GameDataDefs_InsertEntry72At(self, nullptr, position, 1u, element);

    *result = CSF_AT(std::uint32_t, self, 0x04)
            + static_cast<std::uint32_t>(index) * 72u;
    return result;
}

HOOK(0x0077D4B0, GameDataDefs_InsertEntry72, "auto");
