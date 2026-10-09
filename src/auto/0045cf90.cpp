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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_IsSecondCheckTrue, 0x0048E140, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Looks up the scene object referenced by a handle record and stores the
// boolean result (passes first check and not second check) into the flag
// byte at a1->field_0004 + 1. Returns that boolean as int; a negative
// handle index short-circuits and returns the flag-holder pointer value.
static int __stdcall EvaluateSceneHandleFlag(int a1, int a2, int a3)
{
    (void)a2;

    // a3 -> pointer to a record; record+4 -> node; node+12 = handle index, node+4 = handle value
    uintptr_t recordPtr = static_cast<uintptr_t>(*reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a3)));
    uint32_t* node = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(
        reinterpret_cast<uint32_t*>(recordPtr)[1]));
    int32_t handleIndex = static_cast<int32_t>(node[3]);
    int handleValue = static_cast<int>(reinterpret_cast<uintptr_t>(node) + 4);

    if (handleIndex < 0)
    {
        uint32_t* holder = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a1));
        int result = static_cast<int>(holder[1]);
        uint8_t* flagTarget = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(holder[1]));
        flagTarget[1] = 0;
        return result;
    }

    void* sceneObject = reinterpret_cast<void*>(static_cast<uintptr_t>(
        static_cast<uint32_t>(CHandleTable_Resolve(g_pCurrentScene, nullptr, handleValue))));

    if (sceneObject != nullptr
        && SceneObject_PassesFirstCheck(sceneObject, nullptr) != 0
        && SceneObject_IsSecondCheckTrue(sceneObject, nullptr) == 0)
    {
        uint32_t* holder = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a1));
        uint8_t* flagTarget = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(holder[1]));
        flagTarget[1] = 1;
        return 1;
    }

    uint32_t* holder = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a1));
    uint8_t* flagTarget = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(holder[1]));
    flagTarget[1] = 0;
    return 0;
}

HOOK(0x0045CF90, EvaluateSceneHandleFlag, "auto");
