#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00833b88*, __fastcall, SpatialQuery_BuildContext, 0x0040FDA0, (C_00833b88* self, void* , uint32_t* a2, uint32_t* a3));
CSF_FUNC(void, __fastcall, C_0082e090_PlayActivitySound, 0x00416980, (C_0082e090* self, void* , uint32_t* eventData));
CSF_FUNC(void, __fastcall, C_0082e090_HandleCollision, 0x00416AE0, (C_0082e090* self, void* , const float* displacement, uint32_t* collision, int));
CSF_FUNC(int, __fastcall, C_0082e090_UpdateSurfaceContact, 0x00416EA0, (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_008342e8_SetObjectPlacement, 0x00491190, (C_008342e8* self, void* , uint32_t* position, float* direction));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_fIntroVideoDuration, 0x0082E118);
CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);

static void __fastcall C_0082e090_ProcessMovementCollision(
    C_0082e090* self, void* /*edx*/,
    const float* displacement, uint32_t* collision, int flags)
{
    alignas(8) uint8_t positionBytes[0xC];
    alignas(8) uint8_t directionBytes[0x10];
    alignas(8) uint8_t convertedDirectionBytes[0xC];
    float* position = reinterpret_cast<float*>(positionBytes);
    float* direction = reinterpret_cast<float*>(directionBytes);
    float* convertedDirection = reinterpret_cast<float*>(convertedDirectionBytes);

    direction[0] = CSF_AT(float, collision, 0x2C);
    direction[1] = CSF_AT(float, collision, 0x30);
    direction[2] = CSF_AT(float, collision, 0x34);
    position[0] = CSF_AT(float, collision, 0x08);
    position[1] = CSF_AT(float, collision, 0x0C);
    position[2] = CSF_AT(float, collision, 0x10);

    void* object = reinterpret_cast<void*>(CSF_AT(uint32_t, collision, 0x4C));
    if (object && (CSF_AT(uint32_t, object, 0x170) & 0x2000) != 0)
    {
        self->field_0077 = 0;
        int surfaceType = 0;

        alignas(8) uint8_t contextBytes[0x18];
        uint32_t* converted = S_00526d40_ConvertDirection(
            reinterpret_cast<S_00526d40*>(const_cast<float*>(displacement)),
            nullptr, reinterpret_cast<uint32_t*>(convertedDirectionBytes));
        uint32_t* objectPosition = GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0004), nullptr);
        SpatialQuery_BuildContext(
            reinterpret_cast<C_00833b88*>(contextBytes), nullptr,
            objectPosition, converted);

        void* queryObject = C_0082dd78_DispatchReturnedObject(
            CSF_AT(void*, CSF_AT(void*, collision, 0x4C), 0x4C), nullptr);

        alignas(8) uint8_t resultsBytes[0x14];
        CSF_AT(uint32_t, resultsBytes, 0x00) = 0;
        CSF_AT(uint32_t, resultsBytes, 0x04) = 0;
        CSF_AT(uint32_t, resultsBytes, 0x08) = 0;
        CSF_AT(int, resultsBytes, 0x0C) = 10;
        CSF_AT(uint8_t, resultsBytes, 0x10) = 0;

        if (((bool (__fastcall*)(void*, void*, C_00833b88*, uint32_t*, int))
                CSF_AT(void**, queryObject, 0)[0x40 / 4])(
                queryObject, nullptr,
                reinterpret_cast<C_00833b88*>(contextBytes),
                reinterpret_cast<uint32_t*>(resultsBytes), 0))
        {
            uint32_t selectedIndex = 0;
            uint32_t index = 0;
            bool found = false;
            if (CSF_AT(uint32_t, resultsBytes, 0x08))
            {
                int* type = reinterpret_cast<int*>(
                    CSF_AT(uint32_t, resultsBytes, 0x00) + 24);
                do
                {
                    if (found)
                        break;
                    int candidate = *type;
                    if (candidate == 1 || candidate == 4 || candidate == 2)
                    {
                        selectedIndex = index;
                        found = true;
                    }
                    ++index;
                    type += 10;
                }
                while (index < CSF_AT(uint32_t, resultsBytes, 0x08));
            }
            surfaceType = CSF_AT(int,
                reinterpret_cast<void*>(
                    CSF_AT(uint32_t, resultsBytes, 0x00) + 40 * selectedIndex),
                24);
        }

        int global = GetGlobalInt();
        void* sceneController = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(reinterpret_cast<void*>(global), nullptr));
        void* traceManager =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, sceneController, 0)[0x3C / 4])(
                sceneController, nullptr);

        float* sourcePosition = reinterpret_cast<float*>(
            GetObjectPositionVector(CSF_AT(void*, collision, 0x4C), nullptr));
        convertedDirection[0] = sourcePosition[0];
        convertedDirection[1] = sourcePosition[1] - g_WeaponPlacementTraceDepth;
        convertedDirection[2] = sourcePosition[2];

        alignas(8) uint8_t traceBytes[0x24];
        float* trace = reinterpret_cast<float*>(traceBytes);
        trace[6] = convertedDirection[0];
        trace[7] = sourcePosition[1] + g_MessageBoxAnimationScale;
        trace[8] = convertedDirection[2];
        trace[0] = trace[6];
        trace[3] = convertedDirection[0];
        trace[1] = trace[7];
        trace[2] = trace[8];
        trace[4] = convertedDirection[1];
        trace[5] = convertedDirection[2];

        int traceMask = 2;
        void* hit =
            ((void* (__fastcall*)(void*, void*, int, uint32_t*, int*, int))
                CSF_AT(void**, traceManager, 0)[0x34 / 4])(
                traceManager, nullptr, 0,
                reinterpret_cast<uint32_t*>(traceBytes), &traceMask, 3);

        if (hit)
        {
            alignas(8) uint8_t hitBytes[0x68];
            CSF_AT(uint8_t, hitBytes, 0x30) = 0xFF;
            CSF_AT(uint8_t, hitBytes, 0x20) = 0xFF;
            CSF_AT(int, hitBytes, 0x2C) = -1;
            CSF_AT(int, hitBytes, 0x1C) = -1;
            CSF_AT(float, hitBytes, 0x04) = 0.0f;
            CSF_AT(float, hitBytes, 0x08) = 0.0f;
            CSF_AT(float, hitBytes, 0x0C) = 0.0f;
            CSF_AT(float, hitBytes, 0x10) = 0.0f;
            CSF_AT(float, hitBytes, 0x14) = 0.0f;
            CSF_AT(float, hitBytes, 0x18) = 0.0f;
            CSF_AT(int, hitBytes, 0x5C) = 0;
            CSF_AT(uint8_t, hitBytes, 0x28) = 0;
            CSF_AT(int, hitBytes, 0x60) = 0;
            CSF_AT(int, hitBytes, 0x64) = 10;
            CSF_AT(int, hitBytes, 0x00) = 2139095039;
            CSF_AT(int, hitBytes, 0x24) = 0;

            ((void (__fastcall*)(void*, void*, int*))
                CSF_AT(void**, hit, 0)[0x0C / 4])(
                hit, nullptr, reinterpret_cast<int*>(hitBytes));

            convertedDirection[0] = CSF_AT(float, hitBytes, 0x04);
            convertedDirection[1] =
                CSF_AT(float, hitBytes, 0x08) + g_fIntroVideoDuration;
            convertedDirection[2] = CSF_AT(float, hitBytes, 0x0C);
            position[0] = CSF_AT(float, hitBytes, 0x04);
            position[1] = convertedDirection[1];
            position[2] = CSF_AT(float, hitBytes, 0x0C);
            direction[0] = CSF_AT(float, hitBytes, 0x10);
            direction[1] = CSF_AT(float, hitBytes, 0x14);
            direction[2] = CSF_AT(float, hitBytes, 0x18);
        }

        if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0)
            || CSF_AT(uint32_t, CSF_AT(void*, collision, 0x4C), 0x50))
        {
            C_0082e090_UpdateSurfaceContact(
                self, nullptr, CSF_AT(uint32_t*, collision, 0x4C), surfaceType);
        }

        if (CSF_AT(uint32_t, resultsBytes, 0x00))
            CoreMemory_Free(CSF_AT(void*, resultsBytes, 0x00));
    }
    else if (std::fabs(direction[0]) >= direction[1]
             || std::fabs(direction[2]) >= direction[1])
    {
        C_0082e090_HandleCollision(self, nullptr, displacement, collision, flags);
    }
    else
    {
        self->field_0077 = 0;
    }

    if (!self->field_0077)
    {
        C_008342e8_SetObjectPlacement(
            reinterpret_cast<C_008342e8*>(self->field_0004), nullptr,
            reinterpret_cast<uint32_t*>(positionBytes), direction);
        CSF_AT(float, collision, 0x08) = position[0];
        CSF_AT(float, collision, 0x0C) = position[1];
        CSF_AT(float, collision, 0x10) = position[2];
        C_0082e090_PlayActivitySound(self, nullptr, collision);
    }
}

HOOK(0x00417020, C_0082e090_ProcessMovementCollision, "auto");
