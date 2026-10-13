#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseEmbeddedMember, 0x0044AFF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_AppendEntityHandle, 0x0044B9A0, (void* self, void* edx, int a3));
CSF_FUNC(char, __stdcall, TextDatabase_MatchesEntityHandle, 0x004EF080, (void* a1, int a2));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));
CSF_FUNC(int, __fastcall, AiAttackTasks_FindSceneEntryIndex, 0x00770610, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneEntry, 0x00770690, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall TextDatabase_FillEntityElementRange(
    S_004ef780* self,
    void* /*edx*/,
    S_004ef780* rangeKey,
    S_0044b8c0* output)
{
    ReleaseEmbeddedMember(output, nullptr);

    const uint32_t entityCount = CSF_AT(uint32_t, g_pCurrentScene, 0x28);
    for (uint32_t index = 0; index < entityCount; ++index)
    {
        void* entity = CSF_AT(void**, g_pCurrentScene, 0x20)[index];
        if (!entity)
            continue;

        if ((CSF_AT(uint8_t, entity, 0x170) & 1) == 0)
            continue;

        if (((bool(__fastcall*)(void*, void*))
                 CSF_AT(void**, entity, 0)[0x7C / 4])(entity, nullptr))
            continue;

        if ((CSF_AT(uint32_t, entity, 0x170) & 0x2000) == 0 &&
            static_cast<uint8_t>(GameConfig_IsResolvedEntryActive(
                CSF_AT(int, entity, 0x50))) == 0)
            continue;

        const int entryIndex = AiAttackTasks_FindSceneEntryIndex(
            self, nullptr, reinterpret_cast<unsigned int>(rangeKey));
        if (entryIndex == -1)
            continue;

        uint8_t* entityHandle = reinterpret_cast<uint8_t*>(entity) + 0x1C;
        void* sceneEntry = reinterpret_cast<void*>(
            SceneLoader_GetSceneEntry(self, nullptr, entryIndex));

        if (TextDatabase_MatchesEntityHandle(
                sceneEntry, reinterpret_cast<int>(entityHandle)))
        {
            AiAttackTasks_AppendEntityHandle(
                output, nullptr, reinterpret_cast<int>(entityHandle));
        }
    }

    return static_cast<int>(entityCount);
}

HOOK(0x004F0210, TextDatabase_FillEntityElementRange, "auto");
