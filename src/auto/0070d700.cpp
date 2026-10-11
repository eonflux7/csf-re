#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwD3D9SetTransform, 0x0062BA10, (int a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RwD3D9GetTransform, 0x0062BB70, (int a1, void* a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, GfxMatrix_SetFourRowsAndUpdate, 0x006ADA10, (S_006ada10* self, void* , uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(char, __fastcall, GfxWater_RenderSceneWithCamera, 0x006B1910, (void* self, void* edx, int a2, void* a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void, __fastcall, SkinGeom_InitFromSource, 0x006C5A40, (void* self, void* , int sourceHandle, char flagByte));
CSF_FUNC(char, __fastcall, S_006c5cd0_ResetAndCommitState, 0x006C7A20, (S_006c5cd0* self, void*));
CSF_FUNC(int, __fastcall, GfxWater_SetCameraViewport, 0x006D29F0, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __fastcall, C_008655e8_EndCameraUpdate, 0x006D2BE0, (C_008655e8* self, void*));
CSF_FUNC(bool, __fastcall, GfxSkin_BeginCameraUpdate, 0x006D3010, (void* self, void* edx, int arg0, float a3, float a2, float a5, int a6));
CSF_FUNC(int, __fastcall, GfxWater_ConstructCamera, 0x006D32F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxWater_ConfigureCamera, 0x006D3450, (C_008655e8* self, void* edx, int parameters, uint32_t aspectRatio));
CSF_FUNC(int, __fastcall, GfxWater_GetTargetDimension, 0x006D59D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxWater_GetRenderTarget, 0x006D5FD0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxWater_RenderSceneContents, 0x006EF4F0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GfxWater_GetCameraAspectRatio, 0x006F2860, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, S_006fdbb0_GetPosition, 0x006FDBB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxWater_TransformClipPlane, 0x0070CBB0, (void* self, void* edx, int* arg0, int a3));
CSF_FUNC(double, __cdecl, GfxWater_GetPlaneComponentSign, 0x0070CCD0, (float a1));
CSF_FUNC(char, __fastcall, S_006fdbb0_RefreshRenderTarget, 0x0070D5C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, _rand, 0x007B04E7, ());

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_WaterCameraNearClip, 0x00860DA4);
CSF_GLOBAL(float, g_WaterCameraFarClip, 0x00860DA8);
CSF_GLOBAL(std::uint32_t, g_PreviewCameraAngleBits, 0x00860DAC);

