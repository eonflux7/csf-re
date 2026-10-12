#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static std::uint8_t* __fastcall C_00833708_RemoveEntry(
    C_00833708* self, void* /*edx*/, void* entryIndex)
{
    const auto index = reinterpret_cast<std::uintptr_t>(entryIndex);
    const std::uint32_t offset = 96 * index;
    void* object = CSF_AT(void*, reinterpret_cast<void*>(self->field_000c), offset);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])(
            object, nullptr, 1);
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_000c), offset) = 0;
    }

    const std::uint32_t remainingCount = self->field_0014 - 1;
    self->field_0014 = remainingCount;
    if (index != remainingCount)
    {
        auto* destination = reinterpret_cast<std::uint8_t*>(self->field_000c) + offset;
        return static_cast<std::uint8_t*>(
            _memcpy(destination, destination + 96, 96 * (remainingCount - index)));
    }
    return reinterpret_cast<std::uint8_t*>(remainingCount);
}

HOOK(0x00489CF0, C_00833708_RemoveEntry, "auto");
