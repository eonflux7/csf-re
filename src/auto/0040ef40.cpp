#include <cstdint>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(uint32_t, g_QueryInitialState, 0x00869C58);

static char __fastcall C_0082ddf8_FindHighestMatchingResult(
    C_0082ddf8* self, void* /*edx*/, float* outScore,
    uint32_t* outCoordinates, int mask)
{
    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    void* queryProvider =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, sceneController, 0)[0x3C / 4])(
                sceneController, nullptr);

    void* itemRegistry = reinterpret_cast<void*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xE8));

    alignas(8) uint8_t queryStateBytes[0x8];
    CSF_AT(uint32_t, queryStateBytes, 0) = g_QueryInitialState;

    void* iterator =
        ((void* (__fastcall*)(void*, void*, int, float*, uint32_t*, int))
            CSF_AT(void**, queryProvider, 0)[0x34 / 4])(
                queryProvider, nullptr, 0,
                reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 0x2C),
                reinterpret_cast<uint32_t*>(queryStateBytes), 65);

    if (!iterator)
        return 0;

    alignas(8) uint8_t resultBytes[0x74];

    CSF_AT(uint32_t, resultBytes, 0x68) = 0;
    CSF_AT(uint32_t, resultBytes, 0x6C) = 0;
    CSF_AT(uint32_t, resultBytes, 0x70) = 0;
    CSF_AT(uint32_t, resultBytes, 0x04) = 0;
    CSF_AT(uint32_t, resultBytes, 0x08) = 0;
    CSF_AT(uint32_t, resultBytes, 0x0C) = 0;
    CSF_AT(uint32_t, resultBytes, 0x10) = 0;
    CSF_AT(uint32_t, resultBytes, 0x14) = 0;
    CSF_AT(uint32_t, resultBytes, 0x18) = 0;
    CSF_AT(uint32_t, resultBytes, 0x5C) = 0;
    CSF_AT(uint8_t, resultBytes, 0x28) = 0;
    CSF_AT(int, resultBytes, 0x2C) = -1;
    CSF_AT(int8_t, resultBytes, 0x30) = -1;
    CSF_AT(uint32_t, resultBytes, 0x60) = 0;
    CSF_AT(int, resultBytes, 0x64) = 10;
    CSF_AT(float, resultBytes, 0x00) = FLT_MAX;
    CSF_AT(int, resultBytes, 0x1C) = -1;
    CSF_AT(int8_t, resultBytes, 0x20) = -1;
    CSF_AT(uint32_t, resultBytes, 0x24) = 0;

    char found = 0;
    do
    {
        ((void (__fastcall*)(void*, void*, float*))
            CSF_AT(void**, iterator, 0)[0x0C / 4])(
                iterator, nullptr, reinterpret_cast<float*>(resultBytes));

        ((void (__fastcall*)(void*, void*))
            CSF_AT(void**, iterator, 0)[0x10 / 4])(
                iterator, nullptr);

        void* item =
            ((void* (__fastcall*)(void*, void*, int))
                CSF_AT(void**, itemRegistry, 0)[0x08 / 4])(
                    itemRegistry, nullptr, CSF_AT(int, resultBytes, 0x1C));

        if ((static_cast<uint32_t>(mask) & CSF_AT(uint32_t, item, 0x20)) != 0 &&
            (!found || CSF_AT(float, resultBytes, 0x00) >
                           CSF_AT(float, queryStateBytes, 0x04)))
        {
            CSF_AT(uint32_t, resultBytes, 0x68) =
                CSF_AT(uint32_t, resultBytes, 0x04);
            found = 1;
            CSF_AT(uint32_t, resultBytes, 0x6C) =
                CSF_AT(uint32_t, resultBytes, 0x08);
            CSF_AT(uint32_t, resultBytes, 0x70) =
                CSF_AT(uint32_t, resultBytes, 0x0C);
            CSF_AT(float, queryStateBytes, 0x04) =
                CSF_AT(float, resultBytes, 0x00);
        }
    } while (!((bool (__fastcall*)(void*, void*))
                  CSF_AT(void**, iterator, 0)[0x08 / 4])(
                      iterator, nullptr));

    if (found)
    {
        *outScore = CSF_AT(float, queryStateBytes, 0x04);
        outCoordinates[0] = CSF_AT(uint32_t, resultBytes, 0x68);
        outCoordinates[1] = CSF_AT(uint32_t, resultBytes, 0x6C);
        outCoordinates[2] = CSF_AT(uint32_t, resultBytes, 0x70);
    }

    return found;
}

HOOK(0x0040EF40, C_0082ddf8_FindHighestMatchingResult, "auto");