static std::uint32_t GfxWater_GetFloatBits(float value)
{
    std::uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static int __fastcall S_006fdbb0_RenderReflection(
    S_006fdbb0* self, void* /*edx*/, int* sourceCamera)
{
    if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            reinterpret_cast<void**>(self->field_0000)[56 / 4])(self, nullptr)
        || CSF_AT(std::uint32_t, self, 0xFC)
            != CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_00f8), 0x98))
    {
        CSF_AT(std::uint32_t, self, 0xFC) =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_00f8), 0x98);
        S_006fdbb0_RefreshRenderTarget(self, nullptr);
    }

    int sceneController = SceneLoader_GetSceneController(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    CSF_AT(std::uint8_t, reinterpret_cast<void*>(sceneController), 0x95) = 0;

    if (!CSF_AT(std::uint8_t, self, 0xCC)
        && !CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_00f8), 0x8C))
    {
        void* oldCamera = reinterpret_cast<void*>(self->field_00d8);
        CSF_AT(std::uint8_t, self, 0xCC) = 1;
        if (oldCamera)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, oldCamera, 0)[4 / 4])(oldCamera, nullptr, 1);
            self->field_00d8 = 0;
        }

        void* cameraMemory = Mem_Alloc(228);
        self->field_00d8 = cameraMemory
            ? GfxWater_ConstructCamera(cameraMemory, nullptr) : 0;

        int rwManager = ResolveRwManager(GetGlobalInt(), nullptr);

        alignas(8) std::uint8_t clipBytes[0x2C];
        float* clip = reinterpret_cast<float*>(clipBytes);
        float* cameraParameters = clip + 2;
        float* plane = clip + 6;
        cameraParameters[0] = 0.0f;
        cameraParameters[1] = 0.0f;
        cameraParameters[2] = 0.0f;
        cameraParameters[3] = 0.0f;
        plane[0] = g_WaterCameraNearClip;
        plane[1] = g_WaterCameraFarClip;
        plane[2] = 0.0f;
        std::memcpy(plane + 3, &g_PreviewCameraAngleBits, sizeof(float));
        CSF_AT(int, clipBytes, 0x28) = 0;

        float aspectRatio = static_cast<float>(
            GfxWater_GetCameraAspectRatio(reinterpret_cast<void*>(rwManager), nullptr));
        GfxWater_ConfigureCamera(
            reinterpret_cast<C_008655e8*>(self->field_00d8), nullptr,
            reinterpret_cast<int>(cameraParameters), GfxWater_GetFloatBits(aspectRatio));

        alignas(8) std::uint8_t matrixBytes[0xC];
        S_006adb40& reflectedMatrix = *reinterpret_cast<S_006adb40*>(matrixBytes);
        reflectedMatrix.field_0000 = reinterpret_cast<std::uint32_t>(RwMatrixCreate());
        reflectedMatrix.pad_0004[0] = 1;

        alignas(8) std::uint8_t temporaryBytes[0xC];
        alignas(8) std::uint8_t rightBytes[0xC];
        alignas(8) std::uint8_t forwardBytes[0xC];
        alignas(8) std::uint8_t upBytes[0xC];
        alignas(8) std::uint8_t positionBytes[0xC];
        float* right = reinterpret_cast<float*>(rightBytes);
        float* forward = reinterpret_cast<float*>(forwardBytes);
        float* up = reinterpret_cast<float*>(upBytes);
        float* position = reinterpret_cast<float*>(positionBytes);

        if (!CSF_AT(std::uint32_t, self, 0xB4))
        {
            void* frame = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sourceCamera, 0)[16 / 4])(sourceCamera, nullptr);
            void* matrixResult = reinterpret_cast<void*(__fastcall*)(void*, void*, int*)>(
                CSF_AT(void**, frame, 0)[140 / 4])(
                    frame, nullptr, reinterpret_cast<int*>(temporaryBytes));
            float* sourceMatrix = CSF_AT(float*, matrixResult, 0);
            right[0] = -sourceMatrix[0];
            right[1] = -sourceMatrix[1];
            right[2] = -sourceMatrix[2];
            if (CSF_AT(std::uint8_t, temporaryBytes, 4))
                RwMatrixDestroy(CSF_AT(std::uint32_t*, temporaryBytes, 0));

            frame = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sourceCamera, 0)[16 / 4])(sourceCamera, nullptr);
            matrixResult = reinterpret_cast<void*(__fastcall*)(void*, void*, int*)>(
                CSF_AT(void**, frame, 0)[140 / 4])(
                    frame, nullptr, reinterpret_cast<int*>(temporaryBytes));
            sourceMatrix = CSF_AT(float*, matrixResult, 0);
            up[0] = CSF_AT(float, sourceMatrix, 0x20);
            up[1] = CSF_AT(float, sourceMatrix, 0x24);
            up[2] = CSF_AT(float, sourceMatrix, 0x28);
            if (CSF_AT(std::uint8_t, temporaryBytes, 4))
                RwMatrixDestroy(CSF_AT(std::uint32_t*, temporaryBytes, 0));

            up[1] = -up[1];
            float crossZ = right[1] * up[0] - right[0] * up[1];
            CSF_AT(float, temporaryBytes, 4) = crossZ;
            forward[0] = right[2] * up[1] - up[2] * right[1];
            forward[1] = up[2] * right[0] - right[2] * up[0];
            forward[2] = crossZ;

            frame = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sourceCamera, 0)[16 / 4])(sourceCamera, nullptr);
            matrixResult = reinterpret_cast<void*(__fastcall*)(void*, void*, int*)>(
                CSF_AT(void**, frame, 0)[140 / 4])(
                    frame, nullptr, reinterpret_cast<int*>(temporaryBytes));
            sourceMatrix = CSF_AT(float*, matrixResult, 0);
            position[0] = CSF_AT(float, sourceMatrix, 0x30);
            position[1] = CSF_AT(float, sourceMatrix, 0x34);
            position[2] = CSF_AT(float, sourceMatrix, 0x38);
            if (CSF_AT(std::uint8_t, temporaryBytes, 4))
                RwMatrixDestroy(CSF_AT(std::uint32_t*, temporaryBytes, 0));

            (void)CSF_VTBL(0x006FDBB0);
            char* waterPosition = S_006fdbb0_GetPosition(self, nullptr);
            position[1] = CSF_AT(float, waterPosition, 4)
                - (position[1] - CSF_AT(float, waterPosition, 4));

            (void)CSF_VTBL(0x006ADA10);
            GfxMatrix_SetFourRowsAndUpdate(
                reinterpret_cast<S_006ada10*>(matrixBytes), nullptr,
                reinterpret_cast<std::uint32_t*>(rightBytes),
                reinterpret_cast<std::uint32_t*>(forwardBytes),
                reinterpret_cast<std::uint32_t*>(upBytes),
                reinterpret_cast<std::uint32_t*>(positionBytes));

            void* reflectionCamera = reinterpret_cast<void*>(self->field_00d8);
            frame = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, reflectionCamera, 0)[16 / 4])(reflectionCamera, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, S_006adb40*)>(
                CSF_AT(void**, frame, 0)[148 / 4])(frame, nullptr, &reflectedMatrix);
        }
        else
        {
            void* frame = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sourceCamera, 0)[16 / 4])(sourceCamera, nullptr);
            void* matrixResult = reinterpret_cast<void*(__fastcall*)(void*, void*, int*)>(
                CSF_AT(void**, frame, 0)[140 / 4])(
                    frame, nullptr, reinterpret_cast<int*>(temporaryBytes));
            std::memcpy(reinterpret_cast<void*>(reflectedMatrix.field_0000),
                        CSF_AT(const void*, matrixResult, 0), 0x40);
            if (CSF_AT(std::uint8_t, temporaryBytes, 4))
                RwMatrixDestroy(CSF_AT(std::uint32_t*, temporaryBytes, 0));

            void* reflectionCamera = reinterpret_cast<void*>(self->field_00d8);
            frame = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, reflectionCamera, 0)[16 / 4])(reflectionCamera, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, S_006adb40*)>(
                CSF_AT(void**, frame, 0)[148 / 4])(frame, nullptr, &reflectedMatrix);
        }

        clip[0] = 0.0f;
        clip[1] = 0.0f;
        clip[2] = 0.0f;
        clip[3] = g_TransitionOne;
        clip[4] = 0.0f;
        clip[5] = 0.0f;
        CSF_AT(std::uint8_t, clipBytes, 0x28) = 1;

        alignas(8) std::uint8_t inverseBytes[0x8];
        CSF_AT(float*, inverseBytes, 0) = reinterpret_cast<float*>(RwMatrixCreate());
        CSF_AT(std::uint8_t, inverseBytes, 4) = 1;
        (void)CSF_VTBL(0x006ADB40);
        Matrix_InvertWithStoredTarget(
            &reflectedMatrix, nullptr, reinterpret_cast<float**>(inverseBytes));

        (void)CSF_VTBL(0x006FDBB0);
        char* waterPosition = S_006fdbb0_GetPosition(self, nullptr);
        GfxWater_TransformClipPlane(
            waterPosition, nullptr, reinterpret_cast<int*>(inverseBytes),
            reinterpret_cast<int>(clipBytes));
        if (CSF_AT(std::uint8_t, clipBytes, 0x28))
        {
            plane[0] = clip[3];
            plane[1] = clip[4];
            plane[2] = clip[5];
            plane[3] = -(clip[3] * clip[0] + clip[5] * clip[2] + clip[4] * clip[1]);
        }

        int targetHeight = GfxWater_GetTargetDimension(
            reinterpret_cast<void*>(self->field_00f8), nullptr);
        int targetWidth = GfxWater_GetTargetDimension(
            reinterpret_cast<void*>(self->field_00f8), nullptr);
        GfxWater_SetCameraViewport(
            reinterpret_cast<void*>(self->field_00d8), nullptr,
            0, 0, targetWidth, targetHeight);

        void** cameraVtable = CSF_AT(void**, reinterpret_cast<void*>(self->field_00d8), 0);
        float cameraValue = static_cast<float>(
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sourceCamera, 0)[36 / 4])(sourceCamera, nullptr));
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(cameraVtable[32 / 4])(
            reinterpret_cast<void*>(self->field_00d8), nullptr, cameraValue);

        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(self->field_00d8), nullptr);
        int savedTarget = CSF_AT(int, provider, 0x60);
        std::uint32_t savedTargetFlags = CSF_AT(std::uint32_t, provider, 0x64);
        CSF_AT(int, provider, 0x60) = GfxWater_GetRenderTarget(
            reinterpret_cast<void*>(self->field_00f8), nullptr);
        CSF_AT(std::uint32_t, provider, 0x64) = self->field_00f4;

        float randomBlue = static_cast<float>(_rand());
        float randomGreen = static_cast<float>(_rand());
        float randomRed = static_cast<float>(_rand());
        float randomAlpha = static_cast<float>(_rand());

        if (GfxSkin_BeginCameraUpdate(
                reinterpret_cast<void*>(self->field_00d8), nullptr,
                3, randomAlpha, randomRed, randomGreen,
                static_cast<int>(GfxWater_GetFloatBits(randomBlue))))
        {
            int cameraHandle = self->field_00d8;
            void* renderState = reinterpret_cast<void*>(GetGlobalValue_6C5760());
            SkinGeom_InitFromSource(renderState, nullptr, cameraHandle, 0);

            sceneController = SceneLoader_GetSceneController(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr);
            if (CSF_AT(int, reinterpret_cast<void*>(sceneController), 0x64))
            {
                sceneController = SceneLoader_GetSceneController(
                    reinterpret_cast<void*>(GetGlobalInt()), nullptr);
                GfxWater_RenderSceneContents(
                    CSF_AT(void*, reinterpret_cast<void*>(sceneController), 0x64), nullptr);
            }
            S_006c5cd0_ResetAndCommitState(
                reinterpret_cast<S_006c5cd0*>(GetGlobalValue_6C5760()), nullptr);
            C_008655e8_EndCameraUpdate(
                reinterpret_cast<C_008655e8*>(self->field_00d8), nullptr);
        }

        if (CSF_AT(int, reinterpret_cast<void*>(self->field_00f8), 0x98) != 2)
        {
            int savedRenderFlags = CSF_AT(
                int, reinterpret_cast<void*>(GetGlobalValue_6C5760()), 8);
            int renderFlags = (savedRenderFlags & 0x400) | 0x100;
            if (!CSF_AT(int, reinterpret_cast<void*>(self->field_00f8), 0x98))
                renderFlags = (savedRenderFlags & 0x400) | 0x919;
            CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_6C5760()), 8) = renderFlags;

            if (GfxSkin_BeginCameraUpdate(
                    reinterpret_cast<void*>(self->field_00d8), nullptr,
                    0, 0.0f, 0.0f, 0.0f, 0))
            {
                alignas(8) std::uint8_t projectionBytes[0x50];
                float* projection = reinterpret_cast<float*>(projectionBytes);
                RwD3D9GetTransform(3, projectionBytes);

                float cornerX = static_cast<float>(
                    GfxWater_GetPlaneComponentSign(plane[0]) / projection[0]);
                double cornerY = GfxWater_GetPlaneComponentSign(plane[1]);
                float scale = static_cast<float>(g_TransitionOne /
                    (plane[3] * ((g_TransitionOne - projection[10]) / projection[14])
                     + plane[1] * (cornerY / projection[5])
                     + plane[0] * cornerX + plane[2]));

                projection[10] = plane[2] * scale;
                projection[14] = plane[3] * scale;
                projection[2] = plane[0] * scale;
                projection[6] = plane[1] * scale;
                RwD3D9SetTransform(3, reinterpret_cast<std::uint32_t*>(projectionBytes));

                int cameraHandle = self->field_00d8;
                int globalManager = GetGlobalInt();
                GfxWater_RenderSceneWithCamera(
                    reinterpret_cast<void*>(globalManager), nullptr, cameraHandle, nullptr);
                C_008655e8_EndCameraUpdate(
                    reinterpret_cast<C_008655e8*>(self->field_00d8), nullptr);
            }
            CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_6C5760()), 8) = savedRenderFlags;
        }

        CSF_AT(int, provider, 0x60) = savedTarget;
        CSF_AT(std::uint32_t, provider, 0x64) = savedTargetFlags;

        void* reflectionCamera = reinterpret_cast<void*>(self->field_00d8);
        if (reflectionCamera)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, reflectionCamera, 0)[4 / 4])(reflectionCamera, nullptr, 1);
            self->field_00d8 = 0;
        }
        CSF_AT(std::uint8_t, self, 0xCC) = 0;
        if (CSF_AT(std::uint8_t, inverseBytes, 4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, inverseBytes, 0));
        if (reflectedMatrix.pad_0004[0])
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(reflectedMatrix.field_0000));
    }

    sceneController = SceneLoader_GetSceneController(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    CSF_AT(std::uint8_t, reinterpret_cast<void*>(sceneController), 0x95) = 1;
    return sceneController;
}

HOOK(0x0070D700, S_006fdbb0_RenderReflection, "auto");
