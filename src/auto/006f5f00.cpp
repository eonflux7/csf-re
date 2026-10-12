#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_006e48a0_RefreshProjectionData, 0x006F5CD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_006e48a0_RegisterInstance, 0x007012E0, (S_006e48a0* self, void* , int instance));
CSF_FUNC(int, __fastcall, S_007013b0_RemoveCachedEntry, 0x007013B0, (S_007013b0* self, void* , void* entry));
CSF_FUNC(C_00863ac0*, __fastcall, GfxObjectInstances_UpdateProjection, 0x00701850, (void* self, void* , int objectAddress, float* origin, float radius, float length, int flags, S_007013b0* instances, float scale));
CSF_FUNC(bool, __fastcall, GfxObjectInstances_CheckProjectionChange, 0x00701DC0, (S_00701d40* self, void* , int* a2));
CSF_FUNC(int, __fastcall, ParticleEmitter_ReleaseOwnedInstances, 0x00701DE0, (S_00701d40* self, void* , S_007013b0* a2));

CSF_GLOBAL(float, g_LevelScaleFactor, 0x0082E048);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

static int __fastcall S_006e48a0_UpdateProjectionInstances(
    S_006e48a0* self, void* /*edx*/)
{
    // The packet also identifies the cache-removal address as a vtable.
    (void)CSF_VTBL(0x007013B0);

    if (self->field_003c)
        S_006e48a0_RefreshProjectionData(self, nullptr);

    auto* owner = reinterpret_cast<std::uint8_t*>(self->field_0004);
    std::uint32_t instanceIndex;
    // This declared float field actually stores an integer cache handle.
    std::memcpy(&instanceIndex, &self->field_0008, sizeof(instanceIndex));

    auto* instances = reinterpret_cast<S_007013b0*>(owner + 0x18);
    auto* cache = owner + 0x58;
    bool projectionChanged = false;

    if (instanceIndex == static_cast<std::uint32_t>(-1))
    {
        instanceIndex = static_cast<std::uint32_t>(
            S_006e48a0_RegisterInstance(
                reinterpret_cast<S_006e48a0*>(cache), nullptr,
                reinterpret_cast<int>(self)));
        projectionChanged = true;
    }

    S_00701d40* projection = nullptr;
    if (instanceIndex != static_cast<std::uint32_t>(-1))
    {
        auto* entry = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, cache, 0x08) + (instanceIndex << 4));
        if (CSF_AT(S_006e48a0*, entry, 0x00) == self)
        {
            const std::uint32_t stamp = CSF_AT(std::uint32_t, cache, 0x34);
            projection = CSF_AT(S_00701d40*, entry, 0x0C);
            CSF_AT(std::uint32_t, entry, 0x08) = stamp;
            if (CSF_AT(int, entry, 0x04) != -1)
                S_007013b0_RemoveCachedEntry(
                    reinterpret_cast<S_007013b0*>(cache), nullptr, entry);
        }
    }

    if (projection)
    {
        const bool changed = GfxObjectInstances_CheckProjectionChange(
            projection, nullptr, reinterpret_cast<int*>(instances));
        projectionChanged = changed | projectionChanged;
    }
    else
    {
        instanceIndex = static_cast<std::uint32_t>(
            S_006e48a0_RegisterInstance(
                reinterpret_cast<S_006e48a0*>(cache), nullptr,
                reinterpret_cast<int>(self)));
        projection = nullptr;

        if (instanceIndex != static_cast<std::uint32_t>(-1))
        {
            auto* entry = reinterpret_cast<void*>(
                CSF_AT(std::uint32_t, cache, 0x08) + (instanceIndex << 4));
            if (CSF_AT(S_006e48a0*, entry, 0x00) == self)
            {
                const std::uint32_t stamp = CSF_AT(std::uint32_t, cache, 0x34);
                projection = CSF_AT(S_00701d40*, entry, 0x0C);
                CSF_AT(std::uint32_t, entry, 0x08) = stamp;
                if (CSF_AT(int, entry, 0x04) != -1)
                    S_007013b0_RemoveCachedEntry(
                        reinterpret_cast<S_007013b0*>(cache), nullptr, entry);
            }
        }
        projectionChanged = true;
    }

    if (!projection)
    {
        const std::uint32_t invalidIndex = static_cast<std::uint32_t>(-1);
        std::memcpy(&self->field_0008, &invalidIndex, sizeof(invalidIndex));
        return -1;
    }

    if (projectionChanged || static_cast<std::uint8_t>(self->field_0034))
    {
        float* origin =
            reinterpret_cast<float* (__fastcall*)(void*, void*)>(
                reinterpret_cast<void**>(self->field_0000)[0x24 / 4])(
                    self, nullptr);

        const double radius =
            reinterpret_cast<double (__fastcall*)(void*, void*)>(
                reinterpret_cast<void**>(self->field_0000)[0x08 / 4])(
                    self, nullptr);

        const float scaledRadius = static_cast<float>(radius * g_LevelScaleFactor);
        const int flags = static_cast<int>(self->field_0038);
        float length;
        std::memcpy(&length, &self->field_0010, sizeof(length));

        GfxObjectInstances_UpdateProjection(
            projection, nullptr, static_cast<int>(self->field_0030),
            origin, scaledRadius, length, flags, instances, 1.0f);
    }

    auto* renderState = reinterpret_cast<int*>(self->field_002c);
    const int renderStateValue = renderState ? *renderState : 0;
    reinterpret_cast<void (__cdecl*)(int, int)>(
        reinterpret_cast<void**>(g_pRwEngineInstance)[0x20 / 4])(
            1, renderStateValue);

    const int result = ParticleEmitter_ReleaseOwnedInstances(
        projection, nullptr, instances);
    std::memcpy(&self->field_0008, &instanceIndex, sizeof(instanceIndex));
    return result;
}

HOOK(0x006F5F00, S_006e48a0_UpdateProjectionInstances, "auto");
