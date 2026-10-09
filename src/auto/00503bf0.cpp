#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, SceneLoader_IsSceneLoaded, 0x00450610, (uint32_t a1));
CSF_FUNC(int, __cdecl, SceneLoader_SelectScene, 0x00450630, (int a1, int a2));
CSF_FUNC(void*, __cdecl, SceneLoader_PrepareSceneSwitch, 0x004565D0, ());
CSF_FUNC(char, __fastcall, C_GetByte023C, 0x00495490, (char* self, void*));
CSF_FUNC(int, __stdcall, SceneLoader_ProcessSceneSource, 0x004FE2D0, (void* Source));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_AppendSceneRecord, 0x00503A10, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(bool, __cdecl, SceneNamesMatch, 0x00694AA0, (char* first, char* second));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static unsigned int __fastcall SceneLoader_FindOrAddScene(
    void* self, void* /*edx*/, char* source)
{
    unsigned int index = 0;
    unsigned int offset = 0;
    for (;;)
    {
        uint32_t begin = CSF_AT(uint32_t, self, 0x30);
        if (!begin ||
            index >= static_cast<unsigned int>(
                static_cast<int32_t>(CSF_AT(uint32_t, self, 0x34) - begin) / 88))
            break;

        auto* record = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x30) + offset);
        char* name = CSF_AT(uint32_t, record, 0x18) < 0x10
            ? reinterpret_cast<char*>(record + 4)
            : CSF_AT(char*, record, 4);
        if (SceneNamesMatch(source, name))
            return index;

        offset += 88;
        ++index;
    }

    alignas(4) uint8_t temporary[0x68]{};
    auto* firstPair = reinterpret_cast<C_0082d410*>(temporary + 0x24);
    auto* secondPair = reinterpret_cast<C_0082d410*>(temporary + 0x30);
    auto* thirdPair = reinterpret_cast<C_0082d410*>(temporary + 0x3C);

    firstPair->field_0004 = -1;
    firstPair->field_0008 = -1;
    secondPair->field_0004 = -1;
    secondPair->field_0008 = -1;
    thirdPair->field_0004 = -1;
    thirdPair->field_0008 = -1;
    firstPair->vtbl = CSF_VTBL(0x0082D448);
    secondPair->vtbl = CSF_VTBL(0x0082D448);
    thirdPair->vtbl = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, temporary, 0x18) = 15;
    CSF_AT(uint32_t, temporary, 0x14) = 0;
    CSF_AT(char, temporary, 4) = 0;
    CSF_AT(void*, temporary, 0x48) = nullptr;
    CSF_AT(uint32_t, temporary, 0x4C) = 0;
    CSF_AT(uint32_t, temporary, 0x50) = 0;
    CSF_AT(int, temporary, 0x54) = 10;
    CSF_AT(char, temporary, 0x1C) = 0;
    CSF_AT(char, temporary, 0x1D) = 0;
    CSF_AT(int, temporary, 0x20) = 0;

    AssignStringRange_429B60(
        temporary, nullptr, source, static_cast<unsigned int>(std::strlen(source)));
    SceneLoader_AppendSceneRecord(
        static_cast<uint8_t*>(self) + 44, nullptr,
        reinterpret_cast<int>(temporary));

    uint32_t begin = CSF_AT(uint32_t, self, 0x30);
    int addedIndex = (begin
        ? static_cast<int32_t>(CSF_AT(uint32_t, self, 0x34) - begin) / 88
        : 0) - 1;

    if (CSF_AT(char, reinterpret_cast<void*>(GetGlobalInt()), 4))
        SceneLoader_ProcessSceneSource(source);

    begin = CSF_AT(uint32_t, self, 0x30);
    if (begin &&
        static_cast<int32_t>(CSF_AT(uint32_t, self, 0x34) - begin) / 88 == 1)
    {
        auto* record = CSF_AT(uint8_t*, self, 0x30);
        CSF_AT(int, self, 0x0C) = 0;
        CSF_AT(char, self, 0x58) = 0;
        CSF_AT(char, record, 0x1C) = 1;

        record = CSF_AT(uint8_t*, self, 0x30)
            + 88 * CSF_AT(int, self, 0x0C);
        CSF_AT(char, record, 0x1D) = 1;
        record = CSF_AT(uint8_t*, self, 0x30)
            + 88 * CSF_AT(int, self, 0x0C);
        CSF_AT(int, record, 0x20) = 1;

        char* selectedObjects[3] = {};
        auto* objectList = static_cast<uint8_t*>(g_pCurrentScene) + 96;
        unsigned int objectIndex = 0;
        if (CSF_AT(uint32_t, objectList, 8))
        {
            do
            {
                char* object = CSF_AT(char**, objectList, 0)[objectIndex];
                if (object && C_GetByte023C(object, nullptr))
                {
                    if (!selectedObjects[0])
                        selectedObjects[0] = object;
                    else if (!selectedObjects[1])
                        selectedObjects[1] = object;
                    else
                        selectedObjects[2] = object;
                }
                ++objectIndex;
            } while (objectIndex < CSF_AT(uint32_t, objectList, 8));
        }

        record = CSF_AT(uint8_t*, self, 0x30)
            + 88 * CSF_AT(int, self, 0x0C);
        CSF_AT(int, record, 0x28) = -1;
        CSF_AT(int, record, 0x2C) = -1;
        record = CSF_AT(uint8_t*, self, 0x30)
            + 88 * CSF_AT(int, self, 0x0C);
        CSF_AT(int, record, 0x34) = -1;
        CSF_AT(int, record, 0x38) = -1;
        record = CSF_AT(uint8_t*, self, 0x30)
            + 88 * CSF_AT(int, self, 0x0C);
        CSF_AT(int, record, 0x40) = -1;
        CSF_AT(int, record, 0x44) = -1;

        for (unsigned int j = 0; j < 3; ++j)
        {
            char* object = selectedObjects[j];
            if (!object)
                continue;

            switch (CSF_AT(int, object, 0x54))
            {
            case 1:
                record = CSF_AT(uint8_t*, self, 0x30)
                    + 88 * CSF_AT(int, self, 0x0C);
                CSF_AT(uint32_t, record, 0x28) = CSF_AT(uint32_t, object, 0x20);
                CSF_AT(uint32_t, record, 0x2C) = CSF_AT(uint32_t, object, 0x24);
                break;
            case 2:
                record = CSF_AT(uint8_t*, self, 0x30)
                    + 88 * CSF_AT(int, self, 0x0C);
                CSF_AT(uint32_t, record, 0x34) = CSF_AT(uint32_t, object, 0x20);
                CSF_AT(uint32_t, record, 0x38) = CSF_AT(uint32_t, object, 0x24);
                break;
            case 3:
                record = CSF_AT(uint8_t*, self, 0x30)
                    + 88 * CSF_AT(int, self, 0x0C);
                CSF_AT(uint32_t, record, 0x40) = CSF_AT(uint32_t, object, 0x20);
                CSF_AT(uint32_t, record, 0x44) = CSF_AT(uint32_t, object, 0x24);
                break;
            }
        }
    }
    else if (!SceneLoader_IsSceneLoaded(static_cast<uint32_t>(addedIndex)))
    {
        int previousScene = g_CurrentSceneIndex;
        SceneLoader_PrepareSceneSwitch();
        SceneLoader_SelectScene(previousScene, 0);
    }

    void* ownedObjects = CSF_AT(void*, temporary, 0x48);
    if (ownedObjects)
    {
        auto* allocation = static_cast<uint8_t*>(ownedObjects) - 4;
        if (CSF_AT(uint32_t, allocation, 0))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, ownedObjects, 0)[0])(ownedObjects, nullptr, 3);
        }
        else
        {
            Mem_Free(allocation);
        }
        CSF_AT(void*, temporary, 0x48) = nullptr;
    }

    thirdPair->vtbl = CSF_VTBL(0x0082D410);
    secondPair->vtbl = CSF_VTBL(0x0082D410);
    firstPair->vtbl = CSF_VTBL(0x0082D410);
    if (CSF_AT(uint32_t, temporary, 0x18) >= 0x10 &&
        CSF_AT(uint32_t, temporary, 4))
        Mem_FreeDebug(CSF_AT(void*, temporary, 4));

    return static_cast<unsigned int>(addedIndex);
}

HOOK(0x00503BF0, SceneLoader_FindOrAddScene, "auto");
