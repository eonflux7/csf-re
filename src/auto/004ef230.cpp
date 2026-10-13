#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_FindEntryById, 0x00452A30, (CGuiControl* self, void* , unsigned int entryId));
CSF_FUNC(int, __fastcall, GuiControl_GetChildByIndex, 0x004A0F00, (void* self, void* , uint32_t index));
CSF_FUNC(char, __stdcall, TextDatabase_CheckSceneEntryChild, 0x004EF080, (void* a1, int a2));
CSF_FUNC(char, __fastcall, FxParticleManager_IsSceneEntryAssociated, 0x004EF190, (void* self, void* edx, void* a1, int a2));
CSF_FUNC(int, __fastcall, AiAttackTasks_FindSceneEntryIndex, 0x00770610, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneEntry, 0x00770690, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall S_004ef780_CheckSceneEntryChildren(
    S_004ef780* self,
    void* /*edx*/,
    S_004ef780* sceneEntryKey,
    unsigned int entryId,
    int mode)
{
    void* group = reinterpret_cast<void*>(
        CGuiControl_FindEntryById(
            static_cast<CGuiControl*>(g_pCurrentScene), nullptr, entryId));

    if (!group)
        return 0;

    char result = 0;
    if (mode == 0)
    {
        result = 1;
        for (uint32_t index = 0;
             index < CSF_AT(uint32_t, group, 0x0C);
             ++index)
        {
            void* child = reinterpret_cast<void*>(
                GuiControl_GetChildByIndex(group, nullptr, index));

            if (child &&
                !((bool(__fastcall*)(void*, void*))
                    CSF_AT(void**, child, 0)[0x7C / 4])(child, nullptr) &&
                !FxParticleManager_IsSceneEntryAssociated(
                    self, nullptr, sceneEntryKey,
                    reinterpret_cast<int>(child)))
            {
                result = 0;
                break;
            }
        }
    }
    else if (mode != 1)
    {
        return 0;
    }

    for (uint32_t index = 0;
         index < CSF_AT(uint32_t, group, 0x0C);
         ++index)
    {
        void* child = reinterpret_cast<void*>(
            GuiControl_GetChildByIndex(group, nullptr, index));

        if (child &&
            !((bool(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x7C / 4])(child, nullptr))
        {
            int sceneEntryIndex = AiAttackTasks_FindSceneEntryIndex(
                self, nullptr,
                reinterpret_cast<unsigned int>(sceneEntryKey));

            if (sceneEntryIndex != -1)
            {
                int childData = reinterpret_cast<int>(
                    static_cast<uint8_t*>(child) + 0x1C);
                void* sceneEntry = reinterpret_cast<void*>(
                    SceneLoader_GetSceneEntry(self, nullptr, sceneEntryIndex));

                if (TextDatabase_CheckSceneEntryChild(sceneEntry, childData))
                    return 1;
            }
        }
    }

    return result;
}

HOOK(0x004EF230, S_004ef780_CheckSceneEntryChildren, "auto");
