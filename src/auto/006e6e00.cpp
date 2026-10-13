#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static void Array_CopyForwardWords(void* destination, const void* source,
                                   std::uint32_t byteCount)
{
    auto* output = static_cast<std::uint8_t*>(destination);
    const auto* input = static_cast<const std::uint8_t*>(source);
    for (std::uint32_t remaining = byteCount / 4; remaining; --remaining) {
        std::uint32_t word;
        std::memcpy(&word, input, sizeof(word));
        std::memcpy(output, &word, sizeof(word));
        input += 4;
        output += 4;
    }
}

static int __fastcall Array_InsertRecord(void* self, void* /*edx*/,
                                        int index, const void* record)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x08);
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x04);
    const std::uint32_t position = static_cast<std::uint32_t>(index);
    const std::uint32_t byteOffset = 40 * position;

    if (count < capacity) {
        auto* data = CSF_AT(std::uint8_t*, self, 0x00);
        _memcpy(data + byteOffset + 40, data + byteOffset,
                40 * (count - position));
        Array_CopyForwardWords(
            CSF_AT(std::uint8_t*, self, 0x00) + byteOffset, record, 0x28);

        const std::uint32_t result = CSF_AT(std::uint32_t, self, 0x08) + 1;
        CSF_AT(std::uint32_t, self, 0x08) = result;
        return static_cast<int>(result);
    }

    const std::uint32_t newCapacity =
        capacity + CSF_AT(std::uint32_t, self, 0x0C);
    auto* replacement = reinterpret_cast<std::uint8_t*>(
        CoreMemory_AllocUntagged(40 * newCapacity));

    if (replacement && static_cast<std::int32_t>(newCapacity - 1) >= 0) {
        auto* entry = replacement;
        for (std::uint32_t remaining = newCapacity; remaining; --remaining) {
            CSF_AT(std::uint32_t, entry, 0x00) = 0;
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            CSF_AT(std::uint32_t, entry, 0x08) = 0;
            CSF_AT(std::uint32_t, entry, 0x0C) = 0;
            CSF_AT(std::uint32_t, entry, 0x10) = 0;
            CSF_AT(std::uint32_t, entry, 0x14) = 0;
            CSF_AT(std::uint32_t, entry, 0x18) = 0;
            CSF_AT(std::int32_t, entry, 0x1C) = -1;
            CSF_AT(std::int32_t, entry, 0x20) = -1;
            CSF_AT(float, entry, 0x24) = -1.0f;
            entry += 40;
        }
    }

    Array_CopyForwardWords(
        replacement, CSF_AT(std::uint8_t*, self, 0x00), byteOffset);
    Array_CopyForwardWords(replacement + byteOffset, record, 0x28);
    Array_CopyForwardWords(
        replacement + byteOffset + 40,
        CSF_AT(std::uint8_t*, self, 0x00) + byteOffset,
        40 * (CSF_AT(std::uint32_t, self, 0x08) - position));

    auto* oldData = CSF_AT(std::uint8_t*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) =
        CSF_AT(std::uint32_t, self, 0x04) +
        CSF_AT(std::uint32_t, self, 0x0C);
    if (oldData) {
        CoreMemory_Free(oldData);
        CSF_AT(std::uint8_t*, self, 0x00) = nullptr;
    }

    const std::uint32_t result = CSF_AT(std::uint32_t, self, 0x08) + 1;
    CSF_AT(std::uint8_t*, self, 0x00) = replacement;
    CSF_AT(std::uint32_t, self, 0x08) = result;
    return static_cast<int>(result);
}

HOOK(0x006E6E00, Array_InsertRecord, "auto");
