#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, sub_6B2F50, 0x006B2F50, (void* self, void* edx, char a2));

// Request block handed to the scene controller (0x18 bytes on the original's stack).
struct S_SceneLoadRequest
{
    const char* sceneName;    // +0x00
    char        enabled;      // +0x04
    const char* resourceA;    // +0x08
    const char* resourceB;    // +0x0C
    int         paramA;       // +0x10
    int         paramB;       // +0x14
};

static int __fastcall S_004e9090_LoadMissionScene(S_004e9090* self, void* /*edx*/, int missionIndex, char useAlternateScene)
{
    void* registry = reinterpret_cast<void*>(self->field_1fc8);
    uint8_t* entry = ((uint8_t*(__fastcall*)(void*, void*, int))CSF_AT(void**, registry, 0)[2])(registry, nullptr, missionIndex);

    S_SceneLoadRequest request;

    if (useAlternateScene)
    {
        // MSVC 7.1 std::string: inline buffer when capacity < 16, heap pointer otherwise
        const char* textB = (CSF_AT(uint32_t, entry, 0xA8) < 0x10u)
            ? reinterpret_cast<const char*>(entry + 0x94)
            : CSF_AT(const char*, entry, 0x94);
        const char* textA = (CSF_AT(uint32_t, entry, 0x8C) < 0x10u)
            ? reinterpret_cast<const char*>(entry + 0x78)
            : CSF_AT(const char*, entry, 0x78);
        const char* sceneName = (CSF_AT(uint32_t, entry, 0x70) < 0x10u)
            ? reinterpret_cast<const char*>(entry + 0x5C)
            : CSF_AT(const char*, entry, 0x5C);

        request.sceneName = sceneName;
        int paramB = CSF_AT(int, entry, 0xB0);
        request.resourceB = textB;
        int paramA = CSF_AT(int, entry, 0xAC);
        request.resourceA = textA;
        request.paramA = paramA;
        request.paramB = paramB;
    }
    else
    {
        const char* textA = (CSF_AT(uint32_t, entry, 0x8C) < 0x10u)
            ? reinterpret_cast<const char*>(entry + 0x78)
            : CSF_AT(const char*, entry, 0x78);
        const char* textB = (CSF_AT(uint32_t, entry, 0xA8) < 0x10u)
            ? reinterpret_cast<const char*>(entry + 0x94)
            : CSF_AT(const char*, entry, 0x94);

        if (CSF_AT(int, entry, 0xB4) == 0)
        {
            textA = nullptr;
            textB = nullptr;
        }

        const char* sceneName = (CSF_AT(uint32_t, entry, 0x54) < 0x10u)
            ? reinterpret_cast<const char*>(entry + 0x40)
            : CSF_AT(const char*, entry, 0x40);

        request.sceneName = sceneName;
        int paramA = CSF_AT(int, entry, 0xAC);
        request.resourceB = textB;
        int paramB = CSF_AT(int, entry, 0xB0);
        request.resourceA = textA;
        request.paramA = paramA;
        request.paramB = paramB;
    }

    request.enabled = 1;

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* controller = reinterpret_cast<void*>(SceneLoader_GetSceneController(sceneLoader, nullptr));
    int scene = ((int(__fastcall*)(void*, void*, S_SceneLoadRequest*))CSF_AT(void**, controller, 0)[5])(controller, nullptr, &request);

    if (CSF_AT(int, entry, 0xB4) == 13)
        sub_6B2F50(reinterpret_cast<void*>(scene), nullptr, 1);

    return scene;
}

HOOK(0x004E9B80, S_004e9090_LoadMissionScene, "auto");
