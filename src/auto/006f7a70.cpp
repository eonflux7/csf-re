#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RpHAnimHierarchyDestroy, 0x0056CD30, (uint32_t* a1));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);

// The packet labels the direct array-growth call address as a vtable reference.
[[maybe_unused]] static const auto kArrayGrowthReference = CSF_VTBL(0x0041E5F0);

static char __fastcall GfxMaterials_ReleaseTrackHandle(
    uint32_t* self, void* /*edx*/, int** handle)
{
    const int trackIndex = **handle;
    if (trackIndex < 0 || trackIndex >= CSF_AT(int, self, 0x08))
    {
        RpHAnimHierarchyDestroy(reinterpret_cast<uint32_t*>((*handle)[2]));
        RpHAnimHierarchyDestroy(reinterpret_cast<uint32_t*>((*handle)[3]));

        char result = 0;
        if (*handle)
        {
            result = static_cast<char>(CoreMemory_Free(*handle));
            *handle = nullptr;
        }
        *handle = nullptr;
        return result;
    }

    auto* track = CSF_AT(uint8_t*, reinterpret_cast<void*>(self[0]),
                        4 * trackIndex);
    auto* groups = reinterpret_cast<S_0041e5f0*>(track + 0x0C);

    uint32_t groupIndex = 0;
    if (CSF_AT(uint32_t, track, 0x14) != 0)
    {
        do
        {
            auto* group = CSF_AT(
                S_0041e5f0*, reinterpret_cast<void*>(groups->field_0000),
                4 * groupIndex);

            if (group->field_0010 == static_cast<uint32_t>((*handle)[1]))
            {
                auto* currentTrack = CSF_AT(
                    uint8_t*, reinterpret_cast<void*>(self[0]),
                    4 * **handle);

                const double retentionLimit =
                    static_cast<double>(CSF_AT(int, currentTrack, 0x1C)) *
                    static_cast<double>(g_flt_0082DC84);

                if (retentionLimit <=
                    static_cast<double>(
                        static_cast<uint32_t>(group->field_0008)))
                {
                    RpHAnimHierarchyDestroy(
                        reinterpret_cast<uint32_t*>((*handle)[2]));
                    RpHAnimHierarchyDestroy(
                        reinterpret_cast<uint32_t*>((*handle)[3]));
                    if (*handle)
                    {
                        CoreMemory_Free(*handle);
                        *handle = nullptr;
                    }
                }
                else
                {
                    if (static_cast<uint32_t>(group->field_0008) >=
                        static_cast<uint32_t>(group->field_0004))
                    {
                        S_0041e5f0_GrowArrayCapacity(
                            group, nullptr, group->field_000c);
                    }

                    const uint32_t count = group->field_0008;
                    auto* slot = reinterpret_cast<int**>(
                        group->field_0000 + 4 * count);
                    group->field_0008 = count + 1;
                    *slot = *handle;
                }

                *handle = nullptr;
                return 0;
            }

            ++groupIndex;
        }
        while (groupIndex < static_cast<uint32_t>(groups->field_0008));
    }

    auto* allocation = reinterpret_cast<S_0041e5f0*>(
        CoreMemory_AllocUntagged(20));
    S_0041e5f0* newGroup = nullptr;
    if (allocation)
    {
        allocation->field_0000 = 0;
        allocation->field_0004 = 0;
        allocation->field_0008 = 0;
        allocation->field_000c = 10;
        newGroup = allocation;
    }

    const bool hasCapacity =
        static_cast<uint32_t>(newGroup->field_0008) <
        static_cast<uint32_t>(newGroup->field_0004);
    newGroup->field_0010 = (*handle)[1];
    if (!hasCapacity)
    {
        S_0041e5f0_GrowArrayCapacity(
            newGroup, nullptr, newGroup->field_000c);
    }

    const uint32_t handleCount = newGroup->field_0008;
    auto* handleSlot = reinterpret_cast<int**>(
        newGroup->field_0000 + 4 * handleCount);
    newGroup->field_0008 = handleCount + 1;
    *handleSlot = *handle;

    if (static_cast<uint32_t>(groups->field_0008) >=
        static_cast<uint32_t>(groups->field_0004))
    {
        S_0041e5f0_GrowArrayCapacity(
            groups, nullptr, groups->field_000c);
    }

    const uint32_t groupCount = groups->field_0008;
    auto* groupSlot = reinterpret_cast<S_0041e5f0**>(
        groups->field_0000 + 4 * groupCount);
    groups->field_0008 = groupCount + 1;
    *groupSlot = newGroup;

    *handle = nullptr;
    return static_cast<char>(groupCount + 1);
}

HOOK(0x006F7A70, GfxMaterials_ReleaseTrackHandle, "auto");
