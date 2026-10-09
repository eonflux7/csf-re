#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(uint32_t*, __fastcall, C_0082e6a0_Construct, 0x0041EA30, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0083109c_Construct, 0x00443B10, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, C_008310b8_Construct, 0x004449D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00831120_Construct, 0x004465B0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, C_0083114c_Construct, 0x0044A420, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_008311e8_Construct, 0x0044D3B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_008314f8_Construct, 0x0046ADA0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00831698_Construct, 0x00475720, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, InitZeroedObject, 0x0048AA30, (uint8_t* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_008343a4_Construct, 0x0049FF90, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0083e124_Construct, 0x004DE7F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);
CSF_GLOBAL(void*, g_SceneEntries, 0x00899730);
CSF_GLOBAL(unsigned int, g_SceneCapacity, 0x00899734);
CSF_GLOBAL(unsigned int, g_numScenes, 0x00899738);
CSF_GLOBAL(int, g_SceneArrayGrowth, 0x0089973C);
CSF_GLOBAL(C_008312b0*, g_AuxiliaryScene, 0x008AFC88);
CSF_GLOBAL(uint8_t, g_SceneLoaderMainScenePresent, 0x008AFC8C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint32_t* __fastcall CScene_Construct(
    C_008312b0* self, void* /*edx*/, char auxiliary)
{
    CSF_AT(void*, self, 0x0004) = CSF_VTBL(0x0082D428);
    CSF_AT(void**, self, 0x0000) = CSF_VTBL(0x008312B0);
    CSF_AT(void*, self, 0x0004) = CSF_VTBL(0x00831294);

    CSF_AT(int, self, 0x001C) = 10;
    CSF_AT(uint32_t, self, 0x0010) = 0;
    CSF_AT(uint32_t, self, 0x0014) = 0;
    CSF_AT(uint32_t, self, 0x0018) = 0;
    CSF_AT(int, self, 0x002C) = 10;
    CSF_AT(uint32_t, self, 0x0020) = 0;
    CSF_AT(uint32_t, self, 0x0024) = 0;
    CSF_AT(uint32_t, self, 0x0028) = 0;
    CSF_AT(int, self, 0x003C) = 10;
    CSF_AT(uint32_t, self, 0x0030) = 0;
    CSF_AT(uint32_t, self, 0x0034) = 0;
    CSF_AT(uint32_t, self, 0x0038) = 0;
    CSF_AT(int, self, 0x004C) = 10;
    CSF_AT(uint32_t, self, 0x0040) = 0;
    CSF_AT(uint32_t, self, 0x0044) = 0;
    CSF_AT(uint32_t, self, 0x0048) = 0;
    CSF_AT(int, self, 0x005C) = 10;
    CSF_AT(uint32_t, self, 0x0050) = 0;
    CSF_AT(uint32_t, self, 0x0054) = 0;
    CSF_AT(uint32_t, self, 0x0058) = 0;
    CSF_AT(int, self, 0x006C) = 10;
    CSF_AT(uint32_t, self, 0x0060) = 0;
    CSF_AT(uint32_t, self, 0x0064) = 0;
    CSF_AT(uint32_t, self, 0x0068) = 0;
    CSF_AT(int, self, 0x0080) = 10;
    CSF_AT(uint32_t, self, 0x0074) = 0;
    CSF_AT(uint32_t, self, 0x0078) = 0;
    CSF_AT(uint32_t, self, 0x007C) = 0;
    CSF_AT(int, self, 0x0094) = 10;
    CSF_AT(uint32_t, self, 0x0088) = 0;
    CSF_AT(uint32_t, self, 0x008C) = 0;
    CSF_AT(uint32_t, self, 0x0090) = 0;
    CSF_AT(int, self, 0x00A4) = 10;
    CSF_AT(uint32_t, self, 0x0098) = 0;
    CSF_AT(uint32_t, self, 0x009C) = 0;
    CSF_AT(uint32_t, self, 0x00A0) = 0;
    CSF_AT(uint32_t, self, 0x00CC) = 0;
    CSF_AT(uint32_t, self, 0x00D0) = 0;
    CSF_AT(uint32_t, self, 0x00D4) = 0;
    CSF_AT(int, self, 0x00E4) = 10;
    CSF_AT(uint32_t, self, 0x00D8) = 0;
    CSF_AT(uint32_t, self, 0x00DC) = 0;
    CSF_AT(uint32_t, self, 0x00E0) = 0;
    CSF_AT(int, self, 0x00F4) = 10;
    CSF_AT(uint32_t, self, 0x00E8) = 0;
    CSF_AT(uint32_t, self, 0x00EC) = 0;
    CSF_AT(uint32_t, self, 0x00F0) = 0;
    CSF_AT(uint32_t, self, 0x0100) = 0;
    CSF_AT(uint32_t, self, 0x0104) = 0;
    CSF_AT(uint32_t, self, 0x0108) = 0;
    CSF_AT(uint32_t, self, 0x0110) = 0;
    CSF_AT(uint32_t, self, 0x0114) = 0;
    CSF_AT(uint32_t, self, 0x0118) = 0;
    CSF_AT(int, self, 0x0140) = 10;
    CSF_AT(uint32_t, self, 0x0134) = 0;
    CSF_AT(uint32_t, self, 0x0138) = 0;
    CSF_AT(uint32_t, self, 0x013C) = 0;
    CSF_AT(int, self, 0x0154) = 10;
    CSF_AT(uint32_t, self, 0x0148) = 0;
    CSF_AT(uint32_t, self, 0x014C) = 0;
    CSF_AT(uint32_t, self, 0x0150) = 0;
    CSF_AT(int, self, 0x0168) = 10;
    CSF_AT(uint32_t, self, 0x015C) = 0;
    CSF_AT(uint32_t, self, 0x0160) = 0;
    CSF_AT(uint32_t, self, 0x0164) = 0;
    CSF_AT(int, self, 0x017C) = 10;
    CSF_AT(uint32_t, self, 0x0170) = 0;
    CSF_AT(uint32_t, self, 0x0174) = 0;
    CSF_AT(uint32_t, self, 0x0178) = 0;
    CSF_AT(void*, self, 0x0198) = CSF_VTBL(0x0082D448);
    CSF_AT(int, self, 0x019C) = -1;
    CSF_AT(int, self, 0x01A0) = -1;
    CSF_AT(uint32_t, self, 0x000C) = 0;
    g_SceneLoaderMainScenePresent = 0;

    void* storage = Mem_Alloc(56);
    CSF_AT(uint32_t*, self, 0x0184) =
        storage ? C_00831698_Construct(storage, nullptr) : nullptr;

    if (!auxiliary)
    {
        if (g_numScenes >= g_SceneCapacity)
            GrowButtonArray(&g_SceneEntries, nullptr, g_SceneArrayGrowth);

        CSF_AT(C_008312b0*, g_SceneEntries, 4 * g_numScenes++) = self;
        g_CurrentSceneIndex = g_numScenes - 1;
        if (g_numScenes == 1)
            g_AuxiliaryScene = nullptr;
    }

    g_pCurrentScene = CSF_AT(void*, g_SceneEntries, 4 * g_CurrentSceneIndex);
    CSF_AT(uint32_t, self, 0x0188) = 0;
    CSF_AT(uint32_t, self, 0x018C) = 0;
    CSF_AT(uint32_t, self, 0x0190) = 0;
    CSF_AT(uint32_t, self, 0x0194) = 0;

    storage = Mem_Alloc(212);
    CSF_AT(uint32_t*, self, 0x00BC) =
        storage ? C_008314f8_Construct(storage, nullptr) : nullptr;

    storage = Mem_Alloc(12);
    CSF_AT(uint8_t*, self, 0x00C0) =
        storage ? C_008310b8_Construct(storage, nullptr) : nullptr;

    storage = Mem_Alloc(280);
    CSF_AT(uint32_t*, self, 0x00B0) =
        storage ? C_00831120_Construct(storage, nullptr) : nullptr;

    storage = Mem_Alloc(24);
    CSF_AT(uint32_t*, self, 0x00B4) =
        storage ? C_008311e8_Construct(storage, nullptr) : nullptr;

    storage = Mem_Alloc(44);
    CSF_AT(uint32_t*, self, 0x00B8) =
        storage ? C_0083109c_Construct(storage, nullptr) : nullptr;

    storage = Mem_Alloc(8);
    CSF_AT(uint8_t*, self, 0x00A8) =
        storage ? InitZeroedObject(static_cast<uint8_t*>(storage), nullptr) : nullptr;

    storage = Mem_Alloc(52);
    CSF_AT(uint32_t*, self, 0x00AC) =
        storage ? C_008343a4_Construct(storage, nullptr) : nullptr;

    storage = Mem_Alloc(100);
    CSF_AT(float*, self, 0x00C4) =
        storage ? C_0083114c_Construct(storage, nullptr) : nullptr;

    storage = Mem_Alloc(36);
    CSF_AT(uint32_t*, self, 0x0130) =
        storage ? C_0082e6a0_Construct(storage, nullptr) : nullptr;

    CSF_AT(uint32_t, self, 0x012C) = 0;
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 208))
    {
        storage = Mem_Alloc(68);
        CSF_AT(uint32_t*, self, 0x012C) =
            storage ? C_0083e124_Construct(storage, nullptr) : nullptr;
    }

    if (!auxiliary)
    {
        if (g_numScenes > 1)
        {
            storage = Mem_Alloc(204800);
            CSF_AT(void*, self, 0x018C) = storage;
            CSF_AT(uint32_t, self, 0x0190) = 204800;
        }
        if (g_numScenes == 2)
        {
            storage = Mem_Alloc(204800);
            CSF_AT(void*, CSF_AT(void*, g_SceneEntries, 0), 396) = storage;
            CSF_AT(uint32_t, CSF_AT(void*, g_SceneEntries, 0), 400) = 204800;

            auto* auxiliaryScene = static_cast<C_008312b0*>(Mem_Alloc(436));
            if (auxiliaryScene)
            {
                g_AuxiliaryScene = reinterpret_cast<C_008312b0*>(
                    CScene_Construct(auxiliaryScene, nullptr, 1));
                return reinterpret_cast<uint32_t*>(self);
            }
            g_AuxiliaryScene = nullptr;
        }
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00455F40, CScene_Construct, "auto");
