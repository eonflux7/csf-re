#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Interface_SwapDisplayLists, 0x004016B0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, Interface_GrowDisplayList, 0x004017A0, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, Interface_CreateDisplay, 0x00401B50, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, Interface_AppendDisplayEntry, 0x00401C80, (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __cdecl, _qsort, 0x007AEAE0, (void* base, std::size_t count, std::size_t size, int (__cdecl* compare)(const void*, const void*)));

CSF_GLOBAL(float, g_CoverAttackFarDistanceSquared, 0x0082D4C0);
CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall S_00401b50_UpdateGlobalDisplayTime(
    S_00401b50* self, void* /*edx*/, int /*a2*/)
{
    int result = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xD0))
        return result;

    CSF_AT(uint32_t, self, 0x30) = 0;
    const uint32_t sceneCount = CSF_AT(uint32_t, g_pCurrentScene, 0x28);

    alignas(8) uint8_t entryBytes[0x28];
    for (uint32_t index = 0; index < sceneCount; ++index)
    {
        void* sceneEntries = CSF_AT(void*, g_pCurrentScene, 0x20);
        void* entity = CSF_AT(void*, sceneEntries, index * 4);
        if (!(CSF_AT(uint32_t, entity, 0x170) & 0x2000))
            continue;
        if (!CSF_AT(uint32_t, entity, 0x50))
            continue;

        void* component =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, entity, 0)[40 / 4])(
                entity, nullptr);
        void* state =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, component, 0)[120 / 4])(
                component, nullptr);
        if (!state)
            continue;

        component =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, entity, 0)[40 / 4])(
                entity, nullptr);
        state =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, component, 0)[120 / 4])(
                component, nullptr);
        if (((bool (__fastcall*)(void*, void*))CSF_AT(void**, state, 0)[40 / 4])(
                state, nullptr))
            continue;

        component =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, entity, 0)[40 / 4])(
                entity, nullptr);
        float metric = CSF_AT(float, component, 0x78);
        if (metric != g_MissingMetricValue && metric < g_CoverAttackFarDistanceSquared)
        {
            CSF_AT(uint32_t, entryBytes, 0x08) = CSF_AT(uint32_t, entity, 0x20);
            CSF_AT(float, entryBytes, 0x10) = metric;
            CSF_AT(void**, entryBytes, 0x04) = CSF_VTBL(0x0082D448);
            CSF_AT(uint32_t, entryBytes, 0x0C) = CSF_AT(uint32_t, entity, 0x24);
            CSF_AT(uint32_t, entryBytes, 0x00) = 0;
            Interface_AppendDisplayEntry(
                reinterpret_cast<uint8_t*>(self) + 0x28, nullptr,
                reinterpret_cast<uint32_t*>(entryBytes), 0);
            CSF_AT(void**, entryBytes, 0x04) = CSF_VTBL(0x0082D410);
        }
    }

    uint32_t candidateCount = CSF_AT(uint32_t, self, 0x30);
    if (candidateCount > 0xA)
    {
        _qsort(
            CSF_AT(void*, self, 0x28), candidateCount, 0x14,
            reinterpret_cast<int (__cdecl*)(const void*, const void*)>(
                CSF_ADDR_FN(0x004010C0)));
        uint32_t capacity = CSF_AT(uint32_t, self, 0x2C);
        if (capacity < 0xA)
            Interface_GrowDisplayList(
                reinterpret_cast<uint8_t*>(self) + 0x28, nullptr,
                static_cast<int>(10 - capacity));
        CSF_AT(uint32_t, self, 0x30) = 10;
    }

    if (CSF_AT(uint32_t, self, 0x30))
    {
        uint32_t candidateIndex = 0;
        uint32_t candidateOffset = 0;
        do
        {
            bool unmatched = true;
            uint32_t oldIndex = 0;
            uint32_t oldOffset = 0;
            if (CSF_AT(uint32_t, self, 0x10))
            {
                do
                {
                    if (!unmatched)
                        break;
                    void* candidate = reinterpret_cast<uint8_t*>(
                        CSF_AT(void*, self, 0x28)) + candidateOffset;
                    void* oldEntry = reinterpret_cast<uint8_t*>(
                        CSF_AT(void*, self, 0x08)) + oldOffset;
                    if (CSF_AT(uint32_t, oldEntry, 0x08) ==
                        CSF_AT(uint32_t, candidate, 0x08))
                    {
                        CSF_AT(uint32_t, candidate, 0) = CSF_AT(uint32_t, oldEntry, 0);
                        CSF_AT(uint32_t, CSF_AT(void*, self, 0x08), oldOffset) = 0;
                        unmatched = false;
                    }
                    uint32_t oldCount = CSF_AT(uint32_t, self, 0x10);
                    oldOffset += 20;
                    ++oldIndex;
                    if (oldIndex >= oldCount)
                        break;
                } while (true);
            }
            ++candidateIndex;
            candidateOffset += 20;
        } while (candidateIndex < CSF_AT(uint32_t, self, 0x30));
    }

    if (CSF_AT(uint32_t, self, 0x10))
    {
        uint32_t oldIndex = 0;
        uint32_t oldOffset = 0;
        do
        {
            void* display = CSF_AT(void*, CSF_AT(void*, self, 0x08), oldOffset);
            if (display)
            {
                ((void (__fastcall*)(void*, void*))CSF_AT(void**, display, 0)[36 / 4])(
                    display, nullptr);

                void* oldEntry = reinterpret_cast<uint8_t*>(
                    CSF_AT(void*, self, 0x08)) + oldOffset;
                uint32_t capacity = CSF_AT(uint32_t, self, 0x1C);
                if (self->field_0020 >= capacity)
                {
                    uint32_t growth = CSF_AT(uint32_t, self, 0x24);
                    void* replacement = reinterpret_cast<void*>(
                        CoreMemory_AllocUntagged(4 * (growth + capacity)));
                    uint32_t pooledCount = self->field_0020;
                    if (pooledCount)
                    {
                        std::memcpy(
                            replacement, CSF_AT(void*, self, 0x18), 4 * pooledCount);
                    }
                    CSF_AT(uint32_t, self, 0x1C) += growth;
                    if (CSF_AT(void*, self, 0x18))
                    {
                        CoreMemory_Free(CSF_AT(void*, self, 0x18));
                        CSF_AT(uint32_t, self, 0x18) = 0;
                    }
                    CSF_AT(void*, self, 0x18) = replacement;
                }
                uint32_t pooledCount = self->field_0020;
                void* destination = reinterpret_cast<uint8_t*>(
                    CSF_AT(void*, self, 0x18)) + 4 * pooledCount;
                self->field_0020 = pooledCount + 1;
                CSF_AT(uint32_t, destination, 0) = CSF_AT(uint32_t, oldEntry, 0);
            }
            oldOffset += 20;
            ++oldIndex;
        } while (oldIndex < CSF_AT(uint32_t, self, 0x10));
    }

    Interface_SwapDisplayLists(
        reinterpret_cast<uint8_t*>(self) + 0x08, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x28));

    result = CSF_AT(int, self, 0x10);
    if (result)
    {
        uint32_t index = 0;
        uint32_t offset = 0;
        do
        {
            void* entry = reinterpret_cast<uint8_t*>(CSF_AT(void*, self, 0x08)) + offset;
            if (!CSF_AT(void*, entry, 0))
            {
                CSF_AT(int*, entry, 0) = Interface_CreateDisplay(
                    self, nullptr,
                    reinterpret_cast<int>(reinterpret_cast<uint8_t*>(entry) + 4));
                void* display = CSF_AT(void*, CSF_AT(void*, self, 0x08), offset);
                if (display)
                {
                    ((void (__fastcall*)(void*, void*))CSF_AT(void**, display, 0)[32 / 4])(
                        display, nullptr);
                }
            }
            uint32_t count = CSF_AT(uint32_t, self, 0x10);
            result = static_cast<int>(index + 1);
            offset += 20;
            ++index;
            if (index >= count)
                break;
        } while (true);
    }
    return result;
}

HOOK(0x00401CD0, S_00401b50_UpdateGlobalDisplayTime, "auto");
