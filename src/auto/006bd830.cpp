#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpLtMapWorldLightMapsQuery, 0x0056F380, (int a3));
CSF_FUNC(int, __cdecl, RpPVSWorldGetProgressCallBack, 0x005863D0, (int a1));
CSF_FUNC(int, __cdecl, RpWorldSetSectorRenderCallBack, 0x0058CC60,
         (int a1, int (__cdecl *a2)(int a1)));
CSF_FUNC(int, __cdecl, RpWorldAddClump, 0x0058E280, (int a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, C_008605d8_BuildClumpFromRecord, 0x006BD690,
         (void* self, void* edx, int a2));

static int __fastcall C_008605d8_PopulateWorld(C_008605d8* self, void* /*edx*/)
{
    const int progressCallback = RpPVSWorldGetProgressCallBack(self->field_0004);
    void* world = reinterpret_cast<void*>(self->field_0004);
    self->field_0090 = progressCallback != 0;
    CSF_AT(std::uint32_t, world, 0x0C) = 1;
    self->vSlot10(8, 1);

    const bool hasLightMaps = RpLtMapWorldLightMapsQuery(self->field_0004) != 0;
    self->vSlot10(16, hasLightMaps);

    for (std::uint32_t index = 0; index < self->field_0014; ++index)
    {
        const auto clump = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint32_t*>(self->field_000c)[index]);
        RpWorldAddClump(self->field_0004, clump);
    }

    auto* group = reinterpret_cast<std::uint8_t*>(&self->field_002c);
    int groupsRemaining = 3;
    do
    {
        const std::uint32_t count = CSF_AT(std::uint32_t, group, 0x08);
        for (std::uint32_t index = 0; index < count; ++index)
        {
            const auto clump = reinterpret_cast<std::uint32_t*>(
                reinterpret_cast<std::uint32_t*>(
                    CSF_AT(std::uint32_t, group, 0))[index]);
            RpWorldAddClump(self->field_0004, clump);
        }
        group += 0x10;
    } while (--groupsRemaining);

    std::uint32_t recordOffset = 0;
    for (std::uint32_t index = 0; index < self->field_0084; ++index)
    {
        const int clump = C_008605d8_BuildClumpFromRecord(
            self, nullptr, static_cast<int>(self->field_007c + recordOffset));

        if (clump)
        {
            const std::uint32_t capacity = self->field_0020;
            if (self->field_0024 >= capacity)
            {
                const std::uint32_t growth = self->field_0028;
                void* newClumps = Mem_Alloc(4 * (growth + capacity));
                const std::uint32_t count = self->field_0024;
                if (count)
                {
                    std::memcpy(newClumps,
                                reinterpret_cast<const void*>(self->field_001c),
                                4 * count);
                }
                self->field_0020 += growth;
                if (self->field_001c)
                {
                    Mem_Free(reinterpret_cast<void*>(self->field_001c));
                    self->field_001c = 0;
                }
                self->field_001c = reinterpret_cast<std::uintptr_t>(newClumps);
            }

            const std::uint32_t count = self->field_0024;
            int* destination = reinterpret_cast<int*>(self->field_001c + 4 * count);
            self->field_0024 = count + 1;
            *destination = clump;
        }
        recordOffset += 56;
    }

    const int controller = SceneLoader_GetSceneController(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    const std::uint8_t retainMatrices =
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(controller), 0xB4);

    if (!CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x04)
        && !retainMatrices)
    {
        std::uint32_t matrixRecordOffset = 0;
        for (std::uint32_t index = 0; index < self->field_0084; ++index)
        {
            void* record = reinterpret_cast<void*>(self->field_007c + matrixRecordOffset);
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(
                CSF_AT(std::uint32_t, record, 0x24)));
            matrixRecordOffset += 56;
        }
        self->field_0084 = 0;
    }

    const int global = GetGlobalInt();
    if (!CSF_AT(std::uint8_t, reinterpret_cast<void*>(global), 0x04))
    {
        return RpWorldSetSectorRenderCallBack(
            self->field_0004,
            reinterpret_cast<int (__cdecl *)(int)>(CSF_ADDR_FN(0x006BC730)));
    }
    return global;
}

HOOK(0x006BD830, C_008605d8_PopulateWorld, "auto");
