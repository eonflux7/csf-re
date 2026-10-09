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

CSF_FUNC(uint32_t*, __fastcall, SceneNodeQueryCore, 0x004458A0, (void* self, void* edx, int a2, uint32_t* a3, int a4, int a5, int a6));
CSF_FUNC(uint32_t*, __fastcall, ResolveSceneObjectRect, 0x00447130, (void* self, void* edx, float* a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Reads the dword stored at base+offset.
static uint32_t ReadDwordAt(uint32_t base, uint32_t offset)
{
    return CSF_AT(uint32_t, reinterpret_cast<void*>(base), offset);
}

static int __stdcall SubmitTimedRequestAndResolve(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    // Request header: three dwords taken from the object at *(a3[0]+4)+4.
    uint32_t* headerSource = reinterpret_cast<uint32_t*>(ReadDwordAt(a3[0], 4) + 4);
    uint32_t requestHeader[3];
    requestHeader[0] = headerSource[0];
    requestHeader[1] = headerSource[1];
    requestHeader[2] = headerSource[2];

    // Second, third and fourth argument chains.
    uint32_t sceneIdValue = ReadDwordAt(ReadDwordAt(a3[1], 4), 4);
    uint32_t thirdValue = ReadDwordAt(ReadDwordAt(a3[2], 4), 4);

    // Float limit from *(*(a3[3]+4)+4); non-positive values become FLT_MAX.
    uint32_t limitHolder = ReadDwordAt(a3[3], 4) + 4;
    float limit = *reinterpret_cast<float*>(limitHolder);
    if (limit <= g_fZero)
    {
        limit = FLT_MAX;
    }
    uint32_t limitBits;
    std::memcpy(&limitBits, &limit, sizeof(limitBits));

    uint8_t sceneQueryBuffer[32];
    SceneNodeQueryCore(sceneQueryBuffer, nullptr, 1, requestHeader,
                       static_cast<int>(sceneIdValue),
                       static_cast<int>(limitBits),
                       static_cast<int>(thirdValue));

    // Resolve the rect using the current scene's +0xB0 child.
    void* currentScene = g_pCurrentScene;
    void* sceneChild = CSF_AT(void*, currentScene, 0xB0);
    uint32_t* resolved = ResolveSceneObjectRect(sceneChild, nullptr,
                                                reinterpret_cast<float*>(sceneQueryBuffer));
    return static_cast<int>(reinterpret_cast<uintptr_t>(resolved));
}

HOOK(0x0045B4E0, SubmitTimedRequestAndResolve, "auto");
