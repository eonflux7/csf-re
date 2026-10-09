#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(bool, __stdcall, IsOccluderNearOrigin, 0x0044F3E0, (void* a1, float* a4, float arg8));
CSF_FUNC(char, __stdcall, IsRayOccludedByObjects, 0x00450300, (uint32_t* a1, float* a2, float a3, int a4));
CSF_FUNC(int, __cdecl, Get_Global_8B0578, 0x005189D0, ());
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(float*, __fastcall, NormalizeVector, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, OrientMatrixAlongVector, 0x006ADFA0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, InitializeQuaternion, 0x006AE6D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CreateQuaternionMatrix, 0x006AE6F0, (void* self, void* edx, int arg0));
CSF_FUNC(float*, __fastcall, SetQuaternionAxisAngle, 0x006AE970, (void* self, void* edx, void* a2, float a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static const auto g_ButtonArrayVtable = CSF_VTBL(0x0041E5F0);

CSF_GLOBAL(float, g_Zero, 0x0082D4CC);
CSF_GLOBAL(float, g_QuarterTurnConstant, 0x0082FAD4);
CSF_GLOBAL(float, g_BoundsInset, 0x00831274);
CSF_GLOBAL(float, g_AxisZ, 0x008A7D84);
CSF_GLOBAL(float, g_AxisZ_Y, 0x008A7D88);
CSF_GLOBAL(float, g_AxisZ_Z, 0x008A7D8C);
CSF_GLOBAL(uint32_t, g_RayFlagsFirst, 0x008AFC98);
CSF_GLOBAL(uint32_t, g_RayFlagsSecond, 0x008AFC9C);
CSF_GLOBAL(uint16_t, g_RayFlagsLast, 0x008AFCA0);

CSF_GLOBAL(float, g_Point0X, 0x008AFCA8);
CSF_GLOBAL(float, g_Point0Y, 0x008AFCAC);
CSF_GLOBAL(float, g_Point0Z, 0x008AFCB0);
CSF_GLOBAL(float, g_Point1X, 0x008AFCB4);
CSF_GLOBAL(float, g_Point1Y, 0x008AFCB8);
CSF_GLOBAL(float, g_Point1Z, 0x008AFCBC);
CSF_GLOBAL(float, g_Point2X, 0x008AFCC0);
CSF_GLOBAL(float, g_Point2Y, 0x008AFCC4);
CSF_GLOBAL(float, g_Point2Z, 0x008AFCC8);
CSF_GLOBAL(float, g_Point3X, 0x008AFCCC);
CSF_GLOBAL(float, g_Point3Y, 0x008AFCD0);
CSF_GLOBAL(float, g_Point3Z, 0x008AFCD4);
CSF_GLOBAL(float, g_Point4X, 0x008AFCD8);
CSF_GLOBAL(float, g_Point4Y, 0x008AFCDC);
CSF_GLOBAL(float, g_Point4Z, 0x008AFCE0);
CSF_GLOBAL(float, g_Point5X, 0x008AFCE4);
CSF_GLOBAL(float, g_Point5Y, 0x008AFCE8);
CSF_GLOBAL(float, g_Point5Z, 0x008AFCEC);
CSF_GLOBAL(float, g_Point6X, 0x008AFCF0);
CSF_GLOBAL(float, g_Point6Y, 0x008AFCF4);
CSF_GLOBAL(float, g_Point6Z, 0x008AFCF8);
CSF_GLOBAL(float, g_Point7X, 0x008AFCFC);
CSF_GLOBAL(float, g_Point7Y, 0x008AFD00);
CSF_GLOBAL(float, g_Point7Z, 0x008AFD04);
CSF_GLOBAL(float, g_Point8X, 0x008AFD08);
CSF_GLOBAL(float, g_Point8Y, 0x008AFD0C);
CSF_GLOBAL(float, g_Point8Z, 0x008AFD10);
CSF_GLOBAL(float, g_Point9X, 0x008AFD14);
CSF_GLOBAL(float, g_Point9Y, 0x008AFD18);
CSF_GLOBAL(float, g_Point9Z, 0x008AFD1C);
CSF_GLOBAL(float, g_Point10X, 0x008AFD20);
CSF_GLOBAL(float, g_Point10Y, 0x008AFD24);
CSF_GLOBAL(float, g_Point10Z, 0x008AFD28);
CSF_GLOBAL(float, g_Point11X, 0x008AFD2C);
CSF_GLOBAL(float, g_Point11Y, 0x008AFD30);
CSF_GLOBAL(float, g_Point11Z, 0x008AFD34);
CSF_GLOBAL(float, g_Point12X, 0x008AFD38);
CSF_GLOBAL(float, g_Point12Y, 0x008AFD3C);
CSF_GLOBAL(float, g_Point12Z, 0x008AFD40);
CSF_GLOBAL(float, g_Point13X, 0x008AFD44);
CSF_GLOBAL(float, g_Point13Y, 0x008AFD48);
CSF_GLOBAL(float, g_Point13Z, 0x008AFD4C);
CSF_GLOBAL(float, g_Point14X, 0x008AFD50);
CSF_GLOBAL(float, g_Point14Y, 0x008AFD54);
CSF_GLOBAL(float, g_Point14Z, 0x008AFD58);
CSF_GLOBAL(float, g_Point15X, 0x008AFD5C);
CSF_GLOBAL(float, g_Point15Y, 0x008AFD60);
CSF_GLOBAL(float, g_Point15Z, 0x008AFD64);
CSF_GLOBAL(float, g_Point16X, 0x008AFD68);
CSF_GLOBAL(float, g_Point16Y, 0x008AFD6C);
CSF_GLOBAL(float, g_Point16Z, 0x008AFD70);
CSF_GLOBAL(float, g_Point17X, 0x008AFD74);
CSF_GLOBAL(float, g_Point17Y, 0x008AFD78);
CSF_GLOBAL(float, g_Point17Z, 0x008AFD7C);
CSF_GLOBAL(float, g_Point18X, 0x008AFD80);
CSF_GLOBAL(float, g_Point18Y, 0x008AFD84);
CSF_GLOBAL(float, g_Point18Z, 0x008AFD88);
CSF_GLOBAL(float, g_Point19X, 0x008AFD8C);
CSF_GLOBAL(float, g_Point19Y, 0x008AFD90);
CSF_GLOBAL(float, g_Point19Z, 0x008AFD94);
CSF_GLOBAL(uint32_t, g_RayInitializationFlags, 0x008AFD98);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall CGuiControl_RemoveOccludedObjects(
    CGuiControl* self, void* /*edx*/, uint32_t* objects,
    float* origin, float heightOffset, int collisionMask)
{
    void* globals = reinterpret_cast<void*>(GetGlobalInt());
    void* scene = reinterpret_cast<void*>(SceneLoader_GetSceneController(globals, nullptr));
    void* collisionWorld =
        reinterpret_cast<void*>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, scene, 0)[0x3C / 4])(scene, nullptr));

    const float originX = origin[0];
    const float originZ = origin[2];
    const float originY = origin[1] + heightOffset;

    // Storage owned by GrowButtonArray, not a C++ container.
    uint32_t occluders[5] = {0, 0, 0, 10, 0};
    const uint32_t childCount = self->m_id.alloc;
    for (uint32_t childIndex = 0; childIndex < childCount; ++childIndex)
    {
        void* child = CSF_AT(void*, reinterpret_cast<void*>(self->field_0010),
                             4 * childIndex);
        if (child)
        {
            const uint32_t flags = CSF_AT(uint32_t, child, 0x170);
            if ((flags & 1) != 0 && (flags & 0x20) != 0 &&
                !reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, child, 0)[0x7C / 4])(child, nullptr) &&
                IsOccluderNearOrigin(child, origin, heightOffset))
            {
                if (occluders[2] >= occluders[1])
                    GrowButtonArray(occluders, nullptr, static_cast<int>(occluders[3]));

                const uint32_t insertionIndex = occluders[2]++;
                CSF_AT(void*, reinterpret_cast<void*>(occluders[0]),
                       4 * insertionIndex) = child;
            }
        }
    }

    for (int objectIndex = static_cast<int>(objects[2] - 1);
         objectIndex >= 0; --objectIndex)
    {
        const int object = CSF_AT(int, reinterpret_cast<void*>(objects[0]),
                                  4 * objectIndex);
        auto* dispatcher = CSF_AT(C_0082dd78*, reinterpret_cast<void*>(object), 0x4C);
        void* returnedObject =
            reinterpret_cast<void*>(C_0082dd78_DispatchReturnedObject(dispatcher, nullptr));
        void* boundsProvider =
            reinterpret_cast<void*>(
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, returnedObject, 0)[0x34 / 4])(
                        returnedObject, nullptr));

        float bounds[15];
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, boundsProvider, 0)[0x1C / 4])(
                boundsProvider, nullptr, bounds);

        float scaledAxes[3][3];
        for (int axis = 0; axis < 3; ++axis)
            for (int component = 0; component < 3; ++component)
                scaledAxes[axis][component] =
                    bounds[6 + 3 * axis + component] * bounds[3 + axis] * g_BoundsInset;

        float direction[7];
        direction[0] = bounds[0] - originX;
        direction[1] = 0.0f;
        direction[2] = bounds[2] - originZ;
        if (direction[0] == g_ZeroVectorX &&
            g_Zero == g_ZeroVectorY &&
            direction[2] == g_ZeroVectorZ)
        {
            direction[0] = g_AxisZ;
            direction[1] = g_AxisZ_Y;
            direction[2] = g_AxisZ_Z;
        }
        else
        {
            NormalizeVector(direction, nullptr);
        }

        alignas(4) uint8_t orientation[0x24];
        CSF_AT(uint32_t*, orientation, 0) = RwMatrixCreate();
        CSF_AT(uint8_t, orientation, 4) = 1;
        OrientMatrixAlongVector(orientation, nullptr,
                               reinterpret_cast<uint32_t*>(direction));
        direction[0] *= 30.0f;
        direction[1] *= 30.0f;
        direction[2] *= 30.0f;

        if ((g_RayInitializationFlags & 1) == 0)
        {
            g_RayInitializationFlags |= 1u;
            for (int point = 0; point < 18; ++point)
            {
                CSF_AT(float, &g_Point2X, 12 * point) = g_ZeroVectorX;
                CSF_AT(float, &g_Point2X, 12 * point + 4) = g_ZeroVectorY;
                CSF_AT(float, &g_Point2X, 12 * point + 8) = g_ZeroVectorZ;
            }
        }

        g_Point1Y = bounds[1];
        g_Point0X = originX;
        g_Point0Y = originY;
        g_Point0Z = originZ;
        g_Point1X = bounds[0];
        g_Point1Z = bounds[2];

        float quaternion[0x80 / sizeof(float)];
        InitializeQuaternion(quaternion, nullptr);
        SetQuaternionAxisAngle(quaternion, nullptr,
                               CSF_AT(uint32_t*, orientation, 0), 1.5707964f);

        float doubledDirection[3] = {
            direction[0] + direction[0],
            direction[1] + direction[1],
            direction[2] + direction[2]
        };
        alignas(4) uint8_t firstMatrix[0x8];
        const int firstMatrixObject = CreateQuaternionMatrix(
            quaternion, nullptr, reinterpret_cast<int>(firstMatrix));
        const int firstMatrixAddress =
            CSF_AT(int, reinterpret_cast<void*>(firstMatrixObject), 0);
        float firstOffset[0x34 / sizeof(float)];
        firstOffset[0] = 0.0f;
        firstOffset[1] = 0.0f;
        firstOffset[2] = 0.0f;
        RwV3dTransformVectors(reinterpret_cast<int>(firstOffset),
                              reinterpret_cast<int>(doubledDirection),
                              1, firstMatrixAddress);
        float offsetX = firstOffset[0];
        float offsetY = firstOffset[1];
        float offsetZ = firstOffset[2];
        if (CSF_AT(uint8_t, firstMatrix, 4))
            RwMatrixDestroy(CSF_AT(uint32_t*, firstMatrix, 0));

        g_Point2X = offsetX + originX;
        g_Point3X = bounds[0];
        g_Point2Y = offsetY + originY;
        g_Point3Y = bounds[1];
        g_Point2Z = offsetZ + originZ;
        g_Point3Z = bounds[2];

        SetQuaternionAxisAngle(
            quaternion, nullptr,
            reinterpret_cast<uint8_t*>(CSF_AT(uint32_t*, orientation, 0)) + 16,
            1.5707964f);
        alignas(4) uint8_t secondMatrix[0x8];
        const int secondMatrixObject = CreateQuaternionMatrix(
            quaternion, nullptr, reinterpret_cast<int>(secondMatrix));
        const int secondMatrixAddress =
            CSF_AT(int, reinterpret_cast<void*>(secondMatrixObject), 0);
        float secondOffset[3] = {0.0f, 0.0f, 0.0f};
        RwV3dTransformVectors(reinterpret_cast<int>(secondOffset),
                              reinterpret_cast<int>(direction), 1, secondMatrixAddress);
        offsetX = secondOffset[0];
        offsetY = secondOffset[1];
        offsetZ = secondOffset[2];
        if (CSF_AT(uint8_t, secondMatrix, 4))
            RwMatrixDestroy(CSF_AT(uint32_t*, secondMatrix, 0));

        g_Point4X = offsetX + originX;
        g_Point5X = bounds[0];
        g_Point4Y = offsetY + originY;
        g_Point5Y = bounds[1];
        g_Point4Z = offsetZ + originZ;
        g_Point5Z = bounds[2];

        SetQuaternionAxisAngle(
            quaternion, nullptr,
            reinterpret_cast<uint8_t*>(CSF_AT(uint32_t*, orientation, 0)) + 16,
            -1.5707964f);
        alignas(4) uint8_t thirdMatrix[0x8];
        const int thirdMatrixObject = CreateQuaternionMatrix(
            quaternion, nullptr, reinterpret_cast<int>(thirdMatrix));
        const int thirdMatrixAddress =
            CSF_AT(int, reinterpret_cast<void*>(thirdMatrixObject), 0);
        float thirdOffset[3] = {0.0f, 0.0f, 0.0f};
        RwV3dTransformVectors(reinterpret_cast<int>(thirdOffset),
                              reinterpret_cast<int>(direction), 1, thirdMatrixAddress);
        offsetX = thirdOffset[0];
        offsetY = thirdOffset[1];
        offsetZ = thirdOffset[2];
        if (CSF_AT(uint8_t, thirdMatrix, 4))
            RwMatrixDestroy(CSF_AT(uint32_t*, thirdMatrix, 0));

        g_Point8X = originX;
        g_Point8Y = originY;
        g_Point8Z = originZ;
        g_Point10X = originX;
        g_Point6X = offsetX + originX;
        g_Point7X = bounds[0];
        g_Point10Y = originY;
        g_Point6Y = offsetY + originY;
        g_Point7Y = bounds[1];
        g_Point10Z = originZ;
        g_Point6Z = offsetZ + originZ;
        g_Point7Z = bounds[2];
        g_Point12X = originX;
        g_Point9X = bounds[0] + scaledAxes[1][0];
        g_Point12Y = originY;
        g_Point12Z = originZ;
        g_Point9Y = bounds[1] + scaledAxes[1][1];
        g_Point9Z = bounds[2] + scaledAxes[1][2];
        g_Point11X = bounds[0] - scaledAxes[1][0];
        g_Point11Y = bounds[1] - scaledAxes[1][1];
        g_Point11Z = bounds[2] - scaledAxes[1][2];
        g_Point13X = bounds[0] + scaledAxes[0][0];
        g_Point13Y = bounds[1] + scaledAxes[0][1];
        g_Point13Z = bounds[2] + scaledAxes[0][2];
        g_Point15X = bounds[0] - scaledAxes[0][0];
        g_Point15Y = bounds[1] - scaledAxes[0][1];
        g_Point15Z = bounds[2] - scaledAxes[0][2];
        g_Point14Z = originZ;
        g_Point16Z = originZ;
        g_Point18Z = originZ;
        g_Point17X = bounds[0] + scaledAxes[2][0];
        g_Point14X = originX;
        g_Point14Y = originY;
        g_Point16X = originX;
        g_Point17Y = bounds[1] + scaledAxes[2][1];
        g_Point16Y = originY;
        g_Point18X = originX;
        g_Point18Y = originY;
        g_Point17Z = bounds[2] + scaledAxes[2][2];
        g_Point19X = bounds[0] - scaledAxes[2][0];
        g_RayFlagsFirst = 16843009;
        g_RayFlagsSecond = 16843009;
        g_Point19Y = bounds[1] - scaledAxes[2][1];
        g_RayFlagsLast = 257;
        g_Point19Z = bounds[2] - scaledAxes[2][2];

        float rayScratch[0x30 / sizeof(float)];
        for (int rayIndex = 1; rayIndex <= 3; ++rayIndex)
        {
            const int pointOffset = (rayIndex - 1) * 24;
            rayScratch[2] = originZ;
            rayScratch[5] = CSF_AT(float, &g_Point2X, pointOffset + 8);
            rayScratch[4] = CSF_AT(float, &g_Point2X, pointOffset + 4);
            rayScratch[3] = CSF_AT(float, &g_Point2X, pointOffset);
            rayScratch[0] = originX;
            rayScratch[1] = originY;

            if (reinterpret_cast<bool(__fastcall*)(void*, void*, float*, int, int)>(
                    CSF_AT(void**, collisionWorld, 0)[0x24 / 4])(
                        collisionWorld, nullptr, rayScratch, collisionMask, 3))
            {
                CSF_AT(uint8_t, &g_RayFlagsFirst, rayIndex) = 0;
            }
        }

        bool visible = false;
        for (uint32_t rayIndex = 0, byteOffset = 0;
             byteOffset < 0xF0; ++rayIndex, byteOffset += 24)
        {
            if (visible)
                break;

            if (CSF_AT(uint8_t, &g_RayFlagsFirst, rayIndex))
            {
                float* ray = reinterpret_cast<float*>(
                    reinterpret_cast<uint8_t*>(&g_Point0X) + byteOffset);
                if (!reinterpret_cast<bool(__fastcall*)(void*, void*, float*, int, int)>(
                        CSF_AT(void**, collisionWorld, 0)[0x24 / 4])(
                            collisionWorld, nullptr, ray, collisionMask, 3))
                {
                    bool occluded = false;
                    if (occluders[2])
                    {
                        const float deltaX = ray[3] - ray[0];
                        const float deltaY = ray[4] - ray[1];
                        const float deltaZ = ray[5] - ray[2];
                        float* objectRay = rayScratch + 6;
                        objectRay[0] = deltaX;
                        objectRay[1] = deltaY;
                        objectRay[2] = deltaZ;
                        objectRay[3] = ray[0];
                        objectRay[4] = ray[1];
                        // The original converts this coordinate to a signed integer.
                        CSF_AT(int, objectRay, 0x14) = static_cast<int>(ray[2]);
                        NormalizeVector(objectRay, nullptr);
                        const float length =
                            std::sqrt(deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX);
                        occluded = IsRayOccludedByObjects(
                            occluders, objectRay, length, object) != 0;
                    }
                    if (!occluded)
                    {
                        visible = true;
                        Get_Global_8B0578();
                    }
                }
            }
        }

        if (!visible)
        {
            const int lastIndex = static_cast<int>(objects[2] - 1);
            objects[2] = static_cast<uint32_t>(lastIndex);
            if (objectIndex != lastIndex)
            {
                _memcpy(
                    reinterpret_cast<uint8_t*>(objects[0]) + 4 * objectIndex,
                    reinterpret_cast<uint8_t*>(objects[0]) + 4 * objectIndex + 4,
                    4 * (lastIndex - objectIndex));
            }
        }

        if (CSF_AT(uint8_t, orientation, 4))
            RwMatrixDestroy(CSF_AT(uint32_t*, orientation, 0));
    }

    if (occluders[0])
        return Mem_Free(reinterpret_cast<void*>(occluders[0]));
    return 0;
}

HOOK(0x00451250, CGuiControl_RemoveOccludedObjects, "auto");
