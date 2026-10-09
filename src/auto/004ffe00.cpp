#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, SceneLoader_DecodeColor, 0x00476430, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, SceneLoader_RefreshRenderState, 0x00520020, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneObjectManager, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void, __fastcall, SceneLoader_SetFogParameters, 0x006B1430, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, SetByteAt0x50, 0x006B1540, (void* self, void* , char value));
CSF_FUNC(int, __fastcall, SceneLoader_SetEnvironmentPreset, 0x006CAF20, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_AddSceneInterval, 0x006CCC80, (void* self, void* edx, void* a2, int a3, int a4, char a5, char a6));
CSF_FUNC(int, __fastcall, SceneLoader_GetRangeManager, 0x00726C40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneEntry, 0x00770690, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ColorWhite, 0x008A7D90);

struct SceneFogParameters
{
    int mode;
    float firstColor[4];
    int firstParameters[3];
    float secondColor[4];
    int secondParameters[2];
};

struct SceneRangeParameters
{
    int type;
    void* name;
    float first;
    float second;
    char enabled;
    char padding[3];
    uint32_t spare[4];
};

static int SceneLoader_FloatBits(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static int __fastcall CSceneLoader_ApplySceneSettings(CSceneLoader* self, void* /*edx*/)
{
    auto* settings = reinterpret_cast<uint8_t*>(GetValue8B0554()) + 84;
    void* clumpManager = reinterpret_cast<void*>(ResolveClumpManager(GetGlobalInt(), nullptr));
    void* objectManager = reinterpret_cast<void*>(
        SceneLoader_GetSceneObjectManager(reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    void* renderSettings =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, objectManager, 0)[64 / 4])
        (objectManager, nullptr);

    SceneLoader_RefreshRenderState(
        reinterpret_cast<void*>(GetGlobalValue_51F740()), nullptr);

    ((void(__fastcall*)(void*, void*, int, unsigned int))
        CSF_AT(void**, renderSettings, 0)[16 / 4])
        (renderSettings, nullptr, 32, CSF_AT(uint8_t, settings, 0x8A0));

    float color[8] = {};
    SceneFogParameters fog = {};

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1E9))
    {
        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, clumpManager, 0)[16 / 4])
            (clumpManager, nullptr, &g_ColorWhite);
    }
    else
    {
        unsigned int packedColor = CSF_AT(uint32_t, settings, 0x8A4);
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = 1.0f;
        SceneLoader_DecodeColor(color, nullptr, packedColor);
        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, clumpManager, 0)[20 / 4])
            (clumpManager, nullptr, color);

        SceneLoader_DecodeColor(color, nullptr, CSF_AT(uint32_t, settings, 0x8A8));
        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, clumpManager, 0)[24 / 4])
            (clumpManager, nullptr, color);
    }

    if (CSF_AT(uint8_t, settings, 0x8BC))
    {
        int firstRed = CSF_AT(uint8_t, settings, 0x8D6);
        int firstGreen = CSF_AT(uint8_t, settings, 0x8D5);
        fog.mode = CSF_AT(int, settings, 0x8C0);
        unsigned int firstBlue = CSF_AT(uint32_t, settings, 0x8D4) & 0xFF;

        color[0] = static_cast<float>(firstRed);
        color[1] = static_cast<float>(firstGreen);
        color[2] = static_cast<float>(firstBlue);
        color[3] = static_cast<float>(CSF_AT(int, settings, 0x8D8));

        fog.firstColor[1] = color[1];
        fog.firstParameters[0] = CSF_AT(int, settings, 0x8DC);
        int secondRed = CSF_AT(uint8_t, settings, 0x8FA);
        fog.firstColor[0] = color[0];
        fog.firstColor[2] = color[2];
        int firstParameter1 = CSF_AT(int, settings, 0x8E0);
        fog.firstColor[3] = color[3];
        int firstParameter2 = CSF_AT(int, settings, 0x8E4);

        color[0] = static_cast<float>(secondRed);
        fog.firstParameters[1] = firstParameter1;
        int secondGreen = CSF_AT(uint8_t, settings, 0x8F9);
        fog.firstParameters[2] = firstParameter2;
        unsigned int secondBlue = CSF_AT(uint32_t, settings, 0x8F8) & 0xFF;
        color[1] = static_cast<float>(secondGreen);
        color[2] = static_cast<float>(secondBlue);
        color[3] = static_cast<float>(CSF_AT(int, settings, 0x8FC));

        fog.secondColor[0] = color[0];
        fog.secondColor[3] = color[3];
        fog.secondColor[1] = color[1];
        fog.secondParameters[0] = CSF_AT(int, settings, 0x900);
        fog.secondColor[2] = color[2];
        fog.secondParameters[1] = CSF_AT(int, settings, 0x904);

        SceneLoader_SetFogParameters(reinterpret_cast<void*>(GetGlobalInt()), nullptr, &fog);
    }

    char fogEnabled = CSF_AT(char, settings, 0x8BC);
    SetByteAt0x50(reinterpret_cast<void*>(GetGlobalInt()), nullptr, fogEnabled);

    auto* entries = CSF_AT(S_004ef780*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC);
    uint32_t begin = entries->field_0008;
    unsigned int count = begin
        ? static_cast<int32_t>(entries->field_000c - begin) / 104
        : 0;

    for (unsigned int i = 0; i < count; ++i)
    {
        void* entry = reinterpret_cast<void*>(SceneLoader_GetSceneEntry(entries, nullptr, i));
        if (CSF_AT(uint8_t, entry, 0x58) & 0x10)
        {
            int mode = CSF_AT(int, entry, 0x60);
            char firstFlag = mode == 0 ? 1 : 0;
            char secondFlag = mode == 0 ? 1 : 0;
            float end = CSF_AT(float, entry, 0x54) + CSF_AT(float, entry, 0x50);
            int startBits = CSF_AT(int, entry, 0x50);
            void* name = C_008310b8_GetSelf(
                reinterpret_cast<C_008310b8*>(static_cast<uint8_t*>(entry) + 48), nullptr);
            SceneLoader_AddSceneInterval(
                objectManager, nullptr, name, startBits, SceneLoader_FloatBits(end),
                firstFlag, secondFlag);
        }
    }

    void* rangeManager = reinterpret_cast<void*>(
        SceneLoader_GetRangeManager(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* rangeCollection =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, rangeManager, 0)[72 / 4])
        (rangeManager, nullptr);

    begin = entries->field_0008;
    count = begin
        ? static_cast<int32_t>(entries->field_000c - begin) / 104
        : 0;

    for (unsigned int i = 0; i < count; ++i)
    {
        void* entry = reinterpret_cast<void*>(SceneLoader_GetSceneEntry(entries, nullptr, i));
        int flags = CSF_AT(int, entry, 0x58);
        if (flags & 4)
        {
            SceneRangeParameters range = {};
            range.name = static_cast<uint8_t*>(entry) + 48;
            float end = CSF_AT(float, entry, 0x54) + CSF_AT(float, entry, 0x50);
            if (end <= g_fZero)
            {
                range.first = end;
                range.second = CSF_AT(float, entry, 0x50);
            }
            else
            {
                range.second = end;
                range.first = CSF_AT(float, entry, 0x50);
            }
            if (flags & 8)
                range.enabled = 1;

            ((void(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, rangeCollection, 0)[16 / 4])
                (rangeCollection, nullptr, &range);
        }
    }

    if (CSF_AT(uint8_t,
            CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC), 0x2C))
    {
        void* scene = reinterpret_cast<void*>(GetValue8B0554());
        SceneLoader_SetEnvironmentPreset(
            objectManager, nullptr,
            CSF_AT(int, CSF_AT(void*, scene, 0x1FDC), 0x30));
    }

    void* scene = reinterpret_cast<void*>(GetValue8B0554());
    uint32_t objectsBegin = CSF_AT(uint32_t, scene, 0x48);
    int objectCount = objectsBegin
        ? static_cast<int32_t>(CSF_AT(uint32_t, scene, 0x4C) - objectsBegin) / 168
        : 0;

    int offset = 0;
    while (objectCount)
    {
        void* currentScene = reinterpret_cast<void*>(GetValue8B0554());
        auto* object = CSF_AT(uint8_t*, currentScene, 0x48) + offset;
        uint32_t capacity = CSF_AT(uint32_t, object, 0x18);
        char* name = capacity < 0x10
            ? reinterpret_cast<char*>(object + 4)
            : CSF_AT(char*, object, 0x04);

        void* loadedObject =
            ((void*(__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, objectManager, 0)[96 / 4])
                (objectManager, nullptr, name);
        if (loadedObject)
        {
            ((void(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, loadedObject, 0)[4 / 4])
                (loadedObject, nullptr, object + 28);
        }
        offset += 168;
        --objectCount;
    }

    int result = reinterpret_cast<int>(self);
    if (self->field_00b6)
    {
        result = GetGlobalState_50D190();
        if (CSF_AT(void*, reinterpret_cast<void*>(result), 0x1C))
        {
            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
            void* controller = CSF_AT(void*, state, 0x1C);
            return ((int(__fastcall*)(void*, void*, float))
                CSF_AT(void**, controller, 0)[68 / 4])
                (controller, nullptr, 10.0f);
        }
    }
    return result;
}

HOOK(0x004FFE00, CSceneLoader_ApplySceneSettings, "auto");
