#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, _rwD3D9VSSetActiveWorldMatrix, 0x0059B990, (int* a1));
CSF_FUNC(int, __cdecl, _rwD3D9VSGetComposedTransformMatrix, 0x0059BA20, (int a1));
CSF_FUNC(int, __cdecl, RwV3dTransformVector, 0x00623070, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __cdecl, RwFrameGetRoot, 0x00623740, (int a1));
CSF_FUNC(float*, __cdecl, RwMatrixInvert, 0x00624950, (float* a1, float* a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, RwD3D9SetVertexShaderConstant, 0x0062C0A0, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwD3D9SetPixelShaderConstant, 0x0062C0F0, (int a1, int a2, int a3));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GfxSkin_GetDistanceOverrideOwner, 0x006B8840, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, GfxWater_GetLightingSource, 0x006D13C0, (void* self, void* edx));
CSF_FUNC(void**, __fastcall, GfxWater_CopyMaterialParameters, 0x00709B20, (void* self, void* edx, void* a2));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

static void* __fastcall GfxWater_SetShaderConstants(float* self, void* /*edx*/, int renderObject)
{
    alignas(8) std::uint8_t composedMatrix[0x40];
    alignas(8) std::uint8_t inverseAndCamera[0x80];
    alignas(8) std::uint8_t parameters[0x8C];
    alignas(8) float pixelDirection[4];
    alignas(8) float sourceDirection[3];
    alignas(8) std::uint8_t matrixReference[0x8]{};
    alignas(8) float worldMatrix[16];
    alignas(8) float cameraAxis[4] = {};
    alignas(8) float position[4];
    alignas(8) float pixelParameters[4];
    alignas(8) float direction[3];
    alignas(8) float constants[4];

    void* owner = reinterpret_cast<void*>(GfxSkin_GetDistanceOverrideOwner(renderObject));
    void* material = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, owner, 0)[2])(owner, nullptr);
    GfxWater_CopyMaterialParameters(parameters, nullptr, material);

    int root = RwFrameGetRoot(
        CSF_AT(int, reinterpret_cast<void*>(renderObject), 0x4));
    RwFrameUpdateObjects(root);
    std::uint32_t* ltm = RwFrameGetLTM(
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(renderObject), 0x4));
    _rwD3D9VSSetActiveWorldMatrix(reinterpret_cast<int*>(ltm));

    for (int i = 0; i < 16; ++i)
        worldMatrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;

    worldMatrix[0] = CSF_AT(float, ltm, 0x00);
    worldMatrix[4] = CSF_AT(float, ltm, 0x04);
    worldMatrix[8] = CSF_AT(float, ltm, 0x08);
    worldMatrix[1] = CSF_AT(float, ltm, 0x10);
    worldMatrix[5] = CSF_AT(float, ltm, 0x14);
    worldMatrix[9] = CSF_AT(float, ltm, 0x18);
    worldMatrix[2] = CSF_AT(float, ltm, 0x20);
    worldMatrix[6] = CSF_AT(float, ltm, 0x24);
    worldMatrix[10] = CSF_AT(float, ltm, 0x28);
    worldMatrix[3] = CSF_AT(float, ltm, 0x30);
    worldMatrix[7] = CSF_AT(float, ltm, 0x34);
    worldMatrix[11] = CSF_AT(float, ltm, 0x38);
    RwD3D9SetVertexShaderConstant(24, reinterpret_cast<int>(worldMatrix), 4);

    _rwD3D9VSGetComposedTransformMatrix(reinterpret_cast<int>(composedMatrix));
    RwD3D9SetVertexShaderConstant(4, reinterpret_cast<int>(composedMatrix), 4);

    constants[0] = CSF_AT(float, self, 0xEC) * g_MessageBoxAnimationScale;
    constants[1] = 0.0f;
    constants[2] = 0.0f;
    constants[3] = 0.0f;
    RwD3D9SetVertexShaderConstant(12, reinterpret_cast<int>(constants), 1);

    void* global = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* provider = GetCachedObjectProvider(CSF_AT(void*, global, 0xF0), nullptr);
    std::uint8_t* cameraMatrix = inverseAndCamera + 0x40;
    std::memcpy(cameraMatrix,
                static_cast<std::uint8_t*>(CSF_AT(void*, provider, 0x4)) + 0x10,
                0x40);

    cameraAxis[0] = CSF_AT(float, cameraMatrix, 0x20);
    cameraAxis[1] = CSF_AT(float, cameraMatrix, 0x24);
    cameraAxis[2] = CSF_AT(float, cameraMatrix, 0x28);
    RwD3D9SetVertexShaderConstant(2, reinterpret_cast<int>(cameraAxis), 1);

    position[0] = CSF_AT(float, cameraMatrix, 0x30);
    position[1] = CSF_AT(float, cameraMatrix, 0x34);
    position[2] = CSF_AT(float, cameraMatrix, 0x38);
    position[3] = 0.0f;
    RwD3D9SetVertexShaderConstant(20, reinterpret_cast<int>(position), 1);

    position[0] = CSF_AT(float, ltm, 0x30);
    position[1] = CSF_AT(float, ltm, 0x34);
    position[2] = CSF_AT(float, ltm, 0x38);
    RwD3D9SetVertexShaderConstant(21, reinterpret_cast<int>(position), 1);

    constants[0] = CSF_AT(float, parameters, 0x28);
    constants[1] = CSF_AT(float, parameters, 0x24) * g_MessageBoxAnimationScale;
    constants[2] = CSF_AT(float, parameters, 0x18);
    constants[3] = CSF_AT(float, parameters, 0x4C);
    RwD3D9SetVertexShaderConstant(22, reinterpret_cast<int>(constants), 1);

    constants[0] = CSF_AT(float, parameters, 0x50) * g_Const200_0f;
    constants[1] = CSF_AT(float, parameters, 0x54) * g_Const200_0f;
    constants[2] = 0.0f;
    constants[3] = 0.0f;
    RwD3D9SetVertexShaderConstant(23, reinterpret_cast<int>(constants), 1);

    hook::Regs colorRegisters{};
    colorRegisters.eax = CSF_AT(std::uint32_t, parameters, 0x4);
    colorRegisters.ecx = reinterpret_cast<std::uint32_t>(constants);
    hook::call_regs(CSF_ADDR_FN(0x00709720), &colorRegisters, nullptr, 0);
    RwD3D9SetPixelShaderConstant(2, reinterpret_cast<int>(constants), 1);

    colorRegisters = {};
    colorRegisters.eax = CSF_AT(std::uint32_t, parameters, 0x0);
    colorRegisters.ecx = reinterpret_cast<std::uint32_t>(constants);
    hook::call_regs(CSF_ADDR_FN(0x00709720), &colorRegisters, nullptr, 0);
    RwD3D9SetPixelShaderConstant(3, reinterpret_cast<int>(constants), 1);

    colorRegisters = {};
    colorRegisters.eax = CSF_AT(std::uint32_t, parameters, 0x8);
    colorRegisters.ecx = reinterpret_cast<std::uint32_t>(constants);
    hook::call_regs(CSF_ADDR_FN(0x00709720), &colorRegisters, nullptr, 0);
    RwD3D9SetPixelShaderConstant(4, reinterpret_cast<int>(constants), 1);

    pixelParameters[2] = g_TransitionOne - CSF_AT(float, parameters, 0x1C);
    pixelParameters[0] = CSF_AT(float, parameters, 0x0C);
    pixelParameters[1] = CSF_AT(float, parameters, 0x10);
    pixelParameters[3] = CSF_AT(float, parameters, 0x2C);
    if (CSF_AT(int, owner, 0xB4) == 1)
        pixelParameters[3] = 0.005f;
    RwD3D9SetPixelShaderConstant(5, reinterpret_cast<int>(pixelParameters), 1);

    pixelParameters[0] = CSF_AT(float, parameters, 0x14);
    pixelParameters[1] = CSF_AT(float, parameters, 0x58);
    int overrideMode = CSF_AT(int, owner, 0xB4);
    pixelParameters[2] = 1.0f;
    if (overrideMode != 0)
    {
        pixelParameters[2] = 0.0f;
        if (CSF_AT(int, owner, 0xB4) != 0)
            CSF_AT(std::uint32_t, pixelParameters, 0xC) = 0xBF800000;
        else
            pixelParameters[3] = 1.0f;
    }
    else
    {
        pixelParameters[3] = 1.0f;
    }
    RwD3D9SetPixelShaderConstant(6, reinterpret_cast<int>(pixelParameters), 1);

    int managerId = GetGlobalInt();
    int manager = ResolveClumpManager(managerId, nullptr);
    void* lightingSource = reinterpret_cast<void*>(
        GfxWater_GetLightingSource(reinterpret_cast<void*>(manager), nullptr));

    direction[0] = 0.60000002f;
    direction[1] = 1.0f;
    direction[2] = 0.60000002f;
    if (lightingSource)
    {
        void* frameProvider = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, lightingSource, 0)[9])(lightingSource, nullptr);
        void* lightMatrix = reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, frameProvider, 0)[35])(frameProvider, nullptr, matrixReference);
        direction[0] = CSF_AT(float, lightMatrix, 0x20);
        direction[1] = CSF_AT(float, lightMatrix, 0x24);
        direction[2] = CSF_AT(float, lightMatrix, 0x28);
        if (CSF_AT(std::uint8_t, matrixReference, 0x4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixReference, 0x0));
    }

    sourceDirection[0] = direction[0];
    sourceDirection[1] = direction[1];
    sourceDirection[2] = direction[2];
    RwMatrixInvert(reinterpret_cast<float*>(inverseAndCamera), reinterpret_cast<float*>(ltm));
    RwV3dTransformVector(reinterpret_cast<int>(direction),
                         reinterpret_cast<int>(sourceDirection),
                         reinterpret_cast<int>(inverseAndCamera));

    pixelDirection[0] = -direction[0];
    pixelDirection[1] = -direction[2];
    pixelDirection[2] = direction[1];
    pixelDirection[3] = 0.0f;
    RwD3D9SetPixelShaderConstant(8, reinterpret_cast<int>(pixelDirection), 1);

    std::uint32_t capacity = CSF_AT(std::uint32_t, parameters, 0x48);
    void* result = reinterpret_cast<void*>(capacity);
    if (capacity >= 0x10)
    {
        result = CSF_AT(void*, parameters, 0x34);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x00709BC0, GfxWater_SetShaderConstants, "auto");
