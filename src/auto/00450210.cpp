#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, Vector_GetLength, 0x00401FA0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));

static char __fastcall CGuiControl_FindNearbyChildPosition(
    CGuiControl* self, void* /*edx*/, int referenceObject,
    float maximumDistance, uint32_t* excludedFirst,
    uint32_t* excludedSecond, uint32_t* outputPosition)
{
    const uint32_t childCount = CSF_AT(uint32_t, self, 0x28);

    for (uint32_t index = 0; index < childCount; ++index)
    {
        void* child = CSF_AT(void**, self, 0x20)[index];
        if (!child)
            continue;

        const uint32_t flags = CSF_AT(uint32_t, child, 0x170);
        if (!(flags & 1) || !(flags & 0x20))
            continue;

        if (((bool(__fastcall*)(void*, void*))
                 CSF_AT(void**, child, 0)[0x7C / 4])(child, nullptr))
            continue;

        if (child == excludedFirst || child == excludedSecond)
            continue;

        auto* dispatcher = CSF_AT(C_0082dd78*, child, 0x4C);
        // sized by the original's frame (0x2C bytes): struct sizes in auto_classes.h are lower bounds
        alignas(8) uint8_t queryBytes[0x2C] = {};
        S_00694cd0& query = *reinterpret_cast<S_00694cd0*>(queryBytes);
        std::memset(&query, 0, 12);
        query.field_000c = 10;
        *reinterpret_cast<uint8_t*>(&query.field_0010) = 0;

        const int dispatchedObject =
            C_0082dd78_DispatchReturnedObject(dispatcher, nullptr);
        char found = 0;

        if (dispatchedObject)
        {
            void* object = reinterpret_cast<void*>(
                static_cast<uintptr_t>(dispatchedObject));

            if (((bool(__fastcall*)(void*, void*, int, S_00694cd0*, int))
                     CSF_AT(void**, object, 0)[0x40 / 4])(
                    object, nullptr, referenceObject, &query, 0))
            {
                auto* position = reinterpret_cast<uint32_t*>(query.field_0000);
                float facingStorage[6]{};
                auto* referencePosition = reinterpret_cast<float*>(
                    static_cast<uintptr_t>(referenceObject) + 12);

                float* facing = BuildFacingVectorBetween(
                    position, nullptr, facingStorage, referencePosition);

                if (Vector_GetLength(facing, nullptr) < maximumDistance)
                {
                    outputPosition[0] = position[0];
                    outputPosition[1] = position[1];
                    outputPosition[2] = position[2];
                    found = 1;
                }
            }
        }

        ResetEmbeddedMemberAt8(reinterpret_cast<void**>(&query), nullptr);
        if (found)
            return found;
    }

    return 0;
}

HOOK(0x00450210, CGuiControl_FindNearbyChildPosition, "auto");
