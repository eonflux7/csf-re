#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_7784C0, 0x007784C0, (void* self, void* edx, void* a1, unsigned int a3, void* arg8));

static std::uint32_t* __fastcall GameDataDefs_InsertRecordAndGetIterator(
    S_00778920* self, void* /*edx*/, std::uint32_t* result,
    char* position, C_0082d410* record)
{
    const std::uint32_t begin = self->field_0004;
    std::int32_t index = 0;

    if (begin != 0) {
        const std::int32_t byteCount = static_cast<std::int32_t>(
            self->field_0008 - begin);

        if (byteCount / 44 != 0) {
            const std::uintptr_t positionAddress =
                reinterpret_cast<std::uintptr_t>(position);
            index = static_cast<std::int32_t>(positionAddress - begin) / 44;
        }
    }

    sub_7784C0(self, nullptr, position, 1u, record);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 44u;
    return result;
}

HOOK(0x00778920, GameDataDefs_InsertRecordAndGetIterator, "auto");
