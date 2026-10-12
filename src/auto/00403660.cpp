#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, RpCollisionWorldForAllIntersections, 0x0056BAF0, (int world, void* box, uintptr_t callback, void* userData));
static CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
static CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
static CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));

static char __cdecl Collision_ForAllSceneIntersections(int* a1, int* a2, int* a3, int* a4, int a5, int* a6)
{
    alignas(8) uint8_t queryBytes[0x1C];   // ebp-0x1C
    alignas(8) uint8_t userBytes[0x34];    // ebp-0x50

    CSF_AT(int, queryBytes, 0x0C) = a1[3];
    CSF_AT(int, queryBytes, 0x10) = a1[4];
    CSF_AT(int, queryBytes, 0x14) = a1[5];
    CSF_AT(int, queryBytes, 0x00) = a1[0];
    CSF_AT(int, queryBytes, 0x04) = a1[1];
    CSF_AT(int, queryBytes, 0x08) = a1[2];

    CSF_AT(int, userBytes, 0x00) = a5;
    CSF_AT(int, userBytes, 0x1C) = a4[0];
    CSF_AT(int, userBytes, 0x20) = a4[1];
    CSF_AT(int, userBytes, 0x24) = a4[2];
    CSF_AT(int, userBytes, 0x04) = a2[0];
    CSF_AT(int, userBytes, 0x08) = a2[1];
    CSF_AT(int, userBytes, 0x0C) = a2[2];
    CSF_AT(int, userBytes, 0x10) = a3[0];
    CSF_AT(int, userBytes, 0x14) = a3[1];
    CSF_AT(int, userBytes, 0x18) = a3[2];
    CSF_AT(int, queryBytes, 0x18) = 4;
    CSF_AT(int, userBytes, 0x30) = *a6;

    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    unsigned int count = ((unsigned int(__fastcall*)(void*, void*))CSF_AT(void**, sceneController, 0)[34])(sceneController, nullptr);
    unsigned int index = 0;
    if (count != 0) {
        do {
            void* scene = ((void*(__fastcall*)(void*, void*, unsigned int))CSF_AT(void**, sceneController, 0)[36])(sceneController, nullptr, index);
            if (scene && SceneLoader_GetWorldObjectCollection(scene, nullptr)) {
                int collection = SceneLoader_GetWorldObjectCollection(scene, nullptr);
                RpCollisionWorldForAllIntersections(collection, queryBytes, CSF_ADDR_FN(0x402CB0), userBytes);
            }
            ++index;
        } while (index < count);
    }

    void* controller2 = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* scene2 = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, controller2, 0)[17])(controller2, nullptr);

    char result = CSF_AT(char, scene2, 0x28);
    if (result) {
        int collection = SceneLoader_GetWorldObjectCollection(scene2, nullptr);
        result = static_cast<char>(collection);
        if (collection) {
            collection = SceneLoader_GetWorldObjectCollection(scene2, nullptr);
            result = static_cast<char>(RpCollisionWorldForAllIntersections(collection, queryBytes, CSF_ADDR_FN(0x402CB0), userBytes));
        }
    }
    return result;
}

HOOK(0x00403660, Collision_ForAllSceneIntersections, "auto");
