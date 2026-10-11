#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, FxParticleManager_IsSceneEntryAssociated, 0x004EF190, (void* self, void* edx, void* a1, int a2));
CSF_FUNC(int, __fastcall, FxParticleManager_FindSceneEntryAtPosition, 0x004EF6E0, (void* self, void* edx, void* a2, char a3));
CSF_FUNC(int, __fastcall, AiAttackTasks_FindSceneEntryIndex, 0x00770610, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneEntry, 0x00770690, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fIntroVideoDuration, 0x0082E118);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_00840f6c_UpdateParticleState(
    C_00840f6c* self, void* /*edx*/, float deltaTime)
{
    void* sceneEntries =
        CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC);

    if (self->field_0018 == 0)
        return 0;

    alignas(8) uint8_t positionBytes[0x18];
    uint32_t processed = 0;
    uint32_t recordOffset = 0;

    do
    {
        auto* record =
            reinterpret_cast<uint8_t*>(self->field_0010) + recordOffset;

        int resolvedObject = CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(record + 4));
        void* object = reinterpret_cast<void*>(resolvedObject);

        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, object, 0)[0xA4 / 4])(
                object, nullptr, positionBytes);

        bool updateTimer;
        void* associatedEntry = CSF_AT(void*, record, 0x10);
        if (associatedEntry == reinterpret_cast<void*>(-1) ||
            !FxParticleManager_IsSceneEntryAssociated(
                sceneEntries, nullptr, associatedEntry, resolvedObject))
        {
            int entryIndex = FxParticleManager_FindSceneEntryAtPosition(
                sceneEntries, nullptr, positionBytes, 2);

            updateTimer = true;
            if (entryIndex != -1)
            {
                void* entry = reinterpret_cast<void*>(
                    SceneLoader_GetSceneEntry(sceneEntries, nullptr, entryIndex));
                updateTimer = (CSF_AT(uint8_t, entry, 0x58) & 8) != 0;
            }
        }
        else
        {
            int entryIndex = AiAttackTasks_FindSceneEntryIndex(
                sceneEntries, nullptr, CSF_AT(uint32_t, record, 0x10));
            void* entry = reinterpret_cast<void*>(
                SceneLoader_GetSceneEntry(sceneEntries, nullptr, entryIndex));
            updateTimer = (CSF_AT(uint8_t, entry, 0x58) & 8) != 0;
        }

        if (!updateTimer)
        {
            void* particle = CSF_AT(void*, record, 0);
            if (!((bool(__fastcall*)(void*, void*))
                    CSF_AT(void**, particle, 0)[0x34 / 4])(particle, nullptr))
            {
                particle = CSF_AT(void*, record, 0);
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, particle, 0)[0x2C / 4])(
                        particle, nullptr, 0);
            }
        }
        else
        {
            float elapsed = deltaTime + CSF_AT(float, record, 0x18);
            CSF_AT(float, record, 0x18) = elapsed;

            if (elapsed > g_fIntroVideoDuration)
            {
                void* particle = CSF_AT(void*, record, 0);
                if (((bool(__fastcall*)(void*, void*))
                        CSF_AT(void**, particle, 0)[0x34 / 4])(particle, nullptr))
                {
                    particle = CSF_AT(void*, record, 0);
                    ((void(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, particle, 0)[0x28 / 4])(
                            particle, nullptr, 0);
                    CSF_AT(float, record, 0x18) = 0.0f;
                }
            }
        }

        ++processed;
        recordOffset += 28;
    }
    while (processed < self->field_0018);

    return static_cast<int>(processed);
}

HOOK(0x0051EB20, C_00840f6c_UpdateParticleState, "auto");
