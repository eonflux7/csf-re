#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static void* __fastcall C_008316d8_InitializeWeapon(
    C_008316d8* self, void* /*edx*/, int definitionId, int owner)
{
    self->vtbl = CSF_VTBL(0x008316D8);
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0014 = 1;
    self->field_0010 = 0.0f;
    self->field_0004 = owner;
    self->field_000c = definitionId;

    void* registry = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC0);
    auto* definition = reinterpret_cast<uint8_t*>(
        reinterpret_cast<void*(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, registry, 0)[0x8 / 4])(
                registry, nullptr, self->field_000c));

    self->field_0040 = reinterpret_cast<uint32_t>(definition);
    self->field_001c = CSF_AT(uint32_t, definition, 0x104);
    self->field_0020 = CSF_AT(uint32_t, definition, 0x108);
    self->field_0024 = CSF_AT(uint32_t, definition, 0x110);
    self->field_0016 = 0;
    self->field_0018 = 0.0f;

    void* modelName = CSF_AT(uint32_t, definition, 0x70) < 0x10u
        ? definition + 0x5C
        : CSF_AT(void*, definition, 0x5C);

    alignas(4) uint8_t lookup[0x18]{};
    CSF_AT(void*, lookup, 0x00) = modelName;
    CSF_AT(uint8_t, lookup, 0x04) = 1;
    CSF_AT(uint32_t, lookup, 0x08) = 0;
    CSF_AT(uint32_t, lookup, 0x0C) = 0;
    CSF_AT(uint32_t, lookup, 0x10) = 0;
    CSF_AT(uint32_t, lookup, 0x14) = 0;

    void* loader = reinterpret_cast<void*>(GetGlobalInt());
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(loader, nullptr));
    auto* sceneObject =
        reinterpret_cast<C_00833908*(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, controller, 0)[0x14 / 4])(
                controller, nullptr, lookup);
    self->field_0008 = reinterpret_cast<uint32_t>(sceneObject);

    if (sceneObject)
    {
        uint8_t ownerOption = CSF_AT(
            uint8_t, CSF_AT(void*, reinterpret_cast<void*>(owner), 0x2C), 0x15B);
        if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0040), 0x74) == 33)
            ownerOption = 0;

        // One contiguous object: the initializer also constructs its embedded strings.
        alignas(4) uint8_t options[0xB8]{};
        InitializeSceneObjectOptions(options, nullptr);
        CSF_AT(uint32_t, options, 0x00) =
            (CSF_AT(uint32_t, options, 0x00) & 0xFFFFBF3E) |
            (ownerOption ? 0x4001 : 1);

        definition = reinterpret_cast<uint8_t*>(self->field_0040);
        CSF_AT(uint32_t, options, 0x68) = 0;
        float boundsDifference[3];
        boundsDifference[0] = CSF_AT(float, definition, 0x1D8) -
                              CSF_AT(float, definition, 0x1E4);
        boundsDifference[1] = CSF_AT(float, definition, 0x1DC) -
                              CSF_AT(float, definition, 0x1E8);
        boundsDifference[2] = CSF_AT(float, definition, 0x1E0) -
                              CSF_AT(float, definition, 0x1EC);
        CSF_AT(float, options, 0x70) = boundsDifference[1];
        CSF_AT(float, options, 0x6C) = boundsDifference[0];
        CSF_AT(float, options, 0x74) = boundsDifference[2];
        CSF_AT(uint32_t, options, 0x94) = 0;

        float* center = SceneMissionSetup_GetBoundsCenter(
            definition + 0x1D8, nullptr, boundsDifference);
        CSF_AT(float, options, 0x98) = center[0];
        definition = reinterpret_cast<uint8_t*>(self->field_0040);
        CSF_AT(float, options, 0x9C) = center[1];
        CSF_AT(float, options, 0xA0) = center[2];
        CSF_AT(uint32_t, options, 0xA4) = 2;
        CSF_AT(uint8_t, options, 0x64) = 1;
        CSF_AT(uint8_t, options, 0x65) = 0;
        CSF_AT(uint8_t, options, 0x66) = 0;

        if (CSF_AT(uint32_t, definition, 0x1F0))
        {
            GetGlobalState_50D190();
            definition = reinterpret_cast<uint8_t*>(self->field_0040);
            CSF_AT(uint32_t, options, 0x04) =
                CSF_AT(uint32_t, definition, 0x1F0);
            AssignStringSubstring(options + 0x08, nullptr,
                                  definition + 0x1F4, 0, -1);
            AssignStringSubstring(
                options + 0x24, nullptr,
                reinterpret_cast<uint8_t*>(self->field_0040) + 0x58, 0, -1);
            definition = reinterpret_cast<uint8_t*>(self->field_0040);
            CSF_AT(uint32_t, options, 0x58) = CSF_AT(uint32_t, definition, 0x21C);
            CSF_AT(uint32_t, options, 0x5C) = CSF_AT(uint32_t, definition, 0x220);
            CSF_AT(uint32_t, options, 0x60) = CSF_AT(uint32_t, definition, 0x224);
        }
        else if (!CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            CSF_AT(uint32_t, options, 0x04) = 2;
            AssignStringSubstring(
                options + 0x24, nullptr,
                reinterpret_cast<uint8_t*>(self->field_0040) + 0x58, 0, -1);
            CSF_AT(uint32_t, options, 0x58) = 1065353216;
            CSF_AT(uint32_t, options, 0x5C) = 1063675494;
            CSF_AT(uint32_t, options, 0x60) = 1058642330;
        }

        sceneObject = reinterpret_cast<C_00833908*>(self->field_0008);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            sceneObject->vtbl[0x10 / 4])(sceneObject, nullptr, options);

        if (CSF_AT(uint32_t, options, 0x90) >= 0x10 &&
            CSF_AT(void*, options, 0x7C))
        {
            Mem_FreeDebug(CSF_AT(void*, options, 0x7C));
        }
        CSF_AT(uint32_t, options, 0x90) = 15;
        CSF_AT(uint32_t, options, 0x8C) = 0;
        CSF_AT(uint8_t, options, 0x7C) = 0;
        DestroySceneObjectOptionsStorage(options + 0x04, nullptr);
    }

    self->field_0044 = -1;
    self->field_0034 = -1;
    self->field_0050 = 1;
    self->field_004c = 1;
    self->field_002c = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    self->field_0048 = 0.0f;
    self->field_0038 = 0;
    self->field_003c = 0.0f;
    self->field_0015 = 0;
    self->field_0054 = -1027080192;
    return self;
}

HOOK(0x00477CF0, C_008316d8_InitializeWeapon, "auto");
