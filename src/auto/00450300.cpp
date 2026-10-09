#include <cstdint>
#include "globals.h"
#include <cmath>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static bool __stdcall SpatialQuery_HasNearbyFlaggedIntersection(
    void* objects, float* query, float distanceLimit, int excludedObject)
{
    const std::uint32_t objectCount = CSF_AT(std::uint32_t, objects, 0x08);
    bool found = false;

    for (std::uint32_t index = 0; index < objectCount && !found; ++index)
    {
        void* object = CSF_AT(void**, objects, 0x00)[index];
        if (!object ||
            !(CSF_AT(std::uint8_t, object, 0x170) & 0x20) ||
            reinterpret_cast<std::uintptr_t>(object) ==
                static_cast<std::uint32_t>(excludedObject))
        {
            continue;
        }

        auto* dispatcher = CSF_AT(C_0082dd78*, object, 0x4C);
        std::uint32_t result[0x1C / sizeof(std::uint32_t)]{};
        result[3] = 10;

        void* target = reinterpret_cast<void*>(
            C_0082dd78_DispatchReturnedObject(dispatcher, nullptr));

        const bool intersects =
            reinterpret_cast<bool (__fastcall*)(
                void*, void*, float*, std::uint32_t*, int)>(
                CSF_AT(void**, target, 0x00)[0x40 / 4])(
                    target, nullptr, query, result, 0);

        float* point = reinterpret_cast<float*>(result[0]);
        if (intersects)
        {
            const float dx = point[0] - query[3];
            const float dy = point[1] - query[4];
            const float dz = point[2] - query[5];
            if (std::sqrt(dz * dz + dy * dy + dx * dx) < distanceLimit)
                found = true;
        }

        if (point)
        {
            Mem_Free(point);
            result[0] = 0;
        }
    }

    return found;
}

HOOK(0x00450300, SpatialQuery_HasNearbyFlaggedIntersection, "auto");
