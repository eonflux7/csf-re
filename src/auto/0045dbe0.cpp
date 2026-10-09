#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee bindings (addresses from the packet).
CSF_FUNC(uint32_t*, __fastcall, SceneRequest_Setup, 0x00446100,
         (void* self, void* edx, int a2, uint32_t* a3, int a4, int a5));
CSF_FUNC(unsigned int*, __fastcall, SceneRequest_Submit, 0x00446F10,
         (void* self, void* edx, int a2));

// Current scene object global.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Maps the mode code read from the request to the flag value passed to SceneRequest_Setup.
static int MapSceneModeCodeToFlags(uint32_t modeCode) {
    switch (modeCode) {
        case 0:
            return 0;
        case 1:
            return 2;
        case 2:
            return 4;
        case 3:
            return 12;
        default:
            return 0;
    }
}

// __stdcall, 3 stack args (ret 12). a3 is an array of four pointers to argument records.
static unsigned int* __stdcall DispatchSceneRequestFromArgs(int /*a1*/, int /*a2*/, uint32_t* a3) {
    // Mode code: *(*(*a3 + 4) + 4)
    uint32_t* modeRecord = reinterpret_cast<uint32_t*>(a3[0]);
    uint32_t* modeInner = reinterpret_cast<uint32_t*>(modeRecord[1]);
    uint32_t modeCode = modeInner[1];
    int flags = MapSceneModeCodeToFlags(modeCode);

    // Copy of the 3-dword block at (*(a3[1] + 4)) + 4.
    uint32_t* blockRecord = reinterpret_cast<uint32_t*>(a3[1]);
    uint32_t* blockSrc = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(reinterpret_cast<uint32_t*>(blockRecord[1])) + 4);
    uint32_t v7[3];
    v7[0] = blockSrc[0];
    v7[1] = blockSrc[1];
    v7[2] = blockSrc[2];

    // Value from a3[3]: *(*(a3[3] + 4) + 4)
    uint32_t* lastRecord = reinterpret_cast<uint32_t*>(a3[3]);
    uint32_t lastValue = reinterpret_cast<uint32_t*>(lastRecord[1])[1];

    // Value from a3[2]: *(*(a3[2] + 4) + 4)
    uint32_t* midRecord = reinterpret_cast<uint32_t*>(a3[2]);
    uint32_t midValue = reinterpret_cast<uint32_t*>(midRecord[1])[1];

    int resultBuffer[10];
    SceneRequest_Setup(resultBuffer, nullptr, flags, v7,
                       static_cast<int>(midValue), static_cast<int>(lastValue));

    // Scene subobject: *(g_pCurrentScene + 0xB0)
    void* scene = g_pCurrentScene;
    void* sceneSubObject = *CSF_AT(void**, scene, 0xB0);
    return SceneRequest_Submit(sceneSubObject, nullptr,
                               reinterpret_cast<int>(resultBuffer));
}

HOOK(0x0045DBE0, DispatchSceneRequestFromArgs, "auto");
