#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_RegionScaleFactor, 0x008313E4);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObject_RefreshAfterRegionHit, 0x0048E4D0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, SceneGrid_RegisterHandlePosition, 0x004DD220, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, Spatial_TestPointInRegion, 0x0075CE00, (void* self, void* edx, float* a2, uint32_t* a3));
CSF_FUNC(double, __fastcall, Spatial_GetRegionScale, 0x0075D0A0, (void* self, void* edx, uint32_t* a2));

static char __stdcall SceneObject_SyncPositionFromHandleEvent(
    int /*unusedArg30*/, int /*unusedArg34*/, uint32_t* eventPayload)
{
    void* payloadOwner = reinterpret_cast<void*>(*eventPayload);
    void* handleRecord = reinterpret_cast<void*>(CSF_AT(uint32_t, payloadOwner, 4));
    const int32_t handleField = CSF_AT(int32_t, handleRecord, 12);
    const int handleAddress = reinterpret_cast<int>(static_cast<uint8_t*>(handleRecord) + 4);

    char returnValue = static_cast<char>(handleField);
    if (handleField >= 0)
    {
        const int resolvedValue = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress);
        returnValue = static_cast<char>(resolvedValue);
        CPlayerCharacter* object = reinterpret_cast<CPlayerCharacter*>(resolvedValue);
        if (resolvedValue != 0)
        {
            void* positionOwner = reinterpret_cast<void*>(eventPayload[1]);
            void* positionRecord = reinterpret_cast<void*>(CSF_AT(uint32_t, positionOwner, 4));
            void* positionBlock = static_cast<uint8_t*>(positionRecord) + 4;
            float position[3];
            position[0] = CSF_AT(float, positionBlock, 0);
            position[1] = CSF_AT(float, positionBlock, 4);
            position[2] = CSF_AT(float, positionBlock, 8);

            ((void(__fastcall*)(void*, void*, float*))object->vtbl[20])(
                object, nullptr, position);

            const int runtimeContext = GetValue8B0554();
            void* spatialWorld = reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(runtimeContext), 8152));
            SceneGrid_RegisterHandlePosition(
                static_cast<uint8_t*>(spatialWorld) + 52,
                nullptr, handleAddress, reinterpret_cast<uint32_t*>(position));

            const int runtimeContextAgain = GetValue8B0554();
            spatialWorld = reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(runtimeContextAgain), 8152));
            uint32_t cellIndexPair[2] = { 0xFFFFFFFFu, 0xFFFFFFFFu };

            returnValue = Spatial_TestPointInRegion(
                spatialWorld, nullptr, position, cellIndexPair);

            if (returnValue != 0)
            {
                const double regionScale = Spatial_GetRegionScale(
                    spatialWorld, nullptr, cellIndexPair);
                const float scaledRegionValue =
                    static_cast<float>(regionScale * g_RegionScaleFactor);

                // This callee ends in a virtual tail jump, forwarding the float
                // stack argument to its final callee. Its supplied binding omits it.
                returnValue = static_cast<char>(
                    reinterpret_cast<int(__fastcall*)(void*, void*, float)>(
                        SceneObject_RefreshAfterRegionHit)(
                            object, nullptr, scaledRegionValue));
            }

            C_008672e4* attachedObject =
                reinterpret_cast<C_008672e4*>(object->field_0034);
            if (attachedObject != nullptr)
            {
                returnValue = static_cast<char>(
                    ((int(__fastcall*)(void*, void*))attachedObject->vtbl[43])(
                        attachedObject, nullptr));
            }

            void* component = reinterpret_cast<void*>(object->field_0030);
            if (component != nullptr)
            {
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, component, 0)[40])(
                    component, nullptr);

                component = reinterpret_cast<void*>(object->field_0030);
                returnValue = static_cast<char>(
                    ((int(__fastcall*)(void*, void*))CSF_AT(void**, component, 0)[39])(
                        component, nullptr));
            }
        }
    }
    return returnValue;
}

HOOK(0x0045F560, SceneObject_SyncPositionFromHandleEvent, "auto");
