#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Inserts a single 20-byte rule record at `position` (calls the real insertion
// routine at 0x559A70; not analysed here - marked "todo").
CSF_FUNC(int, __fastcall, InsertRuleRecord20At, 0x00559A70,
         (void* self, void* edx, char* a1, unsigned int a3, int* arg8));

// Inserts a 20-byte mp_rules record at `position` and writes the address of the
// slot that now corresponds to that position (after the insertion, the array may
// have moved) into `result`, which is also returned. The index is computed
// from the position relative to the array start, before the insertion.
static int* __fastcall InsertRuleRecord20(
    S_00559d90* self, void* /*edx*/, int* result,
    char* position, int* value)
{
    const uint32_t begin = self->field_0004;
    std::int32_t index = 0;

    if (begin != 0)
    {
        const std::int32_t sizeBytes = static_cast<std::int32_t>(self->field_0008 - begin);

        if (sizeBytes / 20 != 0)
        {
            const std::uintptr_t positionAddress =
                reinterpret_cast<std::uintptr_t>(position);
            index = static_cast<std::int32_t>(positionAddress - begin) / 20;
        }
    }

    InsertRuleRecord20At(self, nullptr, position, 1u, value);

    *result = static_cast<int>(self->field_0004
            + static_cast<std::uint32_t>(index) * 20u);
    return result;
}

HOOK(0x00559D90, InsertRuleRecord20, "auto");
