#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall S_00441ca0_RemoveMatchingObject(
    S_00441ca0* self, void* /*edx*/, int listIndex, int objectId, char flagged)
{
    const std::uint32_t listOffset = 16u * static_cast<std::uint32_t>(listIndex);
    auto* list = reinterpret_cast<std::uint8_t*>(self->field_0004) + listOffset;

    if (CSF_AT(std::uint32_t, list, 0x8) == 0)
        return 0;

    GetValue8B0554();

    list = reinterpret_cast<std::uint8_t*>(self->field_0004) + listOffset;
    if (CSF_AT(std::uint32_t, list, 0x8) == 0)
        return 0;

    std::uint32_t index = 0;
    std::uint32_t objectAddress;
    for (;;)
    {
        auto* elements = CSF_AT(std::uint8_t*, list, 0x0);
        objectAddress = CSF_AT(std::uint32_t, elements, 4 * index);
        void* object = reinterpret_cast<void*>(objectAddress);

        if ((objectId == -1 ||
             CSF_AT(int, CSF_AT(void*, object, 0x2C), 0x0) == objectId) &&
            ((CSF_AT(std::uint32_t, object, 0x170) & 0x400) == 1024) == flagged)
        {
            break;
        }

        ++index;
        if (index >= CSF_AT(std::uint32_t, list, 0x8))
            return 0;
    }

    const std::uint32_t newCount = CSF_AT(std::uint32_t, list, 0x8) - 1;
    CSF_AT(std::uint32_t, list, 0x8) = newCount;

    if (index != newCount)
    {
        auto* destination = CSF_AT(std::uint8_t*, list, 0x0) + 4 * index;
        _memcpy(destination, destination + 4, 4 * (newCount - index));
    }

    return static_cast<int>(objectAddress);
}

HOOK(0x00441D80, S_00441ca0_RemoveMatchingObject, "auto");
