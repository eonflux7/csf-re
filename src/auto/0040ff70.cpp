#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0082dd78_FindNearestScenePosition(
    C_0082dd78* self, void* /*edx*/, int query, uint32_t* output)
{
    const double radius =
        (reinterpret_cast<double(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x5C / 4]))(self, nullptr);
    const float radiusSquared = static_cast<float>(radius * radius);
    const uint32_t objectCount = CSF_AT(uint32_t, g_pCurrentScene, 0x18);
    float nearestSquared = 100000000.0f;
    char found = 0;

    for (uint32_t i = 0; i < objectCount; ++i)
    {
        void* object = CSF_AT(void**, g_pCurrentScene, 0x10)[i];
        if (!object)
            continue;
        if ((CSF_AT(uint8_t, object, 0x170) & 1) == 0)
            continue;
        if (object == reinterpret_cast<void*>(self->field_0004))
            continue;

        if (!(reinterpret_cast<int(__fastcall*)(void*, void*)>(
                  CSF_AT(void**, object, 0)[0x28 / 4]))(object, nullptr))
            continue;

        const uint32_t flags = CSF_AT(uint32_t, object, 0x170);
        if ((flags & 8) == 0 || (flags & 0x2000) != 0)
            continue;

        const int dispatched = C_0082dd78_DispatchReturnedObject(
            CSF_AT(C_0082dd78*, object, 0x4C), nullptr);
        void* queryObject =
            reinterpret_cast<void*>(static_cast<uintptr_t>(dispatched));

        struct QueryResult
        {
            uint32_t words[4];
            uint8_t flag;
        };
        static_assert(sizeof(QueryResult) == 0x14);
        QueryResult result{};
        result.words[3] = 10;

        const bool succeeded =
            (reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*, int)>(
                CSF_AT(void**, queryObject, 0)[0x40 / 4]))(
                queryObject, nullptr, query, &result, 0);

        auto* position = reinterpret_cast<uint32_t*>(result.words[0]);
        if (succeeded)
        {
            float facing[0x24 / sizeof(float)]{};
            BuildFacingVectorBetween(
                position, nullptr, facing,
                reinterpret_cast<float*>(static_cast<uintptr_t>(query) + 12));

            const float distanceSquared =
                facing[2] * facing[2] +
                facing[1] * facing[1] +
                facing[0] * facing[0];

            if (distanceSquared < nearestSquared &&
                distanceSquared < radiusSquared)
            {
                nearestSquared = distanceSquared;
                output[0] = position[0];
                output[1] = position[1];
                output[2] = position[2];
                found = 1;
            }
        }

        if (position)
        {
            Mem_Free(position);
            result.words[0] = 0;
        }
    }

    return found;
}

HOOK(0x0040FF70, C_0082dd78_FindNearestScenePosition, "auto");
