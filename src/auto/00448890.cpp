#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, FindEntryIndex, 0x00448830, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall S_00448830_RemoveInactiveEntries(
    S_00448830* self, void* /*edx*/, int key)
{
    (void)CSF_VTBL(0x00448830);

    int result = FindEntryIndex(self, nullptr, key);
    if (result == -1)
        return result;

    auto* entry = reinterpret_cast<std::uint8_t*>(
        self->field_0018 + 20u * static_cast<std::uint32_t>(result));
    int index = CSF_AT(int, entry, 0x0C) - 1;
    std::uint32_t offset = 36u * static_cast<std::uint32_t>(index);

    while (index >= 0)
    {
        result = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            static_cast<int>(CSF_AT(std::uint32_t, entry, 0x04) + offset));

        bool remove = result == 0;
        if (!remove)
        {
            result = SceneObj_IsInactive(
                reinterpret_cast<void*>(result), nullptr);
            remove = static_cast<std::uint8_t>(result) != 0;
        }

        if (remove)
        {
            result = CSF_AT(int, entry, 0x0C) - 1;
            CSF_AT(int, entry, 0x0C) = result;
            if (index != result)
            {
                auto* destination = reinterpret_cast<std::uint8_t*>(
                    CSF_AT(std::uint32_t, entry, 0x04) + offset);
                result = reinterpret_cast<int>(_memcpy(
                    destination, destination + 36,
                    36u * (static_cast<std::uint32_t>(result) -
                           static_cast<std::uint32_t>(index))));
            }
        }

        --index;
        offset -= 36;
    }
    return result;
}

HOOK(0x00448890, S_00448830_RemoveInactiveEntries, "auto");
