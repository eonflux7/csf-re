#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(unsigned int, __cdecl, SceneLoader_CopyMapFilename, 0x006948E0, (char* a1, char* a2, unsigned int a3));
CSF_FUNC(char*, __cdecl, Hud_ExtractResourcePath, 0x00694960, (char* a1, uint8_t* a2, unsigned int a3));
CSF_FUNC(unsigned int, __cdecl, CoreFiles_FindNameIndex, 0x006977C0, (char* name, void* entries));
CSF_FUNC(uint32_t*, __fastcall, CoreFiles_InitializeArchiveEntryString, 0x00697BA0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CoreFiles_InsertArchiveEntry, 0x00698200, (void* self, void* edx, void* a3));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(uint32_t*, __fastcall, CoreFiles_ConstructArchiveFileSystem, 0x0069F980, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(char*, __cdecl, _strstr, 0x007B2350, (char* Str, char* SubStr));

static void __cdecl CoreFiles_SelectArchive(char* filename)
{
    if (!filename || !*filename)
        return;

    if (CoreFiles_FindNameIndex(
            filename, reinterpret_cast<uint8_t*>(g_pOptions) + 0x50) !=
        static_cast<unsigned int>(-1))
        return;

    alignas(8) uint8_t resourcePath[0x200] = {};
    alignas(8) char mapFilename[0x200] = {};
    alignas(8) uint8_t entryBytes[0x24];
    C_0082d410& entry = *reinterpret_cast<C_0082d410*>(entryBytes);

    Hud_ExtractResourcePath(filename, resourcePath, 512);
    SceneLoader_CopyMapFilename(filename, mapFilename, 512);
    CoreFiles_InitializeArchiveEntryString(&entry, nullptr);
    AssignStringRange_429B60(
        &entry, nullptr, filename,
        static_cast<unsigned int>(std::strlen(filename)));

    const uint8_t previousAccessFlag = CSF_AT(uint8_t, g_pOptions, 0x40);
    CSF_AT(uint8_t, g_pOptions, 0x40) = 1;

    const unsigned int resourcePathLength =
        static_cast<unsigned int>(
            std::strlen(reinterpret_cast<char*>(resourcePath)));
    CSF_AT(uint32_t, &entry, 0x20) = resourcePathLength;

    void* file = nullptr;
    if (resourcePathLength)
    {
        alignas(8) char mapPath[0x200] = {};
        if (_strstr(mapFilename, const_cast<char*>(CSF_STR(0x0085E714, "_MP."))))
            _sprintf(mapPath, CSF_STR(0x0085E708, "Maps/MP/%s"), mapFilename);
        else
            _sprintf(mapPath, CSF_STR(0x0085E700, "Maps/%s"), mapFilename);

        file = CoreFiles_OpenSceneFile(
            mapPath, const_cast<char*>(CSF_STR(0x0083E130, "r")), 0);
    }

    if (!file)
        file = CoreFiles_OpenSceneFile(
            filename, const_cast<char*>(CSF_STR(0x0083E130, "r")), 0);

    if (file)
    {
        const int storage = CoreMemory_AllocUntagged(68);
        if (storage)
        {
            CSF_AT(uint32_t*, &entry, 0x1C) =
                CoreFiles_ConstructArchiveFileSystem(
                    reinterpret_cast<void*>(storage), nullptr,
                    reinterpret_cast<int>(file));
        }
        else
        {
            CSF_AT(uint32_t*, &entry, 0x1C) = nullptr;
        }

        CoreFiles_InsertArchiveEntry(
            reinterpret_cast<uint8_t*>(g_pOptions) + 0x50,
            nullptr, &entry);
    }

    CSF_AT(uint8_t, g_pOptions, 0x40) = previousAccessFlag;
    CMenuSaveGame_ResetStringStorage(&entry, nullptr);
}

HOOK(0x00698960, CoreFiles_SelectArchive, "auto");
