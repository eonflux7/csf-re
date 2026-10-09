#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, Vector_GetLength, 0x00401FA0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char __fastcall CGuiControl_FindNearbyFacingVector(
    CGuiControl* self, void* /*edx*/, int source, float maximumDistance,
    int excludedFirst, int excludedSecond, uint32_t* output)
{
    const uint32_t count = CSF_AT(uint32_t, self, 0x28);

    for (uint32_t index = 0; index < count; ++index)
    {
        void* child = CSF_AT(void**, self, 0x20)[index];
        if (!child || !(CSF_AT(uint8_t, child, 0x170) & 1))
            continue;

        if (((bool(__fastcall*)(void*, void*))
                 CSF_AT(void**, child, 0)[0x7C / 4])(child, nullptr))
            continue;

        if (child == reinterpret_cast<void*>(excludedFirst) ||
            child == reinterpret_cast<void*>(excludedSecond))
            continue;

        auto* dispatcher = CSF_AT(C_0082dd78*, child, 0x4C);
        uint32_t query[5] = {0, 0, 0, 10, 0};
        void* returnedObject = reinterpret_cast<void*>(
            C_0082dd78_DispatchReturnedObject(dispatcher, nullptr));

        const bool hasVector =
            ((bool(__fastcall*)(void*, void*, int, uint32_t*, int))
                 CSF_AT(void**, returnedObject, 0)[0x40 / 4])(
                returnedObject, nullptr, source, query, 0);

        auto* vector = reinterpret_cast<uint32_t*>(query[0]);
        char found = 0;
        if (hasVector)
        {
            float facingVector[6] = {};
            float* difference = BuildFacingVectorBetween(
                vector, nullptr, facingVector,
                reinterpret_cast<float*>(source + 12));

            if (Vector_GetLength(difference, nullptr) < maximumDistance)
            {
                output[0] = vector[0];
                output[1] = vector[1];
                output[2] = vector[2];
                found = 1;
            }
        }

        if (vector)
        {
            Mem_Free(vector);
            query[0] = 0;
        }

        if (found)
            return found;
    }

    return 0;
}

HOOK(0x004504A0, CGuiControl_FindNearbyFacingVector, "auto");
