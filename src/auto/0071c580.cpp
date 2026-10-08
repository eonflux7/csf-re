#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(std::uint32_t*, __cdecl, CopyEightByteElements, 0x0071A410,
    (std::uint32_t* destination, std::uint32_t count,
     std::uint32_t* source, void* context));

static CSF_FUNC(std::uint32_t, __fastcall, InsertQueuedObjectChange, 0x0071B9C0,
    (void* self, void* edx, char* position, std::uint32_t count,
     std::uint32_t* source));

static CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
    (void* destination, const void* source, std::size_t size));

static std::uint32_t __fastcall C_00864508_QueueObjectChange(
    C_00864508* self, void* /*edx*/, void* object, char changeFlag)
{
    const std::uint32_t objectAddress =
        reinterpret_cast<std::uint32_t>(object);

    const std::uint32_t objectBegin = self->field_0048;
    const std::uint32_t objectCount = objectBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_004c - objectBegin) >> 2)
        : 0;

    std::uint32_t objectIndex = 0;
    if (objectCount != 0)
    {
        std::uint32_t cursor = self->field_0048;
        while (CSF_AT(std::uint32_t, reinterpret_cast<void*>(cursor), 0)
               != objectAddress)
        {
            ++objectIndex;
            cursor += 4;
            if (objectIndex >= objectCount)
                break;
        }

        if (objectIndex != objectCount)
        {
            const std::uint32_t pendingBegin = self->field_0034;
            const std::uint32_t pendingCount = pendingBegin
                ? static_cast<std::uint32_t>(
                      static_cast<std::int32_t>(
                          CSF_AT(std::uint32_t, self, 0x38) - pendingBegin) >> 3)
                : 0;

            std::uint32_t pendingIndex = 0;
            if (pendingCount != 0)
            {
                std::uint32_t pendingCursor = self->field_0034;
                while (CSF_AT(std::uint32_t,
                             reinterpret_cast<void*>(pendingCursor), 0)
                       != objectIndex)
                {
                    ++pendingIndex;
                    pendingCursor += 8;
                    if (pendingIndex >= pendingCount)
                        break;
                }
            }

            if (pendingIndex != pendingCount)
                return pendingCount;

            void* pendingContainer =
                reinterpret_cast<std::uint8_t*>(self) + 0x30;

            // The original initializes only the index and flag, not the padding.
            std::uint32_t entry[2];
            entry[0] = objectIndex;
            CSF_AT(char, entry, 4) = changeFlag;

            const std::uint32_t begin = self->field_0034;
            const std::uint32_t size = begin
                ? static_cast<std::uint32_t>(
                      static_cast<std::int32_t>(
                          CSF_AT(std::uint32_t, self, 0x38) - begin) >> 3)
                : 0;

            if (begin != 0 &&
                size < static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(
                        CSF_AT(std::uint32_t, self, 0x3C) - begin) >> 3))
            {
                const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x38);
                CopyEightByteElements(
                    reinterpret_cast<std::uint32_t*>(end),
                    1, entry, pendingContainer);
                CSF_AT(std::uint32_t, self, 0x38) = end + 8;
            }
            else
            {
                InsertQueuedObjectChange(
                    pendingContainer, nullptr,
                    reinterpret_cast<char*>(CSF_AT(std::uint32_t, self, 0x38)),
                    1, entry);
            }

            void* relatedObject =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x20 / 4])(object, nullptr);

            std::uint32_t result =
                reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x24 / 4])(object, nullptr);

            if (static_cast<std::uint8_t>(result) != 0)
            {
                result =
                    reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, relatedObject, 0)[0x194 / 4])(
                            relatedObject, nullptr);
                if (result != 1)
                {
                    result =
                        reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*)>(
                            CSF_AT(void**, relatedObject, 0)[0x1A4 / 4])(
                                relatedObject, nullptr);
                }
            }
            return result;
        }
    }

    const std::uint32_t indexBegin = self->field_0024;
    const std::uint32_t indexCount = indexBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0028 - indexBegin) >> 2)
        : 0;

    if (indexCount == 0)
        return 0;

    std::uint32_t indexCursor = self->field_0024;
    const std::uint32_t recordBase = self->field_0014;
    std::uint32_t indexPosition = 0;
    for (;;)
    {
        const std::uint32_t recordIndex =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(indexCursor), 0);
        const std::uint32_t recordAddress = recordBase + (recordIndex << 6);
        if (CSF_AT(std::uint32_t, reinterpret_cast<void*>(recordAddress), 0x38)
            == objectAddress)
            break;

        ++indexPosition;
        indexCursor += 4;
        if (indexPosition >= indexCount)
            return indexCount;
    }

    const std::uint32_t begin = self->field_0024;
    const std::uint32_t end = self->field_0028;
    const std::uint32_t destination = begin + indexPosition * 4;
    const std::uint32_t source = destination + 4;
    const std::uint32_t bytes =
        static_cast<std::uint32_t>(
            static_cast<std::int32_t>(end - source) >> 2) << 2;

    _memcpy(reinterpret_cast<void*>(destination),
               reinterpret_cast<const void*>(source), bytes);

    const std::uint32_t result = self->field_0028 - 4;
    self->field_0028 = result;
    return result;
}

HOOK(0x0071C580, C_00864508_QueueObjectChange, "auto");
