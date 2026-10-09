#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, Vector_GetLength, 0x00401FA0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char __stdcall FindNearbyFacingPosition(
    void* object, int target, float maximumDistance, std::uint32_t* output)
{
    if (!object)
        return 0;

    const std::uint32_t flags = CSF_AT(std::uint32_t, object, 0x170);
    if ((flags & 1) == 0 || (flags & 0x20) == 0)
        return 0;

    if (((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[0x7C / 4])(object, nullptr))
        return 0;

    auto* dispatcher = reinterpret_cast<C_0082dd78*>(
        CSF_AT(std::uint32_t, object, 0x4C));

    std::uint32_t query[5] = {0, 0, 0, 10, 0};
    void* provider = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(dispatcher, nullptr));

    const int queryResult =
        ((int(__fastcall*)(void*, void*, int, std::uint32_t*, int))
            CSF_AT(void**, provider, 0)[0x40 / 4])(
                provider, nullptr, target, query, 0);

    void* position = reinterpret_cast<void*>(query[0]);
    char found = 0;

    if (static_cast<std::uint8_t>(queryResult))
    {
        float facingVector[4] = {};
        float* vector = BuildFacingVectorBetween(
            position, nullptr, facingVector,
            reinterpret_cast<float*>(static_cast<std::uintptr_t>(target) + 0xC));

        if (Vector_GetLength(vector, nullptr) < maximumDistance)
        {
            output[0] = CSF_AT(std::uint32_t, position, 0);
            output[1] = CSF_AT(std::uint32_t, position, 4);
            output[2] = CSF_AT(std::uint32_t, position, 8);
            found = 1;
        }
    }

    if (position)
        Mem_Free(position);

    return found;
}

HOOK(0x00450140, FindNearbyFacingPosition, "auto");
