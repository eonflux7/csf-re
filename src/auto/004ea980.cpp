#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_ApplySceneEvent, 0x0048E2B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x00490D20, (C_00833908* self, void* edx, uint32_t a2, char a3));
CSF_FUNC(uint32_t, __fastcall, C_00833908_SetInitialState, 0x00490D70, (C_00833908* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetHealthMode, 0x00490E70, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_00833908_SetInitialOrientation, 0x00490F60, (void* self, void* edx, int a2, float a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_SetControlMode, 0x00491050, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetSecondarySceneObject, 0x00494930, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_00834640_ConstructForCharacter, 0x004A5F50, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_004e9090_CreateCharacterSceneObject, 0x004E9B80, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __fastcall, C_00833908_SetOwnedContext, 0x0055D860, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, SceneMissionSetup_SetBounds, 0x006AB4E0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_SetBoundsCenter, 0x006AB5C0, (void* self, void* edx, float* a2));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_NegativeHalfScale, 0x0082D4B8);
CSF_GLOBAL(float, g_CharacterHeightScale, 0x0082D4BC);
CSF_GLOBAL(float, g_CharacterWidthScale, 0x0082E060);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);

static int SceneMissionSetup_FloatArgument(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

// Both option records have the same layout, including their embedded strings
// and bounding box. The initializer constructs their string storage.
static void SceneMissionSetup_FillCharacterOptions(
    void* options, void* definition, uint32_t flags)
{
    InitializeSceneObjectOptions(options, nullptr);
    CSF_AT(uint32_t, options, 0x00) |= flags;

    if (CSF_AT(uint8_t, definition, 0x159))
        CSF_AT(uint32_t, options, 0x00) |= 0x400;
    else
        CSF_AT(uint32_t, options, 0x00) &= 0xFFFFFBFF;

    if (CSF_AT(uint8_t, definition, 0x15A))
        CSF_AT(uint32_t, options, 0x00) &= 0xFFFFFFDF;
    else
        CSF_AT(uint32_t, options, 0x00) |= 0x20;

    if (CSF_AT(uint8_t, definition, 0x15B))
        CSF_AT(uint32_t, options, 0x00) |= 0x4000;
    else
        CSF_AT(uint32_t, options, 0x00) &= 0xFFFFBFFF;

    CSF_AT(uint32_t, options, 0x68) = CSF_AT(uint32_t, definition, 0xCC);
    CSF_AT(uint32_t, options, 0x6C) = CSF_AT(uint32_t, definition, 0x160);
    CSF_AT(uint32_t, options, 0x70) = CSF_AT(uint32_t, definition, 0x164);
    CSF_AT(uint32_t, options, 0x74) = CSF_AT(uint32_t, definition, 0x168);

    AssignStringSubstring(static_cast<uint8_t*>(options) + 0x78, nullptr,
                          static_cast<uint8_t*>(definition) + 0x16C, 0, -1);

    CSF_AT(uint32_t, options, 0x94) =
        CSF_AT(uint32_t, definition, 0x188) - 1;
    CSF_AT(uint32_t, options, 0x98) = CSF_AT(uint32_t, definition, 0x18C);
    CSF_AT(uint32_t, options, 0x9C) = CSF_AT(uint32_t, definition, 0x190);
    CSF_AT(uint32_t, options, 0xA0) = CSF_AT(uint32_t, definition, 0x194);
    CSF_AT(uint32_t, options, 0xA4) = CSF_AT(uint32_t, definition, 0xD0);

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    CSF_AT(int, options, 0x04) = CSF_AT(uint8_t, state, 0xD0) ? 0 : 4;
}

static void SceneMissionSetup_ClearOptionString(void* options)
{
    if (CSF_AT(uint32_t, options, 0x90) >= 0x10 &&
        CSF_AT(uint32_t, options, 0x7C))
    {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, options, 0x7C)));
    }
    CSF_AT(uint32_t, options, 0x90) = 15;
    CSF_AT(uint32_t, options, 0x8C) = 0;
    CSF_AT(uint8_t, options, 0x7C) = 0;
}

