#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int*, __fastcall, SceneLoader_ResetPreviousSceneManager, 0x00441990, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetValue_8afc80, 0x004419C0, ());
CSF_FUNC(int*, __fastcall, SceneLoader_ResetNextSceneManager, 0x00442200, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_NotifySceneChange, 0x004570F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneManager, 0x004FC550, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_UpdateLoadState, 0x004FF760, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSelectedSceneName, 0x00500350, (void* self, void* edx));
CSF_FUNC(unsigned int, __stdcall, SceneLoader_CopySceneName, 0x005011E0, (int a1, void* a2));
CSF_FUNC(int, __fastcall, SceneLoader_ClearScene, 0x00501AB0, (void* a1, int a2, char a3));
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuSystem_ResetStateFields, 0x0050D230, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_ResetMenuResources, 0x0050E130, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareMenuForScene, 0x0050E7C0, (void* self, void* edx));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneResources, 0x0051FAD0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwFreeListPurgeAllFreeLists, 0x006265F0, ());
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(char, __cdecl, SceneLoader_ResetArchiveSelection, 0x006980D0, (char* String1));
CSF_FUNC(void, __cdecl, SceneLoader_SelectArchive, 0x00698960, (char* String1));
CSF_FUNC(int*, __cdecl, GetGlobalIntPtr, 0x0069C580, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static char __fastcall S_00500350_PrepareSceneLoad(
    S_00500350* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);
    char archivePath[0x204];

    SceneLoader_UpdateLoadState(self, nullptr);
    Nullsub_506AC0();

    if (CompareStringSubstring(
            bytes + 0x98, nullptr, 0, CSF_AT(uint32_t, self, 0xAC),
            reinterpret_cast<const char*>(&g_byte_82E65C), 0))
    {
        SceneLoader_CopySceneName(
            reinterpret_cast<int>(bytes + 0x98), bytes + 0x7C);
    }

    if (!CompareStringSubstring(
            bytes + 0x7C, nullptr, 0, CSF_AT(uint32_t, self, 0x90),
            reinterpret_cast<const char*>(&g_byte_82E65C), 0))
    {
        self->field_005c = 0;
        return 0;
    }

    const uint32_t entriesBegin = self->field_0030;
    if (entriesBegin &&
        static_cast<int32_t>(self->field_0034 - entriesBegin) / 88 != 0)
    {
        const char* requestedName =
            CSF_AT(uint32_t, self, 0x94) < 0x10u
                ? reinterpret_cast<const char*>(bytes + 0x80)
                : CSF_AT(const char*, self, 0x80);
        const char* selectedName = reinterpret_cast<const char*>(
            SceneLoader_GetSelectedSceneName(self, nullptr));
        CSF_AT(uint8_t, self, 0xE1) =
            __stricmp(requestedName, selectedName) == 0;
    }
    else
    {
        CSF_AT(uint8_t, self, 0xE1) = 0;
    }

    const char* currentName =
        CSF_AT(uint32_t, self, 0x78) < 0x10u
            ? reinterpret_cast<const char*>(bytes + 0x64)
            : CSF_AT(const char*, self, 0x64);
    const char* requestedName =
        CSF_AT(uint32_t, self, 0x94) < 0x10u
            ? reinterpret_cast<const char*>(bytes + 0x80)
            : CSF_AT(const char*, self, 0x80);
    const bool sameScene = __stricmp(requestedName, currentName) == 0;
    CSF_AT(uint8_t, self, 0xB6) = !sameScene;
    CSF_AT(uint8_t, self, 0xE0) = sameScene;

    if (!CompareStringSubstring(
            bytes + 0x98, nullptr, 0, CSF_AT(uint32_t, self, 0xAC),
            reinterpret_cast<const char*>(&g_byte_82E65C), 0) &&
        !CSF_AT(uint8_t, self, 0xE0))
    {
        SceneLoader_ResetPreviousSceneManager(GetGlobal8afc7c(), 0);
    }

    SceneLoader_PrepareSceneManager(
        reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, self, 0)[0x3C / 4])(self, nullptr, 0);

    if (!CSF_AT(uint8_t, self, 0xE0))
    {
        SceneLoader_ClearScene(self, 0, 0);
        RwFreeListPurgeAllFreeLists();
    }

    if (CSF_AT(uint8_t,
               reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA5C))
    {
        bool selectArchive = false;
        bool clearArchive = false;

        if (!CSF_AT(uint8_t, self, 0xE0))
        {
            selectArchive = true;
            clearArchive = true;
        }
        else if (!CompareStringSubstring(
                     bytes + 0x98, nullptr, 0,
                     CSF_AT(uint32_t, self, 0xAC),
                     reinterpret_cast<const char*>(&g_byte_82E65C), 0))
        {
            selectArchive = true;
            clearArchive = !CSF_AT(uint8_t, self, 0xE0);
        }

        if (selectArchive)
        {
            if (clearArchive)
            {
                Nullsub_506AC0();
                SceneLoader_SelectArchive(
                    reinterpret_cast<char*>(GetGlobalIntPtr()));
                SceneLoader_ResetMenuResources(
                    reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
            }

            SceneLoader_ResetArchiveSelection(
                reinterpret_cast<char*>(GetGlobalIntPtr()));

            const char* scenePath =
                CSF_AT(uint32_t, self, 0x94) < 0x10u
                    ? reinterpret_cast<const char*>(bytes + 0x80)
                    : CSF_AT(const char*, self, 0x80);
            std::strcpy(archivePath, scenePath);
            Path_ReplaceExtension(
                archivePath, 512,
                const_cast<char*>(CSF_STR(0x00830FEC, ".pak")));
            Nullsub_506AC0();
            SceneLoader_SelectArchive(archivePath);
        }
    }

    MenuSystem_ResetStateFields(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

    if (!CSF_AT(uint8_t, self, 0xE0))
    {
        SceneLoader_ClearScene(self, 0, 0);
        SceneLoader_ResetNextSceneManager(
            reinterpret_cast<void*>(GetValue_8afc80()), nullptr);

        if (!CSF_AT(uint8_t,
                    reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA5C))
        {
            SceneLoader_ResetMenuResources(
                reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
        }

        SceneLoader_PrepareMenuForScene(
            reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
        SceneLoader_PrepareSceneResources(
            reinterpret_cast<void*>(GetGlobalValue_51F740()), nullptr);

        void* notificationObject = CSF_AT(void*, self, 0xE4);
        if (notificationObject)
            SceneLoader_NotifySceneChange(notificationObject, nullptr);
    }

    return 1;
}

HOOK(0x00501B90, S_00500350_PrepareSceneLoad, "auto");
