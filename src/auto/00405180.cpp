#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

CSF_FUNC(char, __cdecl, ComputeBoundsQuery, 0x004048F0,
         (float* a1, int arg4, char arg8, char a4, char a5, char a6,
          uint32_t* a7, int a8, int* a9, int a10));
CSF_FUNC(char*, __fastcall, GetField3F0Ptr, 0x00495E60, (char* self, void*));

static char __fastcall S_00405180_QueryBoundsAndAdjustPosition(
    S_00405180* self, void* /*edx*/, float* query, float* result,
    char option1, char option2, int* details)
{
    alignas(8) uint8_t shapeScratch[0x3C] = {};
    alignas(8) uint8_t boundsBytes[0x18] = {};
    alignas(8) uint8_t transformedBytes[0xC] = {};
    alignas(8) uint8_t positionBytes[0x1C] = {};

    float* bounds = reinterpret_cast<float*>(boundsBytes);
    float* transformed = reinterpret_cast<float*>(transformedBytes);
    float* position = reinterpret_cast<float*>(positionBytes);

    void* owner = reinterpret_cast<void*>(self->field_0000);
    void* component =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])(
            owner, nullptr);
    void* geometry =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, component, 0)[0x70 / 4])(
            component, nullptr);
    void* shape =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, geometry, 0)[0x34 / 4])(
            geometry, nullptr);

    if (((int(__fastcall*)(void*, void*))CSF_AT(void**, shape, 0)[8 / 4])(
            shape, nullptr) == 0)
    {
        bounds[0] = CSF_AT(float, shape, 0x8);
        bounds[1] = CSF_AT(float, shape, 0xC);
        bounds[2] = CSF_AT(float, shape, 0x10);
        float radius = CSF_AT(float, shape, 0x14);
        bounds[3] = radius;
        bounds[4] = radius;
        bounds[5] = radius;
    }
    else if (((int(__fastcall*)(void*, void*))CSF_AT(void**, shape, 0)[8 / 4])(
                 shape, nullptr) == 1)
    {
        bounds[0] = CSF_AT(float, shape, 0x8);
        bounds[1] = CSF_AT(float, shape, 0xC);
        bounds[2] = CSF_AT(float, shape, 0x10);
        bounds[3] = CSF_AT(float, shape, 0x14);
        bounds[4] = CSF_AT(float, shape, 0x18);
        bounds[5] = CSF_AT(float, shape, 0x1C);
    }
    else if (((int(__fastcall*)(void*, void*))CSF_AT(void**, shape, 0)[8 / 4])(
                 shape, nullptr) == 2)
    {
        float* dimensions =
            ((float*(__fastcall*)(void*, void*))CSF_AT(void**, geometry, 0)[0x30 / 4])(
                geometry, nullptr);
        float halfX = dimensions[0] * g_HalfDurationScale;
        float halfY = dimensions[1] * g_HalfDurationScale;
        float halfZ = dimensions[2] * g_HalfDurationScale;

        float* center =
            ((float*(__fastcall*)(void*, void*, void*))CSF_AT(void**, shape, 0)[0x1C / 4])(
                shape, nullptr, shapeScratch);
        bounds[0] = center[0];
        bounds[1] = center[1];
        bounds[2] = center[2];
        bounds[3] = halfX;
        bounds[4] = halfY;
        bounds[5] = halfZ;
    }

    char* auxiliary = nullptr;
    owner = reinterpret_cast<void*>(self->field_0000);
    if (CSF_AT(uint32_t, owner, 0x50) == 0)
        auxiliary = GetField3F0Ptr(reinterpret_cast<char*>(owner), nullptr);

    char status = ComputeBoundsQuery(
        bounds, reinterpret_cast<int>(query), option1, option2,
        CSF_AT(char, self, 0x4), CSF_AT(char, self, 0x5),
        reinterpret_cast<uint32_t*>(self->field_0000),
        reinterpret_cast<int>(result), details, reinterpret_cast<int>(auxiliary));

    float* dimensions =
        ((float*(__fastcall*)(void*, void*))CSF_AT(void**, geometry, 0)[0x30 / 4])(
            geometry, nullptr);
    float halfHeight = dimensions[1] * g_HalfDurationScale;
    float x = CSF_AT(float, geometry, 0x18);
    float z = CSF_AT(float, geometry, 0x20);
    float y = halfHeight + CSF_AT(float, geometry, 0x1C);
    position[0] = x;
    position[1] = y;
    position[2] = z;

    owner = reinterpret_cast<void*>(self->field_0000);
    component =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])(
            owner, nullptr);
    void* transform =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, component, 0)[0x18 / 4])(
            component, nullptr);
    ((void(__fastcall*)(void*, void*, float*, float*))CSF_AT(void**, transform, 0)[0xA4 / 4])(
        transform, nullptr, transformed, position);

    result[2] = result[2] - transformed[0];
    result[3] = result[3] - transformed[1];
    result[4] = result[4] - transformed[2];
    return status;
}

HOOK(0x00405180, S_00405180_QueryBoundsAndAdjustPosition, "auto");