static unsigned int __fastcall S_004e9090_ConfigureCharacterSceneObjects(
    S_004e9090* self, void* /*edx*/, C_00833908* character, int index)
{
    void* definitions = reinterpret_cast<void*>(self->field_1fc8);
    void* definition =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, definitions, 0)[8 / 4])(
                definitions, nullptr, index);

    int primaryObject =
        S_004e9090_CreateCharacterSceneObject(self, nullptr, index, 0);
    C_00833908_SetDoorStateFlags(character, nullptr, primaryObject, 1);
    character->field_0078 = 0;

    alignas(4) uint8_t primaryOptions[0xB8] = {};
    SceneMissionSetup_FillCharacterOptions(primaryOptions, definition, 9);

    float width = CSF_AT(float, definition, 0x160) * g_CharacterWidthScale;
    float height = CSF_AT(float, definition, 0x164) * g_CharacterHeightScale;
    float depth = CSF_AT(float, definition, 0x168) * g_CharacterWidthScale;
    float minimumDepth = g_NegativeHalfScale * depth;
    float maximumDepth = depth * g_HalfDurationScale;
    float minimumWidth = g_NegativeHalfScale * width;
    float maximumWidth = width * g_HalfDurationScale;
    float minimumHeight = height * g_SoundActivityThreshold;

    SceneMissionSetup_SetBounds(
        primaryOptions + 0x40, nullptr,
        SceneMissionSetup_FloatArgument(maximumWidth),
        SceneMissionSetup_FloatArgument(minimumWidth),
        SceneMissionSetup_FloatArgument(height),
        SceneMissionSetup_FloatArgument(minimumHeight),
        SceneMissionSetup_FloatArgument(maximumDepth),
        SceneMissionSetup_FloatArgument(minimumDepth));

    float centerStorage[3] = {};
    float* center = SceneMissionSetup_GetBoundsCenter(
        primaryOptions + 0x40, nullptr, centerStorage);
    float translatedZ = CSF_AT(float, definition, 0x194) + center[2];
    float translatedY = CSF_AT(float, definition, 0x190) + center[1];
    float translatedCenter[3] = {
        CSF_AT(float, definition, 0x18C) + center[0],
        translatedY,
        translatedZ
    };
    SceneMissionSetup_SetBoundsCenter(
        primaryOptions + 0x40, nullptr, translatedCenter);

    AssignStringSubstring(primaryOptions + 0x24, nullptr,
                          static_cast<uint8_t*>(definition) + 0x3C, 0, -1);
    CSF_AT(float, primaryOptions, 0x60) = 1.0f;
    CSF_AT(float, primaryOptions, 0x5C) = 0.5f;
    CSF_AT(uint8_t, primaryOptions, 0x66) = 1;
    CSF_AT(uint8_t, primaryOptions, 0x64) = 0;
    CSF_AT(uint8_t, primaryOptions, 0x65) = 1;

    void* primary = reinterpret_cast<void*>(primaryObject);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, primary, 0)[0x10 / 4])(
            primary, nullptr, primaryOptions);

    void* thirdName = CSF_AT(uint32_t, definition, 0xA8) < 0x10u
        ? static_cast<uint8_t*>(definition) + 0x94
        : reinterpret_cast<void*>(CSF_AT(uint32_t, definition, 0x94));
    void* secondName = CSF_AT(uint32_t, definition, 0x8C) < 0x10u
        ? static_cast<uint8_t*>(definition) + 0x78
        : reinterpret_cast<void*>(CSF_AT(uint32_t, definition, 0x78));
    void* firstName = CSF_AT(uint32_t, definition, 0x70) < 0x10u
        ? static_cast<uint8_t*>(definition) + 0x5C
        : reinterpret_cast<void*>(CSF_AT(uint32_t, definition, 0x5C));

    alignas(4) uint8_t creationOptions[0x24] = {};
    CSF_AT(void*, creationOptions, 0x00) = firstName;
    CSF_AT(float, creationOptions, 0x10) = CSF_AT(float, definition, 0xAC);
    CSF_AT(uint8_t, creationOptions, 0x04) = 1;
    CSF_AT(void*, creationOptions, 0x08) = secondName;
    CSF_AT(float, creationOptions, 0x14) = CSF_AT(float, definition, 0xB0);
    CSF_AT(void*, creationOptions, 0x0C) = thirdName;

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* controller =
        reinterpret_cast<void*>(SceneLoader_GetSceneController(global, nullptr));
    int secondaryObject =
        reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, controller, 0)[0x14 / 4])(
                controller, nullptr, creationOptions);

    alignas(4) uint8_t secondaryOptions[0xB8] = {};
    SceneMissionSetup_FillCharacterOptions(secondaryOptions, definition, 5);
    SceneMissionSetup_SetBounds(
        secondaryOptions + 0x40, nullptr,
        SceneMissionSetup_FloatArgument(maximumWidth),
        SceneMissionSetup_FloatArgument(minimumWidth),
        SceneMissionSetup_FloatArgument(height),
        0,
        SceneMissionSetup_FloatArgument(maximumDepth),
        SceneMissionSetup_FloatArgument(minimumDepth));

    AssignStringSubstring(secondaryOptions + 0x24, nullptr,
                          static_cast<uint8_t*>(definition) + 0x3C, 0, -1);
    CSF_AT(uint8_t, secondaryOptions, 0x66) = 1;
    CSF_AT(uint8_t, secondaryOptions, 0x64) = 0;
    CSF_AT(uint8_t, secondaryOptions, 0x65) = 1;

    void* secondary = reinterpret_cast<void*>(secondaryObject);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, secondary, 0)[0x10 / 4])(
            secondary, nullptr, secondaryOptions);

    C_00833908_SetSecondarySceneObject(character, nullptr, secondaryObject);
    C_00833908_SetInitialOrientation(
        character, nullptr, 0, CSF_AT(float, definition, 0x120),
        CSF_AT(int, definition, 0x124), CSF_AT(int, definition, 0x128));
    C_00833908_SetInitialState(character, nullptr, 0);

    void* contextStorage = Mem_Alloc(172);
    void* context = contextStorage
        ? C_00834640_ConstructForCharacter(
              contextStorage, nullptr, reinterpret_cast<int>(character))
        : nullptr;
    C_00833908_SetOwnedContext(character, nullptr, context);

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (CSF_AT(uint8_t, state, 0xD0))
        C_00833908_SetHealthMode(
            character, nullptr, 6, CSF_AT(int, definition, 0xDC));
    else
        C_00833908_SetHealthMode(
            character, nullptr, 1, CSF_AT(int, definition, 0xDC));

    int initialHealth = CSF_AT(int, definition, 0xE0);
    if (!initialHealth)
        initialHealth = CSF_AT(int, definition, 0xDC);
    float health = static_cast<float>(initialHealth);
    std::memcpy(&character[1].field_016c, &health, sizeof(health));
    std::memcpy(&character[1].field_0168, &health, sizeof(health));

    C_00833908_SetControlMode(character, nullptr, 2);

    alignas(4) uint8_t event[0x18] = {};
    CSF_AT(void**, event, 0x00) = CSF_VTBL(0x0083ED10);
    CSF_AT(int, event, 0x04) = 1;
    C_00833908_ApplySceneEvent(character, nullptr, 1, reinterpret_cast<int>(event));

    SceneMissionSetup_ClearOptionString(secondaryOptions);
    DestroySceneObjectOptionsStorage(secondaryOptions + 0x04, nullptr);
    SceneMissionSetup_ClearOptionString(primaryOptions);
    return DestroySceneObjectOptionsStorage(primaryOptions + 0x04, nullptr);
}

HOOK(0x004EA980, S_004e9090_ConfigureCharacterSceneObjects, "auto");
