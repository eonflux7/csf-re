#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float angle));
CSF_FUNC(char, __fastcall, C_00833908_UpdateComponents, 0x004910C0, (C_00833908* self, void* , float deltaTime));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void* , float* angles));

CSF_GLOBAL(float, g_Const20, 0x0082E0F0);
CSF_GLOBAL(float, g_RadiansToDegrees, 0x00834244);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008342e8_UpdateFocusEntries(
    C_008342e8* self, void* /*edx*/, float deltaTime)
{
    alignas(8) std::uint8_t eulerBytes[0xC];
    alignas(8) std::uint8_t facingPositionBytes[0xC];
    alignas(8) std::uint8_t scaledDirectionBytes[0xC];
    alignas(8) std::uint8_t directionBytes[0xC];
    alignas(8) std::uint8_t positionBytes[0xC];
    alignas(8) std::uint8_t rotationMatrixBytes[0x8];
    alignas(8) std::uint8_t directionMatrixBytes[0x14];

    S_00526d40& direction =
        *reinterpret_cast<S_00526d40*>(directionBytes);
    float* scaledDirection = reinterpret_cast<float*>(scaledDirectionBytes);
    int* position = reinterpret_cast<int*>(positionBytes);

    C_00833908_UpdateComponents(self, nullptr, deltaTime);

    void* components =
        reinterpret_cast<void*>(self->vGetField016c());
    int componentIndex =
        ((int(__fastcall*)(void*, void*, int))
             CSF_AT(void**, components, 0)[0x98 / 4])(
            components, nullptr, 3000);

    if (componentIndex == -1)
        return componentIndex;

    components = reinterpret_cast<void*>(self->vGetField016c());
    void* referenceComponent =
        ((void*(__fastcall*)(void*, void*, int))
             CSF_AT(void**, components, 0)[0x8C / 4])(
            components, nullptr, componentIndex);

    int result = static_cast<int>(
        reinterpret_cast<std::uintptr_t>(referenceComponent));
    if (!referenceComponent || self->field_01a0 == 0)
        return result;

    std::uint32_t entryIndex = 0;
    std::uint32_t entryOffset = 0;
    do
    {
        auto* entry =
            reinterpret_cast<std::uint8_t*>(self->field_0198) + entryOffset;

        if (CSF_AT(int, entry, 0x8) >= 0 &&
            (CSF_AT(std::uint8_t, entry, 0x18) & 0xA) == 0)
        {
            auto* target = reinterpret_cast<C_00834050*>(
                CHandleTable_Resolve(
                    g_pCurrentScene, nullptr,
                    static_cast<int>(reinterpret_cast<std::uintptr_t>(entry))));

            if (target)
            {
                components = reinterpret_cast<void*>(self->vGetField016c());
                int entryComponentIndex =
                    ((int(__fastcall*)(void*, void*, int))
                         CSF_AT(void**, components, 0)[0x98 / 4])(
                        components, nullptr, CSF_AT(int, entry, 0x10));

                if (entryComponentIndex != -1)
                {
                    components =
                        reinterpret_cast<void*>(self->vGetField016c());
                    void* entryComponent =
                        ((void*(__fastcall*)(void*, void*, int))
                             CSF_AT(void**, components, 0)[0x8C / 4])(
                            components, nullptr, entryComponentIndex);

                    if (entryComponent)
                    {
                        if ((CSF_AT(std::uint8_t, entry, 0x18) & 0x10) != 0)
                        {
                            void* transform =
                                ((void*(__fastcall*)(void*, void*))
                                     CSF_AT(void**, entryComponent, 0)[0x18 / 4])(
                                    entryComponent, nullptr);

                            void* matrixReference =
                                ((void*(__fastcall*)(void*, void*, void*))
                                     CSF_AT(void**, transform, 0)[0x8C / 4])(
                                    transform, nullptr, directionMatrixBytes);

                            auto* matrix =
                                CSF_AT(std::uint8_t*, matrixReference, 0);
                            std::memcpy(directionBytes, matrix + 0x20, 0xC);

                            if (CSF_AT(std::uint8_t, directionMatrixBytes, 0x4))
                                RwMatrixDestroy(
                                    CSF_AT(std::uint32_t*, directionMatrixBytes, 0));

                            position[0] = 0;
                            position[1] = 0;
                            position[2] = 0;

                            transform =
                                ((void*(__fastcall*)(void*, void*))
                                     CSF_AT(void**, entryComponent, 0)[0x18 / 4])(
                                    entryComponent, nullptr);

                            scaledDirection[0] =
                                CSF_AT(float, directionBytes, 0) * g_Const20;
                            scaledDirection[1] =
                                CSF_AT(float, directionBytes, 0x4) * g_Const20;
                            scaledDirection[2] =
                                CSF_AT(float, directionBytes, 0x8) * g_Const20;

                            float* origin =
                                ((float*(__fastcall*)(void*, void*))
                                     CSF_AT(void**, transform, 0)[0x10 / 4])(
                                    transform, nullptr);
                            float* facingPosition = BuildFacingVectorBetween(
                                origin, nullptr,
                                reinterpret_cast<float*>(facingPositionBytes),
                                scaledDirection);
                            std::memcpy(positionBytes, facingPosition, 0xC);

                            float* objectPosition =
                                GetObjectPositionVector(self, nullptr);
                            CSF_AT(std::uint32_t, positionBytes, 0x4) =
                                CSF_AT(std::uint32_t, objectPosition, 0x4);

                            if (target->field_0050)
                            {
                                void* inner =
                                    reinterpret_cast<void*>(target->field_004c);
                                ((void(__fastcall*)(void*, void*, int*))
                                     CSF_AT(void**, inner, 0)[0x50 / 4])(
                                    inner, nullptr, position);
                            }
                            else
                            {
                                SceneLoader_ConfigureFocusEntry(
                                    target, nullptr, position);
                            }

                            float angle = static_cast<float>(
                                S_00526d40_GetVectorAngleRadians(
                                    &direction, nullptr) * g_RadiansToDegrees);
                            DispatchSceneObjectWithPayload(target, nullptr, angle);
                        }
                        else
                        {
                            void* transform =
                                ((void*(__fastcall*)(void*, void*))
                                     CSF_AT(void**, entryComponent, 0)[0x18 / 4])(
                                    entryComponent, nullptr);
                            int* entryPosition =
                                ((int*(__fastcall*)(void*, void*))
                                     CSF_AT(void**, transform, 0)[0x10 / 4])(
                                    transform, nullptr);
                            SceneLoader_ConfigureFocusEntry(
                                target, nullptr, entryPosition);

                            transform =
                                ((void*(__fastcall*)(void*, void*))
                                     CSF_AT(void**, referenceComponent, 0)[0x18 / 4])(
                                    referenceComponent, nullptr);
                            float** matrixReference =
                                ((float**(__fastcall*)(void*, void*, void*))
                                     CSF_AT(void**, transform, 0)[0x8C / 4])(
                                    transform, nullptr, rotationMatrixBytes);
                            float* angles = GfxMatrix_GetEulerAngles(
                                matrixReference, nullptr,
                                reinterpret_cast<float*>(eulerBytes));
                            float angle = angles[1] * g_RadiansToDegrees;
                            DispatchSceneObjectWithPayload(target, nullptr, angle);

                            if (CSF_AT(std::uint8_t, rotationMatrixBytes, 0x4))
                                RwMatrixDestroy(
                                    CSF_AT(std::uint32_t*, rotationMatrixBytes, 0));
                        }
                    }
                }
            }
        }

        ++entryIndex;
        entryOffset += 28;
        result = static_cast<int>(entryIndex);
    }
    while (entryIndex < self->field_01a0);

    return result;
}

HOOK(0x0049EEE0, C_008342e8_UpdateFocusEntries, "auto");
