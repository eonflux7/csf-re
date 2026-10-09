#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t*, __fastcall, SceneNodeQueryCore, 0x004458A0, (void* self, void* edx, int a2, uint32_t* a3, int a4, int a5, int a6));
CSF_FUNC(uint32_t*, __fastcall, ResolveSceneObjectRect, 0x00447130, (void* self, void* edx, float* a2));

static int __stdcall QueryBoundsFromSceneNodes(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    // Copy the three-dword bounds record reached through a3[0] -> +4 -> +4.
    uint32_t* firstRef = reinterpret_cast<uint32_t*>(a3[0]);
    uint32_t* boundsSource = reinterpret_cast<uint32_t*>(firstRef[1] + 4);
    uint32_t boundsCopy[3];
    boundsCopy[0] = boundsSource[0];
    boundsCopy[1] = boundsSource[1];
    boundsCopy[2] = boundsSource[2];

    uint8_t sceneRectBuffer[32];

    // Second and third inputs: each is a pointer to an object whose +4 field holds a dword at +4.
    uint32_t* sourceA = reinterpret_cast<uint32_t*>(a3[1]);
    int argA = static_cast<int>(reinterpret_cast<uint32_t*>(sourceA[1])[1]);

    uint32_t* sourceB = reinterpret_cast<uint32_t*>(a3[2]);
    int argB = static_cast<int>(reinterpret_cast<uint32_t*>(sourceB[1])[1]);

    SceneNodeQueryCore(sceneRectBuffer, nullptr, 0, boundsCopy, argA, 0, argB);

    void* sceneObject = CSF_AT(void*, g_pCurrentScene, 0xB0);
    uint32_t* resolved = ResolveSceneObjectRect(sceneObject, nullptr, reinterpret_cast<float*>(sceneRectBuffer));
    return static_cast<int>(reinterpret_cast<uintptr_t>(resolved));
}

HOOK(0x0045B470, QueryBoundsFromSceneNodes, "auto");
