#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, MpSpawnPoints_GetObjectId, 0x004A1980, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(void*, __fastcall, NetSession_LookupStatsSlotHandle, 0x0073E770, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall HudAlertIcons_EvaluateStageOne(void* self, void* /*edx*/)
{
    uint32_t objectId = 0;
    int teamCount = 0;

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (!CSF_AT(uint8_t, globalState, 0xD0))
        return;

    void* selection = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8));
    uint32_t actorHandle = CSF_AT(uint32_t, selection, 0x18);
    if (!actorHandle)
        return;

    void* actor = reinterpret_cast<void*>(actorHandle - 1712);
    if (!actor || CSF_AT(uint8_t, actor, 0x458) != 1)
        return;

    void* session = reinterpret_cast<void*>(GetGlobalDword_73C020());
    void* statsHandle = NetSession_LookupStatsSlotHandle(session, nullptr);
    if (statsHandle)
    {
        void* stats = reinterpret_cast<void*>(
            reinterpret_cast<uintptr_t>(statsHandle) - 776);
        if (stats)
        {
            uint8_t team = CSF_AT(uint8_t, actor, 0x443);
            if (team == 1)
                teamCount = CSF_AT(int, stats, 0x44);
            else if (team == 2)
                teamCount = CSF_AT(int, stats, 0x50);

            void* settings = reinterpret_cast<void*>(GetGlobalValue_558150());
            if (teamCount >= CSF_AT(int, settings, 0x1AC))
                return;
        }
    }

    selection = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8));
    if (CSF_AT(uint32_t, selection, 0x298) ||
        CSF_AT(int, selection, 0x248) < 0)
        return;

    int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(
            static_cast<uint8_t*>(selection) + 0x240)));
    if (!resolved)
        return;

    void* target = reinterpret_cast<void*>(resolved);
    if (CSF_AT(int, target, 0x50) != 18)
        return;

    uint32_t targetHandle = CSF_AT(uint32_t, target, 0x18);
    void* targetActor = targetHandle
        ? reinterpret_cast<void*>(targetHandle - 1712)
        : nullptr;
    if (!targetActor)
        return;

    if (CSF_AT(uint8_t, targetActor, 0x443) ==
        CSF_AT(uint8_t, actor, 0x443))
        return;
    if (!(CSF_AT(uint8_t, targetActor, 0x453) & 1))
        return;
    if (CSF_AT(uint8_t, targetActor, 0x749))
        return;

    int spawnObjectId = CSF_AT(int, targetActor, 0x668);
    if (spawnObjectId != -1)
    {
        uint32_t* currentObjectId =
            MpSpawnPoints_GetObjectId(actor, nullptr, &objectId);
        if (static_cast<uint32_t>(spawnObjectId) != *currentObjectId)
            return;
    }

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    selection = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8));
    if (!(CSF_AT(float, selection, 0x258) <=
          CSF_AT(float, settings, 0xB1C)))
        return;

    CSF_AT(int, selection, 0x298) = 13;

    selection = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8));
    CSF_AT(uint32_t, selection, 0x2A0) =
        CSF_AT(uint32_t, selection, 0x244);
    CSF_AT(uint32_t, selection, 0x2A4) =
        CSF_AT(uint32_t, selection, 0x248);

    int descriptionId =
        MenuExtras_ParseDescriptionId(CSF_STR(0x00834420, "G062"));
    selection = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8));
    CSF_AT(int, selection, 0x2AC) = descriptionId;
}

HOOK(0x004A1A20, HudAlertIcons_EvaluateStageOne, "auto");
