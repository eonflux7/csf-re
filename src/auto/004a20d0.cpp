#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_ProcessDisplayFlags_453070, 0x00453070, (CGuiControl* self, void* , void* a2, float* a3, int* a4, int a5, float a6));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GameConfig_IsSceneObjectRemovable, 0x007558C0, (int objectType));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall HudAlertIcons_RefreshFlaggedSelection(
    C_008342e8** self, void* /*edx*/)
{
    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    float* position = GetObjectPositionVector(self[2], nullptr);

    alignas(8) uint8_t positionBytes[0x10];
    float* queryPosition = reinterpret_cast<float*>(positionBytes);
    queryPosition[0] = position[0];
    C_008342e8* selectedObject = self[2];
    queryPosition[1] = position[1];
    queryPosition[2] = position[2];
    queryPosition[3] = CSF_AT(float, settings, 0xB10);

    alignas(8) uint8_t filterBytes[0x34] = {};
    int* filter = reinterpret_cast<int*>(filterBytes);
    filter[0] = 17;
    filter[1] = 7;
    filter[2] = -1;
    filter[3] = -1;
    filter[7] = 10;
    filter[11] = -1;

    alignas(8) uint8_t resultsBytes[0x18] = {};
    CSF_AT(int, resultsBytes, 0xC) = 10;
    CSF_AT(C_008342e8**, resultsBytes, 0x14) = self;

    alignas(8) uint8_t flagsBytes[0x8] = {};
    CSF_AT(int, flagsBytes, 0) = 5;

    float radius = CSF_AT(float, selectedObject, 0x1D4) * g_HalfDurationScale;
    CGuiControl_ProcessDisplayFlags_453070(
        static_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        resultsBytes, queryPosition, filter,
        reinterpret_cast<int>(flagsBytes), radius);

    const int count = CSF_AT(int, resultsBytes, 0x8);
    for (int index = 0; index < count; ++index)
    {
        void* entry = CSF_AT(void**, resultsBytes, 0)[index];
        const int objectType = CSF_AT(int, entry, 0x50);
        void* selection = reinterpret_cast<void*>(self[2]->field_0044);

        if (objectType == 23)
        {
            const int id = CSF_AT(int, entry, 0x184);
            if (id != -1 &&
                ((bool(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, selection, 0)[0xE0 / 4])(
                        selection, nullptr, id))
            {
                void* handler = CSF_AT(void*, entry, 0x34);
                ((void(__fastcall*)(void*, void*, int, int))
                    CSF_AT(void**, handler, 0)[0xCC / 4])(
                        handler, nullptr, id, 0);
            }
            continue;
        }

        int id;
        if (objectType == 22)
        {
            id = CSF_AT(int, entry, 0x184);
        }
        else if (static_cast<uint8_t>(
                     GameConfig_IsSceneObjectRemovable(objectType)) != 0)
        {
            void* owner = CSF_AT(void*, entry, 0x4C);
            if (!owner ||
                ((bool(__fastcall*)(void*, void*))
                    CSF_AT(void**, owner, 0)[0x4C / 4])(owner, nullptr))
            {
                continue;
            }
            id = CSF_AT(int, entry, 0x194);
        }
        else
        {
            void* record = CSF_AT(void*, entry, 0x2C);
            if (CSF_AT(int, record, 0xC0) == -1)
            {
                void* handler = CSF_AT(void*, entry, 0x34);
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, handler, 0)[0xC4 / 4])(
                        handler, nullptr, 0);
            }
            continue;
        }

        if (id != -1 &&
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, selection, 0)[0xB0 / 4])(
                    selection, nullptr, id) == 1)
        {
            void* handler = CSF_AT(void*, entry, 0x34);
            ((void(__fastcall*)(void*, void*, int, int))
                CSF_AT(void**, handler, 0)[0xC8 / 4])(
                    handler, nullptr, id, 0);
        }
    }

    if (CSF_AT(void*, filterBytes, 0x10))
    {
        Mem_Free(CSF_AT(void*, filterBytes, 0x10));
        CSF_AT(void*, filterBytes, 0x10) = nullptr;
    }

    void* results = CSF_AT(void*, resultsBytes, 0);
    return results ? Mem_Free(results) : 0;
}

HOOK(0x004A20D0, HudAlertIcons_RefreshFlaggedSelection, "auto");
