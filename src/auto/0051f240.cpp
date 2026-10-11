#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(uint32_t*, __fastcall, FxParticleManager_ExchangeUpdateEntries, 0x0051EEC0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, FxParticleManager_GrowUpdateEntries, 0x0051EFD0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, FxParticleManager_CompareSortKeys, 0x0051EAE0, (const void* a, const void* b));
CSF_FUNC(unsigned int, __cdecl, RtRandom, 0x005867D0, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(void, __cdecl, _qsort, 0x007AEAE0, (void* base, std::size_t count, std::size_t size, int (__cdecl* compare)(const void*, const void*)));

CSF_GLOBAL(float, g_CoverAttackFarDistanceSquared, 0x0082D4C0);
CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint32_t* __fastcall C_00840f6c_PrepareParticleUpdate(
    C_00840f6c* self, void* /*edx*/, int /*a2*/)
{
    alignas(8) uint8_t entriesBytes[0x24];
    auto& entries = entriesBytes;
    CSF_AT(void*, entries, 0x00) = nullptr;
    CSF_AT(uint32_t, entries, 0x04) = 0;
    CSF_AT(uint32_t, entries, 0x08) = 0;
    CSF_AT(int, entries, 0x0C) = 10;
    FxParticleManager_GrowUpdateEntries(entries, nullptr, 50);

    uint32_t count = CSF_AT(uint32_t, entries, 0x08);
    const uint32_t sceneCount = CSF_AT(uint32_t, g_pCurrentScene, 0x28);
    for (uint32_t index = 0; index < sceneCount; ++index)
    {
        void* entity = CSF_AT(void*,
            CSF_AT(void*, g_pCurrentScene, 0x20), 4 * index);
        if (!(CSF_AT(uint32_t, entity, 0x170) & 0x2000))
            continue;
        if (!CSF_AT(uint32_t, entity, 0x50) &&
            (!CSF_AT(uint8_t, entity, 0x228) ||
             ((CSF_AT(uint32_t, entity, 0x2E0) & 0x10000) &&
              CSF_AT(uint8_t, entity, 0x3D0))))
            continue;

        void* state = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, entity, 0)[0x28 / 4])(entity, nullptr);
        const float distance = CSF_AT(float, state, 0x78);
        if (distance == g_MissingMetricValue ||
            !(distance < g_CoverAttackFarDistanceSquared))
            continue;

        const uint32_t handleLow = CSF_AT(uint32_t, entity, 0x20);
        const uint32_t handleHigh = CSF_AT(uint32_t, entity, 0x24);
        if (count >= CSF_AT(uint32_t, entries, 0x04))
        {
            FxParticleManager_GrowUpdateEntries(
                entries, nullptr, CSF_AT(int, entries, 0x0C));
            count = CSF_AT(uint32_t, entries, 0x08);
        }
        auto* entry = static_cast<uint8_t*>(CSF_AT(void*, entries, 0x00))
            + 28 * count;
        CSF_AT(uint32_t, entry, 0x00) = 0;
        CSF_AT(uint32_t, entry, 0x08) = handleLow;
        CSF_AT(uint32_t, entry, 0x0C) = handleHigh;
        CSF_AT(uint32_t, entries, 0x08) = ++count;
        CSF_AT(uint32_t, entry, 0x10) = 0xFFFFFFFF;
        CSF_AT(float, entry, 0x14) = distance;
        CSF_AT(float, entry, 0x18) = 0.0f;
    }

    if (count > CSF_AT(uint32_t,
            reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1F0))
    {
        _qsort(CSF_AT(void*, entries, 0x00), count, 0x1C,
            FxParticleManager_CompareSortKeys);
        count = CSF_AT(uint32_t,
            reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1F0);
        if (count > CSF_AT(uint32_t, entries, 0x04))
        {
            FxParticleManager_GrowUpdateEntries(entries, nullptr,
                static_cast<int>(count - CSF_AT(uint32_t, entries, 0x04)));
        }
        CSF_AT(uint32_t, entries, 0x08) = count;
    }

    auto* entry = static_cast<uint8_t*>(CSF_AT(void*, entries, 0x00));
    for (uint32_t remaining = count; remaining; --remaining, entry += 28)
    {
        bool unmatched = true;
        uint32_t oldOffset = 0;
        for (uint32_t oldIndex = 0;
             oldIndex < self->field_0018; ++oldIndex, oldOffset += 28)
        {
            if (!unmatched)
                break;
            if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010),
                    oldOffset + 0x08) == CSF_AT(uint32_t, entry, 0x08))
            {
                CSF_AT(uint32_t, entry, 0x00) = CSF_AT(uint32_t,
                    reinterpret_cast<void*>(self->field_0010), oldOffset);
                CSF_AT(uint32_t,
                    reinterpret_cast<void*>(self->field_0010), oldOffset) = 0;
                CSF_AT(uint32_t, entry, 0x10) = CSF_AT(uint32_t,
                    reinterpret_cast<void*>(self->field_0010), oldOffset + 0x10);
                CSF_AT(uint32_t, entry, 0x18) = CSF_AT(uint32_t,
                    reinterpret_cast<void*>(self->field_0010), oldOffset + 0x18);
                unmatched = false;
            }
        }
    }

    uint32_t offset = 0;
    for (uint32_t index = 0; index < self->field_0018; ++index, offset += 28)
    {
        void* particle = CSF_AT(void*,
            reinterpret_cast<void*>(self->field_0010), offset);
        if (!particle)
            continue;

        void** savedVtable = CSF_AT(void**, particle, 0);
        int state = ((int (__fastcall*)(void*, void*))
            savedVtable[0x48 / 4])(particle, nullptr);
        ((void (__fastcall*)(void*, void*, int))
            savedVtable[0x38 / 4])(particle, nullptr, state);
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, particle, 0)[0x2C / 4])(particle, nullptr, 0);

        auto* oldEntry = reinterpret_cast<uint8_t*>(self->field_0010) + offset;
        if (self->field_0044 >= self->field_0040)
        {
            const uint32_t growth = self->field_0048;
            void* storage = Mem_Alloc(4 * (growth + self->field_0040));
            const uint32_t used = self->field_0044;
            if (used)
                std::memcpy(storage,
                    reinterpret_cast<void*>(self->field_003c), 4 * used);
            self->field_0040 += growth;
            if (self->field_003c)
            {
                Mem_Free(reinterpret_cast<void*>(self->field_003c));
                self->field_003c = 0;
            }
            self->field_003c = reinterpret_cast<uint32_t>(storage);
        }
        const uint32_t poolIndex = self->field_0044;
        auto* poolSlot = reinterpret_cast<uint8_t*>(self->field_003c)
            + 4 * poolIndex;
        self->field_0044 = poolIndex + 1;
        CSF_AT(uint32_t, poolSlot, 0) = CSF_AT(uint32_t, oldEntry, 0);
    }

    uint32_t* result = FxParticleManager_ExchangeUpdateEntries(
        &self->field_0010, nullptr, reinterpret_cast<int>(entries));
    offset = 0;
    for (uint32_t index = 0; index < self->field_0018; ++index, offset += 28)
    {
        auto* current = reinterpret_cast<uint8_t*>(self->field_0010) + offset;
        if (!CSF_AT(uint32_t, current, 0))
        {
            const uint32_t poolCount = self->field_0044;
            if (poolCount)
            {
                CSF_AT(uint32_t, current, 0) = CSF_AT(uint32_t,
                    reinterpret_cast<void*>(self->field_003c), 4 * poolCount - 4);
                --self->field_0044;
            }
            else
            {
                void* services = reinterpret_cast<void*>(GetValue8B0554());
                void* factory = CSF_AT(void*, services, 0x1FCC);
                int definition = ((int (__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, factory, 0)[0x08 / 4])(
                        factory, nullptr, self->field_0008);
                definition += 4;
                void* global = reinterpret_cast<void*>(GetGlobalInt());
                void* registry = reinterpret_cast<void*>(
                    SceneLoader_GetObjectRegistry(global, nullptr));
                CSF_AT(int, current, 0) = ((int (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, registry, 0)[0x14 / 4])(
                        registry, nullptr, definition);
                const unsigned int random = RtRandom() % 0x3E8u;
                CSF_AT(float, current, 0x18) = static_cast<float>(
                    static_cast<double>(random) * g_SceneLoadMillisecondsToSeconds +
                    static_cast<double>(random) * g_SceneLoadMillisecondsToSeconds);
            }

            void* entity = reinterpret_cast<void*>(CHandleTable_Resolve(
                g_pCurrentScene, nullptr,
                static_cast<int>(self->field_0010 + offset + 4)));
            void* state = ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, entity, 0)[0x28 / 4])(entity, nullptr);
            int componentId = ((int (__fastcall*)(void*, void*, int))
                CSF_AT(void**, state, 0)[0x98 / 4])(state, nullptr, 60);
            void* component = ((void* (__fastcall*)(void*, void*, int))
                CSF_AT(void**, state, 0)[0x8C / 4])(
                    state, nullptr, componentId);
            void** particleVtable = CSF_AT(void**,
                CSF_AT(void*, current, 0), 0);
            int attachment = ((int (__fastcall*)(void*, void*))
                CSF_AT(void**, component, 0)[0x18 / 4])(component, nullptr);
            ((void (__fastcall*)(void*, void*, int))
                particleVtable[0x44 / 4])(
                    CSF_AT(void*, current, 0), nullptr, attachment);
        }
        result = reinterpret_cast<uint32_t*>(index + 1);
    }

    auto* base = static_cast<uint8_t*>(CSF_AT(void*, entries, 0));
    if (base)
    {
        uint32_t allocatedCount = CSF_AT(uint32_t, base - 4, 0);
        if (static_cast<int32_t>(allocatedCount - 1) >= 0)
        {
            auto* end = base + 28 * allocatedCount + 4;
            do
            {
                end -= 28;
                --allocatedCount;
                CSF_AT(void**, end, 0) = CSF_VTBL(0x0082D410);
            } while (allocatedCount);
        }
        return reinterpret_cast<uint32_t*>(Mem_Free(base - 4));
    }
    return result;
}

HOOK(0x0051F240, C_00840f6c_PrepareParticleUpdate, "auto");
