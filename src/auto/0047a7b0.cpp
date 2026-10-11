#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, GfxMatrix_EulerAnglesToDirection, 0x006AE640, (float* self, void* , float* out));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

struct WeaponMatrixReference
{
    float* matrix;
    std::uint8_t owned;
    std::uint8_t padding[3];
};

static void WeaponFirearms_CopyVector(float* destination, const float* source)
{
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
}

static int WeaponFirearms_FloatBits(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static int __fastcall C_008319c0_GetFiringPositionAndDirection(
    C_008319c0* self, void* /*edx*/, int* targeting, float* position, float* direction)
{
    WeaponMatrixReference matrixReference{};

    if (self->vResolveEntryLookupValue())
    {
        void* attachment = reinterpret_cast<void*>(self->vSlotD8());
        if (attachment)
        {
            void* transform =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, attachment, 0)[0x18 / 4])(attachment, nullptr);
            float* attachmentPosition =
                ((float*(__fastcall*)(void*, void*))
                    CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);
            WeaponFirearms_CopyVector(position, attachmentPosition);

            transform =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, attachment, 0)[0x18 / 4])(attachment, nullptr);
            void* returnedReference =
                ((void*(__fastcall*)(void*, void*, WeaponMatrixReference*))
                    CSF_AT(void**, transform, 0)[0x8C / 4])(
                        transform, nullptr, &matrixReference);
            float* matrix = CSF_AT(float*, returnedReference, 0);
            WeaponFirearms_CopyVector(direction, matrix + 8);
            if (matrixReference.owned)
                RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixReference.matrix));
        }
    }
    else
    {
        int weaponType = C_008319c0_GetSubObjectField74(self, nullptr);
        int entryIndex = 0;
        bool useTable = false;
        bool useSceneDirection = false;

        switch (weaponType)
        {
        case 12:
            entryIndex = 27;
            useTable = true;
            break;
        case 13:
            if (CSF_AT(std::uint8_t, targeting, 0x23))
                useSceneDirection = true;
            else
            {
                entryIndex = 25;
                useTable = true;
            }
            break;
        case 14:
            if (CSF_AT(std::uint8_t, targeting, 0x23))
                useSceneDirection = true;
            else
            {
                entryIndex = 1;
                useTable = true;
            }
            break;
        case 20:
            if (CSF_AT(std::uint8_t, targeting, 0x23))
                useSceneDirection = true;
            else
            {
                entryIndex = 0;
                useTable = true;
            }
            break;
        case 30:
            entryIndex = 28;
            useTable = true;
            break;
        default:
            break;
        }

        if (useSceneDirection)
        {
            float positionStorage[4]{};
            std::uint32_t anglesReference[2]{};
            float directionStorage[3]{};
            void* context = reinterpret_cast<void*>(
                GetSceneMenuContext(g_pCurrentScene, nullptr));

            float* scenePosition =
                ((float*(__fastcall*)(void*, void*, float*))
                    CSF_AT(void**, context, 0)[0xA4 / 4])(
                        context, nullptr, positionStorage);
            WeaponFirearms_CopyVector(position, scenePosition);

            float* angles =
                ((float*(__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, context, 0)[0xAC / 4])(
                        context, nullptr, anglesReference);
            float* sceneDirection =
                GfxMatrix_EulerAnglesToDirection(angles, nullptr, directionStorage);
            WeaponFirearms_CopyVector(direction, sceneDirection);
        }
        else if (useTable)
        {
            void* owner = reinterpret_cast<void*>(self->field_0004);
            void* component = CSF_AT(void*, owner, 0x4C);
            void* table =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, component, 0)[0x3C / 4])(component, nullptr);
            void* entry = reinterpret_cast<void*>(
                GetTableEntryDwordByIndex(table, nullptr, entryIndex));

            if (entry)
            {
                ((void(__fastcall*)(void*, void*, WeaponMatrixReference*))
                    CSF_AT(void**, entry, 0)[0x3C / 4])(
                        entry, nullptr, &matrixReference);
                float* matrix = matrixReference.matrix;
                WeaponFirearms_CopyVector(position, matrix + 12);
                if (weaponType == 20)
                {
                    float facingStorage[3]{};
                    float* facing = BuildFacingVectorBetween(
                        matrix + 8, nullptr, facingStorage, position);
                    WeaponFirearms_CopyVector(direction, facing);
                }
                else
                {
                    WeaponFirearms_CopyVector(direction, matrix + 8);
                }
                if (matrixReference.owned)
                    RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrix));
            }
        }
    }

    int result = targeting[0];
    if (result == 1 || result == 2)
    {
        void* target = reinterpret_cast<void*>(
            CHandleTable_Resolve(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(targeting + 1)));
        if (CSF_AT(std::uint32_t, target, 0x170) & 0x2000)
        {
            void* targetObject = reinterpret_cast<void*>(
                C_0082dd78_DispatchReturnedObject(
                    CSF_AT(C_0082dd78*, target, 0x4C), nullptr));
            int pointIndex =
                ((int(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, targetObject, 0)[0x54 / 4])(
                        targetObject, nullptr, 1);
            float targetStorage[3]{};
            float* targetPosition;
            if (pointIndex == -1)
            {
                targetPosition =
                    ((float*(__fastcall*)(void*, void*, float*))
                        CSF_AT(void**, target, 0)[0xA4 / 4])(
                            target, nullptr, targetStorage);
            }
            else
            {
                targetPosition =
                    ((float*(__fastcall*)(void*, void*, float*, int))
                        CSF_AT(void**, targetObject, 0)[0x4C / 4])(
                            targetObject, nullptr, targetStorage, pointIndex);
            }

            float x = targetPosition[0];
            float y = targetPosition[1];
            float z = targetPosition[2];
            float deltaX = x - position[0];
            float deltaY = y - position[1];
            float deltaZ = z - position[2];
            direction[0] = deltaX;
            direction[1] = deltaY;
            direction[2] = deltaZ;
            result = WeaponFirearms_FloatBits(deltaZ);
        }
        else
        {
            return GameConfig_IsResolvedEntryActive(CSF_AT(int, target, 0x50));
        }
    }
    else if (result == 3)
    {
        float deltaX = CSF_AT(float, targeting, 0x10) - position[0];
        float deltaY = CSF_AT(float, targeting, 0x14) - position[1];
        float deltaZ = CSF_AT(float, targeting, 0x18) - position[2];
        direction[0] = deltaX;
        direction[1] = deltaY;
        direction[2] = deltaZ;
        result = WeaponFirearms_FloatBits(deltaZ);
    }

    return result;
}

HOOK(0x0047A7B0, C_008319c0_GetFiringPositionAndDirection, "auto");
