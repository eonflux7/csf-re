#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneMissionSetup_SetBounds, 0x006AB4E0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_InitializeObjectOptions, 0x006B2750, (void* self, void*));
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_NegativeHalfScale, 0x0082D4B8);
CSF_GLOBAL(float, g_CharacterHeightScale, 0x0082D4BC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int EncodeFloatArgument(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static int* __fastcall S_00401b50_AcquireSceneObject(
    S_00401b50* self, void* /*edx*/, int handle)
{
    alignas(8) uint8_t frameBytes[0x6C]{};
    void* options = frameBytes;
    void* bounds = frameBytes + 0x3C;
    void* attachment = frameBytes + 0x64;
    float& scaledWidth = *reinterpret_cast<float*>(frameBytes + 0x60);

    int* object;
    if (self->field_0020)
    {
        object = CSF_AT(int**, self, 0x18)[self->field_0020 - 1];
        --self->field_0020;
    }
    else
    {
        void* sceneObject = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
        void* geometry = C_0082dd78_DispatchReturnedObject(
            CSF_AT(void*, sceneObject, 0x4C), nullptr);

        float* dimensions =
            reinterpret_cast<float* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, geometry, 0)[0x30 / 4])(geometry, nullptr);

        const float width = dimensions[0];
        const float height = dimensions[1];
        const float depth = dimensions[2];
        scaledWidth = width * g_CharacterHeightScale;
        CSF_AT(float, attachment, 0) = height * g_CharacterHeightScale;
        CSF_AT(float, attachment, 4) = depth * g_CharacterHeightScale;

        SceneMissionSetup_InitializeObjectOptions(options, nullptr);
        CSF_AT(uint32_t, options, 0) = 4;

        const float negativeDepth =
            CSF_AT(float, attachment, 4) * g_NegativeHalfScale;
        const float positiveDepth =
            CSF_AT(float, attachment, 4) * g_HalfDurationScale;
        const float negativeWidth = scaledWidth * g_NegativeHalfScale;
        const float positiveWidth = scaledWidth * g_HalfDurationScale;

        SceneMissionSetup_SetBounds(
            bounds, nullptr,
            EncodeFloatArgument(positiveWidth),
            EncodeFloatArgument(negativeWidth),
            CSF_AT(int, attachment, 0),
            0,
            EncodeFloatArgument(positiveDepth),
            EncodeFloatArgument(negativeDepth));

        void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
        void* factory = reinterpret_cast<void*>(
            RenderHandle_FromObject(globalObject, nullptr));
        object = reinterpret_cast<int* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, factory, 0)[0x2C / 4])(
                factory, nullptr, options);

        DestroySceneObjectOptionsStorage(options, nullptr);
    }

    if (object)
    {
        CSF_AT(uint32_t, attachment, 0) = 0;
        CSF_AT(uint8_t, attachment, 4) = 0;
        CSF_AT(uint8_t, attachment, 5) = 0;

        void* sceneObject = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
        CSF_AT(int, attachment, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, sceneObject, 0)[0x28 / 4])(
                    sceneObject, nullptr);

        CSF_AT(uint8_t, attachment, 5) = 1;
        CSF_AT(uint8_t, attachment, 4) = 0;
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, object, 0)[0x14 / 4])(
                object, nullptr, attachment);
    }

    return object;
}

HOOK(0x00401B50, S_00401b50_AcquireSceneObject, "auto");
