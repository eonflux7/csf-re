#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static void __fastcall S_004a0490_RemoveAlertIcon(
    S_004a0490* self, void* /*edx*/, std::uint32_t* icon)
{
    const std::uint32_t iconCount = self->field_0018;
    if (iconCount == 0)
        return;

    std::uint32_t iconIndex = 0;
    auto* entry = reinterpret_cast<std::uint32_t**>(self->field_0010);
    while (*entry != icon)
    {
        ++iconIndex;
        ++entry;
        if (iconIndex >= iconCount)
            return;
    }

    if (iconIndex == static_cast<std::uint32_t>(-1))
        return;

    const std::uint32_t secondaryCount = CSF_AT(std::uint32_t, self, 0x28);
    if (secondaryCount != 0)
    {
        std::uint32_t secondaryIndex = 0;
        auto* secondaryEntry = CSF_AT(std::uint32_t**, self, 0x20);
        while (*secondaryEntry != icon)
        {
            ++secondaryIndex;
            ++secondaryEntry;
            if (secondaryIndex >= secondaryCount)
                break;
        }

        if (secondaryIndex < secondaryCount &&
            secondaryIndex != static_cast<std::uint32_t>(-1))
        {
            const std::uint32_t remainingCount =
                CSF_AT(std::uint32_t, self, 0x28) - 1;
            CSF_AT(std::uint32_t, self, 0x28) = remainingCount;
            if (secondaryIndex != remainingCount)
            {
                auto* destination = CSF_AT(std::uint8_t*, self, 0x20)
                    + 4 * secondaryIndex;
                _memcpy(destination, destination + 4,
                        4 * (remainingCount - secondaryIndex));
            }
        }
    }

    void* ownedObject = CSF_AT(void*, icon, 0x0C);
    if (ownedObject)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        CSF_AT(std::uint32_t, icon, 0x0C) = 0;
    }

    if (CSF_AT(std::uint32_t, icon, 0x14) != 0)
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* registry = reinterpret_cast<void*>(
            SceneLoader_GetObjectRegistry(global, nullptr));
        const std::uint32_t objectHandle = CSF_AT(std::uint32_t, icon, 0x14);
        reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
            CSF_AT(void**, registry, 0)[0x18 / 4])(
                registry, nullptr, objectHandle);
    }

    CSF_AT(void**, icon, 0) = CSF_VTBL(0x0082D410);
    Mem_Free(icon);

    const std::uint32_t remainingCount = self->field_0018 - 1;
    self->field_0018 = remainingCount;
    if (iconIndex != remainingCount)
    {
        auto* destination = reinterpret_cast<std::uint8_t*>(self->field_0010)
            + 4 * iconIndex;
        _memcpy(destination, destination + 4,
                4 * (remainingCount - iconIndex));
    }
}

HOOK(0x004A0490, S_004a0490_RemoveAlertIcon, "auto");
