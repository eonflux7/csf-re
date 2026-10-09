#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_502460: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(int, __fastcall, SceneLoader_InsertSectorRecords, 0x00502460,
         (S_00503500* self, void* edx, void* firstArg, unsigned int count, int extraArg));

static std::uint32_t* __fastcall S_00503500_InsertAndGetIterator(
    S_00503500* self, void* /*edx*/, std::uint32_t* result,
    void* position, int extraArg)
{
    const std::uint32_t begin = self->field_0004;
    std::int32_t index = 0;

    if (begin != 0) {
        const std::int32_t byteCount = static_cast<std::int32_t>(
            self->field_0008 - begin);

        if (byteCount / 88 != 0) {
            const std::uint32_t positionAddress =
                reinterpret_cast<std::uint32_t>(position);
            index = static_cast<std::int32_t>(positionAddress - begin) / 88;
        }
    }

    SceneLoader_InsertSectorRecords(self, nullptr, position, 1u, extraArg);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 88u;
    return result;
}

HOOK(0x00503500, S_00503500_InsertAndGetIterator, "auto");
