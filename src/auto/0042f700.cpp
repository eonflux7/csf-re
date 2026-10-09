#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (void* self, void* /*edx*/, int value));
CSF_FUNC(int, __stdcall, Hud_AcquireValueObject, 0x0042EC20, (int a1, int a2));

// Two-slot pair of value objects kept at this+0x74 (slot 0 at +0, slot 1 at +4).
static uint32_t* Hud_ValueSlotPair(uint8_t* hud)
{
    return reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, hud, 0x74));
}

// GetValue8B0554() yields a service object whose sub-object sits at +0x1FC0.
static void* Hud_GetValueServiceSubObject()
{
    int valueService = GetValue8B0554();
    return reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(valueService), 0x1FC0));
}

// Refreshes the HUD panel for a scene handle. Returns the low byte of the
// result (0 when the handle does not resolve, the latched flag when already
// initialised, or the low byte of the mission call on first activation).
static char __fastcall Hud_RefreshObjectPanel(void* self, void* /*edx*/, int handle)
{
    uint8_t* hud = static_cast<uint8_t*>(self);
    char result = static_cast<char>(CSF_AT(uint8_t, hud, 0x127));
    if (result != 0)
    {
        int sceneObject = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
        result = static_cast<char>(sceneObject);
        if (sceneObject != 0)
        {
            uint32_t objectData = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(sceneObject), 0x2C);
            if (objectData != 0)
            {
                int valueId = CSF_AT(int, reinterpret_cast<uint8_t*>(objectData), 0xC0);

                int acquiredB = Hud_AcquireValueObject(valueId, reinterpret_cast<int>(hud + 0xD8));
                Hud_ValueSlotPair(hud)[1] = static_cast<uint32_t>(acquiredB);

                void* valueService = Hud_GetValueServiceSubObject();
                int valueIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, valueService, 0)[3])(valueService, nullptr, valueId);
                if (valueIndex != -1)
                {
                    void* entryService = Hud_GetValueServiceSubObject();
                    int valueEntry = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, entryService, 0)[2])(entryService, nullptr, valueIndex);
                    int targetId = CSF_AT(int, reinterpret_cast<uint8_t*>(valueEntry), 0xDC);

                    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
                    if (sceneMenuContext != 0)
                    {
                        uint8_t* menuContext = reinterpret_cast<uint8_t*>(sceneMenuContext);
                        void* menuSubObject = reinterpret_cast<void*>(CSF_AT(uint32_t, menuContext, 0x44));
                        int handleEntry = ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, menuSubObject, 0)[60])(menuSubObject, nullptr, targetId, 8);
                        int* valuePointer = reinterpret_cast<int*>(Handle_GetValuePointer(reinterpret_cast<void*>(handleEntry), nullptr));
                        int pointedValue = *valuePointer;
                        int acquiredA = Hud_AcquireValueObject(pointedValue, reinterpret_cast<int>(hud + 0xD8));
                        Hud_ValueSlotPair(hud)[0] = static_cast<uint32_t>(acquiredA);
                    }

                    uint32_t firstValueObject = Hud_ValueSlotPair(hud)[0];
                    if (firstValueObject != 0)
                    {
                        void* firstObj = reinterpret_cast<void*>(firstValueObject);
                        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, firstObj, 0)[10])(firstObj, nullptr, reinterpret_cast<int>(hud + 0xF0));
                    }

                    uint32_t secondValueObject = Hud_ValueSlotPair(hud)[1];
                    if (secondValueObject != 0)
                    {
                        void* secondObj = reinterpret_cast<void*>(secondValueObject);
                        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, secondObj, 0)[10])(secondObj, nullptr, reinterpret_cast<int>(hud + 0xF8));
                    }
                }
            }

            result = static_cast<char>(CSF_AT(uint8_t, hud, 0x23D));
            if (result == 0)
            {
                void* missionInfo = reinterpret_cast<void*>(Get_008b0678());
                int setIndexResult = MenuMissionInfo_SetGlobalIndex(missionInfo, nullptr, 93);
                result = static_cast<char>(setIndexResult);
                CSF_AT(uint8_t, hud, 0x23D) = 1;
            }
        }
        else
        {
            CSF_AT(uint8_t, hud, 0x23D) = 0;
        }
    }
    return result;
}

HOOK(0x0042F700, Hud_RefreshObjectPanel, "auto");
