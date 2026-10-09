#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(void, __fastcall, S_0045d610_RemoveEntry, 0x00469120, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_0046f790_IsEntryValid, 0x0046F830, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall S_0045d610_PruneInvalidEntries(
    S_0045d610* self, void* /*edx*/)
{
    const std::uint32_t originalCount = self->field_0020;
    int result = static_cast<int>(originalCount);
    if (!originalCount)
        return result;

    std::uint32_t removedCount = 0;
    bool removed;
    do
    {
        removed = false;
        for (std::uint32_t index = 0; index < originalCount; ++index)
        {
            void* entry =
                reinterpret_cast<void**>(self->field_0018)[index];
            if (entry && !S_0046f790_IsEntryValid(entry, nullptr))
            {
                S_0045d610_RemoveEntry(
                    self, nullptr,
                    reinterpret_cast<int*>(self->field_0018)[index]);
                reinterpret_cast<std::uint32_t*>(self->field_0018)[index] = 0;
                ++removedCount;
                removed = true;
                break;
            }
        }
    } while (removed);

    if (!removedCount)
        return result;

    const std::uint32_t currentCount = self->field_0020;
    if (removedCount == currentCount)
    {
        void* oldData = reinterpret_cast<void*>(self->field_0018);
        CSF_AT(std::uint32_t, self, 0x1C) = 0;
        self->field_0020 = 0;
        result = static_cast<int>(reinterpret_cast<std::uintptr_t>(oldData));
        if (oldData)
        {
            result = Mem_Free(oldData);
            self->field_0018 = 0;
        }
        return result;
    }

    // Leave the full 0x28 bytes available to the original array helper.
    struct TemporaryArray
    {
        std::uint32_t* data;
        std::uint32_t capacity;
        std::uint32_t count;
        std::uint32_t growth;
        std::uint32_t reserved[6];
    };
    TemporaryArray temporary = {nullptr, 0, 0, 10, {}};

    std::uint32_t* data = nullptr;
    std::uint32_t count = 0;
    std::uint32_t capacity = 0;
    const std::uint32_t survivors = currentCount - removedCount;
    if (survivors)
    {
        // The packet also labels this call address as a vtable reference.
        (void)CSF_VTBL(0x0041E5F0);
        GrowButtonArray(&temporary, nullptr, static_cast<int>(survivors));
        count = temporary.count;
        data = temporary.data;
        capacity = temporary.capacity;
    }

    for (std::uint32_t index = 0; index < originalCount; ++index)
    {
        std::uint32_t* source =
            reinterpret_cast<std::uint32_t*>(self->field_0018) + index;
        if (*source)
        {
            if (count >= capacity)
            {
                const std::uint32_t newCapacity = temporary.growth + capacity;
                auto* newData = static_cast<std::uint32_t*>(
                    Mem_Alloc(4 * newCapacity));
                if (count)
                {
                    std::memcpy(newData, data, 4 * count);
                    count = temporary.count;
                    data = temporary.data;
                }
                temporary.capacity = newCapacity;
                if (data)
                    Mem_Free(data);
                capacity = temporary.capacity;
                temporary.data = newData;
                data = newData;
            }
            data[count++] = *source;
            temporary.count = count;
        }
        result = static_cast<int>(originalCount);
    }

    if (count > CSF_AT(std::uint32_t, self, 0x1C))
    {
        void* newData = Mem_Alloc(4 * capacity);
        void* oldData = reinterpret_cast<void*>(self->field_0018);
        CSF_AT(std::uint32_t, self, 0x1C) = temporary.capacity;
        if (oldData)
        {
            Mem_Free(oldData);
            self->field_0018 = 0;
        }
        self->field_0018 =
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(newData));
        result = static_cast<int>(self->field_0018);
    }

    if (count)
    {
        std::memcpy(reinterpret_cast<void*>(self->field_0018), data, 4 * count);
        count = temporary.count;
        data = temporary.data;
    }
    self->field_0020 = count;
    CSF_AT(std::uint32_t, self, 0x24) = temporary.growth;
    if (data)
        return Mem_Free(data);
    return result;
}

HOOK(0x00469370, S_0045d610_PruneInvalidEntries, "auto");
