#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ReleaseCountedArrayOrFreeBlock, 0x006C9ED0, (int dispatchKind, int dataPointer));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static unsigned int __fastcall C_00861988_RemoveArrayBlock(
    C_00861988* self, void* /*edx*/, int dataPointer)
{
    const std::uint32_t blockCount = self->field_003c;
    if (blockCount == 0)
        return 0;

    auto* blocks = reinterpret_cast<std::uint8_t*>(self->field_0034);
    std::uint32_t index = 0;
    auto* entry = blocks;

    while (CSF_AT(std::uint32_t, entry, 0) !=
           static_cast<std::uint32_t>(dataPointer))
    {
        ++index;
        entry += 12;
        if (index >= blockCount)
            return index;
    }

    const std::uint32_t entryOffset = 12 * index;
    entry = blocks + entryOffset;
    const std::uint32_t dataAddress = CSF_AT(std::uint32_t, entry, 0);
    const std::uint32_t elementSize = CSF_AT(std::uint32_t, entry, 4);
    const std::uint32_t totalBytes =
        CSF_AT(std::uint32_t, entry, 8) * elementSize;

    std::uint32_t offset = 0;
    while (offset < totalBytes)
    {
        void* element = reinterpret_cast<void*>(dataAddress + offset);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, element, 0)[0x24 / 4])(element, nullptr);

        offset += elementSize;
        --self->field_0050;
    }

    void* data = reinterpret_cast<void*>(
        static_cast<std::uint32_t>(dataPointer));
    const int dispatchKind =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, data, 0)[0x10 / 4])(data, nullptr);
    ReleaseCountedArrayOrFreeBlock(dispatchKind, dataPointer);

    const std::uint32_t remainingBlocks = self->field_003c - 1;
    self->field_003c = remainingBlocks;

    if (index != remainingBlocks)
    {
        auto* destination =
            reinterpret_cast<std::uint8_t*>(self->field_0034) + entryOffset;
        return static_cast<unsigned int>(reinterpret_cast<std::uintptr_t>(
            _memcpy(destination, destination + 12,
                    12 * (remainingBlocks - index))));
    }

    return remainingBlocks;
}

HOOK(0x006DFCD0, C_00861988_RemoveArrayBlock, "auto");
