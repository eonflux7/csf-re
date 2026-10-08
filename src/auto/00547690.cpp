#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static const auto MenuExtras_InsertRecords =
    reinterpret_cast<int (__fastcall*)(
        void*, void*, char*, unsigned int, std::uint32_t*)>(CSF_ADDR_FN(0x005472C0));

static std::uint32_t* __fastcall MenuExtras_InsertRecordAndGetIterator(
    void* self, void* /*edx*/, std::uint32_t* result,
    char* position, std::uint32_t* record)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    std::int32_t index = 0;

    if (begin != 0) {
        const std::int32_t byteCount = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x08) - begin);

        if (byteCount / 68 != 0) {
            const std::uint32_t positionAddress =
                reinterpret_cast<std::uint32_t>(position);
            index = static_cast<std::int32_t>(positionAddress - begin) / 68;
        }
    }

    MenuExtras_InsertRecords(self, nullptr, position, 1u, record);

    *result = CSF_AT(std::uint32_t, self, 0x04)
            + static_cast<std::uint32_t>(index) * 68u;
    return result;
}

HOOK(0x00547690, MenuExtras_InsertRecordAndGetIterator, "auto");
