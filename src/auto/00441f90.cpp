#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall DynamicObj_ClearObjectGroups(void* self, void* /*edx*/)
{
    std::uint32_t groupIndex = 0;
    std::uint32_t groupOffset = 0;

    for (;;)
    {
        auto* groups = CSF_AT(std::uint8_t*, self, 0x04);
        if (!groups)
            break;

        const auto byteCount = static_cast<std::int32_t>(
            reinterpret_cast<std::uintptr_t>(CSF_AT(void*, self, 0x08)) -
            reinterpret_cast<std::uintptr_t>(groups));
        if (groupIndex >= static_cast<std::uint32_t>(byteCount >> 4))
            break;

        std::uint32_t objectIndex = 0;
        if (CSF_AT(std::uint32_t, groups + groupOffset, 0x08))
        {
            do
            {
                auto* objects = CSF_AT(
                    std::uint8_t*, CSF_AT(std::uint8_t*, self, 0x04) + groupOffset, 0);
                if (CSF_AT(void*, objects + 4 * objectIndex, 0))
                {
                    void* object = CSF_AT(void*, objects + 4 * objectIndex, 0);
                    if (object)
                    {
                        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
                    }

                    objects = CSF_AT(
                        std::uint8_t*, CSF_AT(std::uint8_t*, self, 0x04) + groupOffset, 0);
                    CSF_AT(void*, objects + 4 * objectIndex, 0) = nullptr;
                }

                ++objectIndex;
            }
            while (objectIndex < CSF_AT(
                std::uint32_t, CSF_AT(std::uint8_t*, self, 0x04) + groupOffset, 0x08));
        }

        ++groupIndex;
        groupOffset += 0x10;
    }

    auto* group = CSF_AT(std::uint8_t*, self, 0x04);
    if (group)
    {
        auto* end = CSF_AT(std::uint8_t*, self, 0x08);
        for (; group != end; group += 0x10)
        {
            if (CSF_AT(void*, group, 0))
            {
                Mem_Free(CSF_AT(void*, group, 0));
                CSF_AT(void*, group, 0) = nullptr;
            }
        }

        if (CSF_AT(void*, self, 0x04))
            Mem_FreeDebug(CSF_AT(void*, self, 0x04));
    }
    CSF_AT(void*, self, 0x04) = nullptr;
    CSF_AT(void*, self, 0x08) = nullptr;
    CSF_AT(void*, self, 0x0C) = nullptr;

    group = CSF_AT(std::uint8_t*, self, 0x14);
    if (group)
    {
        auto* end = CSF_AT(std::uint8_t*, self, 0x18);
        for (; group != end; group += 0x10)
        {
            if (CSF_AT(void*, group, 0))
            {
                Mem_Free(CSF_AT(void*, group, 0));
                CSF_AT(void*, group, 0) = nullptr;
            }
        }

        if (CSF_AT(void*, self, 0x14))
            Mem_FreeDebug(CSF_AT(void*, self, 0x14));
    }
    CSF_AT(void*, self, 0x14) = nullptr;
    CSF_AT(void*, self, 0x18) = nullptr;
    CSF_AT(void*, self, 0x1C) = nullptr;

    // The original repeats these resets without another reachable free.
    CSF_AT(void*, self, 0x14) = nullptr;
    CSF_AT(void*, self, 0x18) = nullptr;
    CSF_AT(void*, self, 0x1C) = nullptr;

    group = CSF_AT(std::uint8_t*, self, 0x04);
    if (group)
    {
        auto* end = CSF_AT(std::uint8_t*, self, 0x08);
        for (; group != end; group += 0x10)
        {
            if (CSF_AT(void*, group, 0))
            {
                Mem_Free(CSF_AT(void*, group, 0));
                CSF_AT(void*, group, 0) = nullptr;
            }
        }

        if (CSF_AT(void*, self, 0x04))
            Mem_FreeDebug(CSF_AT(void*, self, 0x04));
    }
    CSF_AT(void*, self, 0x04) = nullptr;
    CSF_AT(void*, self, 0x08) = nullptr;
    CSF_AT(void*, self, 0x0C) = nullptr;

    return 0;
}

HOOK(0x00441F90, DynamicObj_ClearObjectGroups, "auto");
