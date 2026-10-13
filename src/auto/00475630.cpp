#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

CSF_GLOBAL(void**, g_SceneEntryItems, 0x00899744);
CSF_GLOBAL(std::uint32_t, g_SceneEntryCount, 0x0089974C);

static std::uint32_t __fastcall EventDispatcher_Refresh(void* self, void* /*edx*/)
{
    CSF_AT(std::uint8_t, self, 0x24) = 1;

    for (std::uint32_t index = 0;
         index < CSF_AT(std::uint32_t, self, 0x30); ++index)
    {
        void* entry = CSF_AT(void**, self, 0x28)[index];
        ((void(__fastcall*)(void*, void*, void*))
             CSF_AT(void**, entry, 0)[1])(
            entry, nullptr, static_cast<std::uint8_t*>(self) + 4);
    }

    for (std::uint32_t index = 0; index < g_SceneEntryCount; ++index)
    {
        void* entry = g_SceneEntryItems[index];
        ((void(__fastcall*)(void*, void*, void*))
             CSF_AT(void**, entry, 0)[1])(
            entry, nullptr, static_cast<std::uint8_t*>(self) + 4);
    }

    std::uint32_t result = CSF_AT(std::uint32_t, self, 0x0C);
    for (std::uint32_t index = 0; index < result; ++index)
    {
        void* entry = CSF_AT(void**, self, 0x04)[index];
        if (entry)
        {
            ((void(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, entry, 0)[0])(entry, nullptr, 1);
            CSF_AT(void**, self, 0x04)[index] = nullptr;
        }
        result = CSF_AT(std::uint32_t, self, 0x0C);
    }
    CSF_AT(std::uint32_t, self, 0x0C) = 0;

    if (CSF_AT(std::uint32_t, self, 0x1C) >
        CSF_AT(std::uint32_t, self, 0x08))
    {
        void** replacement = reinterpret_cast<void**>(
            CoreMemory_AllocUntagged(
                4 * CSF_AT(std::uint32_t, self, 0x18)));

        CSF_AT(std::uint32_t, self, 0x08) =
            CSF_AT(std::uint32_t, self, 0x18);

        result = CSF_AT(std::uint32_t, self, 0x04);
        if (result)
        {
            result = CoreMemory_Free(CSF_AT(void*, self, 0x04));
            CSF_AT(void*, self, 0x04) = nullptr;
        }
        CSF_AT(void**, self, 0x04) = replacement;
    }

    const std::uint32_t pendingCount =
        CSF_AT(std::uint32_t, self, 0x1C);
    if (pendingCount)
    {
        std::memcpy(CSF_AT(void*, self, 0x04),
                    CSF_AT(void*, self, 0x14),
                    4 * pendingCount);
    }

    CSF_AT(std::uint32_t, self, 0x0C) =
        CSF_AT(std::uint32_t, self, 0x1C);
    CSF_AT(std::uint32_t, self, 0x10) =
        CSF_AT(std::uint32_t, self, 0x20);
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    CSF_AT(std::uint8_t, self, 0x24) = 0;
    return result;
}

HOOK(0x00475630, EventDispatcher_Refresh, "auto");
