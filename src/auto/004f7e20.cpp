#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, WeaponFirearms_InitializeSceneObjectOptions, 0x004777D0, (uint32_t* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __fastcall, CharacterModel_InitializeTransformFromState, 0x006B10B0, (void* self, void* edx, int a2, int a3, int* a4));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

CSF_GLOBAL(std::uint32_t, g_PreviewCameraAngleBits, 0x00860DAC);

static C_0083f1a0* __fastcall C_0083f1a0_InitializeHeadModel(
    C_0083f1a0* self, void* /*edx*/, int width, int height)
{
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x0083F1A0));
    self->field_0028 = 0;
    self->field_0040 = 0;
    self->field_0074 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0078 = -1;
    self->field_007c = -1;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00d0 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_00d4 = -1;
    self->field_00d8 = -1;
    self->field_00dc = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_00e0 = -1;
    self->field_00e4 = -1;
    self->field_00e8 = 0;
    self->field_00ec = 0;
    self->field_00f0 = 0;
    self->field_00f4 = 0;
    self->field_00f8 = 0;
    self->field_00fc = 0;
    self->field_0110 = 0;
    self->field_0114 = 0;
    self->field_0118 = 0;
    self->field_011c = 0;
    self->field_0120 = 0;
    self->field_0124 = 0;
    self->field_0128 = 10;

    alignas(8) uint8_t previewOptions[0x24] = {};
    std::memset(previewOptions, 0, 16);
    CSF_AT(uint32_t, previewOptions, 0x20) = 0;
    CSF_AT(uint32_t, previewOptions, 0x18) = 0;
    CSF_AT(uint32_t, previewOptions, 0x1C) = g_PreviewCameraAngleBits;
    CSF_AT(int, previewOptions, 0x10) = width;
    CSF_AT(int, previewOptions, 0x14) = height;

    void* rwManager = reinterpret_cast<void*>(ResolveRwManager(GetGlobalInt(), nullptr));
    self->field_0004 =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, rwManager, 0)[0x10 / 4])(
                rwManager, nullptr, previewOptions);

    self->field_0008 = 0;
    self->field_0080 = 1125515264;
    self->field_012c = -1;
    self->field_000c = 1;
    self->field_0014 = -1;
    self->field_0018 = 0;
    self->field_0020 = 0;
    self->field_001c = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00b8 = 0;
    self->field_00bc = 0;
    self->field_00c0 = 0;
    self->field_00c4 = 0;
    self->field_00c8 = 0;
    self->field_00d4 = -1;
    self->field_00d8 = -1;
    self->field_00e0 = -1;
    self->field_00e4 = -1;
    self->field_00e8 = 0;
    self->field_00f4 = 0;
    self->field_0110 = 0;
    self->field_00ec = 0;
    self->field_00f8 = 0;
    self->field_0114 = 0;
    self->field_00cd = 0;
    self->field_00f0 = 0;
    self->field_00fc = 0;
    self->field_0118 = 0;
    self->field_0100 = 0;
    self->field_0104 = 0;
    self->field_0108 = 0;
    self->field_010c = 0;
    self->field_00cc = 1;
    self->field_0130 = 0;
    self->field_0134 = -1082130432;

    alignas(8) uint8_t modelRequest[0x24] = {};
    CSF_AT(const char*, modelRequest, 0x00) =
        CSF_STR(0x0083F1BC, "Models/Char/Cabeza.dff");
    CSF_AT(uint8_t, modelRequest, 0x04) = 0;
    CSF_AT(int, modelRequest, 0x08) = 0;
    CSF_AT(int, modelRequest, 0x0C) = 0;
    CSF_AT(int, modelRequest, 0x10) = 0;
    CSF_AT(int, modelRequest, 0x14) = 0;

    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    self->field_0010 =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, sceneController, 0)[0x14 / 4])(
                sceneController, nullptr, modelRequest);

    // The flags and their associated storage form one contiguous stack object.
    alignas(8) uint8_t sceneOptions[0x4 + 0xB4] = {};
    WeaponFirearms_InitializeSceneObjectOptions(
        reinterpret_cast<uint32_t*>(sceneOptions), nullptr);
    void* sceneObject = reinterpret_cast<void*>(self->field_0010);
    CSF_AT(uint32_t, sceneOptions, 0) =
        (CSF_AT(uint32_t, sceneOptions, 0) & 0xFFFFFE3E) | 0x101;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, sceneObject, 0)[0x10 / 4])(
            sceneObject, nullptr, reinterpret_cast<uint32_t*>(sceneOptions));

    int globalState = GetGlobalState_50D190();
    CharacterModel_InitializeTransformFromState(
        reinterpret_cast<uint8_t*>(self) + 0x40, nullptr, 0, 1065353216,
        reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(globalState) + 196));
    globalState = GetGlobalState_50D190();
    CharacterModel_InitializeTransformFromState(
        reinterpret_cast<uint8_t*>(self) + 0x28, nullptr, 0, 1065353216,
        reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(globalState) + 196));

    self->field_0070 = 2139095039;
    self->field_006c = 2139095039;
    self->field_0068 = 2139095039;
    self->field_0064 = 2139095039;
    const bool hasAllocatedStorage = CSF_AT(uint32_t, sceneOptions, 0x90) >= 0x10;
    self->field_0060 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    if (hasAllocatedStorage && CSF_AT(void*, sceneOptions, 0x7C))
        Mem_FreeDebug(CSF_AT(void*, sceneOptions, 0x7C));

    CSF_AT(uint32_t, sceneOptions, 0x90) = 15;
    CSF_AT(int, sceneOptions, 0x8C) = 0;
    CSF_AT(uint8_t, sceneOptions, 0x7C) = 0;
    DestroySceneObjectOptionsStorage(sceneOptions + 0x4, nullptr);
    return self;
}

HOOK(0x004F7E20, C_0083f1a0_InitializeHeadModel, "auto");
