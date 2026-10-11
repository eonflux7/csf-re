#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, GfxScene_SetClumpState, 0x006B2D10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxScene_ConstructClumpInstance, 0x006B4330, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxScene_GetCategoryClumpCount, 0x006BCA20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxScene_GetCategoryClump, 0x006BCA40, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_EndRenderReset, 0x006CBC10, (void* self, void* edx));

static int __fastcall C_00860bb8_CreateClumpInstances(
    C_00860bb8* self, void* /*edx*/)
{
    SceneLoader_EndRenderReset(self, nullptr);

    int lastCount = 0;
    for (int category = 0; category < 3; ++category)
    {
        lastCount = GfxScene_GetCategoryClumpCount(
            reinterpret_cast<void*>(self->field_0008), nullptr, category);

        const std::uint32_t sourceCount = static_cast<std::uint32_t>(lastCount);
        const std::size_t listOffset = 0x24 + category * 0x10;
        auto& data = CSF_AT(std::uint32_t, self, listOffset);
        auto& capacity = CSF_AT(std::uint32_t, self, listOffset + 4);
        auto& count = category == 0
            ? self->field_002c
            : CSF_AT(std::uint32_t, self, listOffset + 8);
        auto& growth = CSF_AT(std::uint32_t, self, listOffset + 12);

        for (std::uint32_t index = 0; index < sourceCount; ++index)
        {
            void* instanceStorage = Mem_Alloc(212);
            int instance = 0;
            if (instanceStorage)
            {
                int sourceClump = GfxScene_GetCategoryClump(
                    reinterpret_cast<void*>(self->field_0008),
                    nullptr, category, static_cast<int>(index));
                instance = GfxScene_ConstructClumpInstance(
                    instanceStorage, nullptr, sourceClump);
            }

            GfxScene_SetClumpState(
                reinterpret_cast<void*>(instance), nullptr, 0);

            if (count >= capacity)
            {
                const std::uint32_t growthAmount = growth;
                void* replacement = Mem_Alloc(4 * (growthAmount + capacity));
                if (count)
                {
                    std::memcpy(
                        replacement, reinterpret_cast<const void*>(data),
                        4 * count);
                }

                capacity += growthAmount;
                if (data)
                {
                    Mem_Free(reinterpret_cast<void*>(data));
                    data = 0;
                }
                data = reinterpret_cast<std::uintptr_t>(replacement);
            }

            auto* destination = reinterpret_cast<std::uint32_t*>(
                data + 4 * count);
            ++count;
            *destination = static_cast<std::uint32_t>(instance);
        }
    }

    return lastCount;
}

HOOK(0x006CCB70, C_00860bb8_CreateClumpInstances, "auto");
