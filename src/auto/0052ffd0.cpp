#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, CopyStringSubstring, 0x004109E0,
         (void* destination, void* edx, const void* source,
          unsigned int position, unsigned int count));
CSF_FUNC(void*, __fastcall, AssignStringRange, 0x00429B60,
         (void* destination, void* edx, char* source, unsigned int count));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200,
         (void* string, void* edx, unsigned int position,
          unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(void*, __cdecl, ClearPageState, 0x0050C300, (void* pageState));
CSF_FUNC(void*, __fastcall, AddScene, 0x0052FEF0,
         (CMenuSceneSelect* self, void* edx, void* sceneName));
CSF_FUNC(int, __cdecl, FreeDebugMemory, 0x00690E60, (void* block));
CSF_FUNC(bool, __cdecl, SceneNamesMatch, 0x00694AA0,
         (char* first, char* second));

// Capture EAX, including its incidental upper bits, for the target's return.
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x00698350,
         (const char* path, const char* mode, int flags));
CSF_FUNC(std::uint32_t, __cdecl, SceneFileExists, 0x00698B20,
         (const char* path, char flags));
CSF_FUNC(void, __fastcall, SelectListItem, 0x00716A60,
         (void* list, void* edx, int index));
CSF_FUNC(void*, __cdecl, GetGlobalSettings, 0x00756A10, ());

static void* __fastcall CMenuSceneSelect_PopulateSceneList(
    CMenuSceneSelect* self, void* /*edx*/)
{
    void* result = reinterpret_cast<void*>(self->field_0050);
    if (!result)
        return result;

    using ClearList = void (__fastcall*)(void*, void*);
    reinterpret_cast<ClearList>(CSF_AT(void**, result, 0)[0x298 / 4])(
        result, nullptr);

    ClearPageState(reinterpret_cast<std::uint8_t*>(self) + 0x38);

    void* settings = GetGlobalSettings();
    alignas(4) std::uint8_t currentScene[28];
    CSF_AT(std::uint32_t, currentScene, 0x18) = 15;
    CSF_AT(std::uint32_t, currentScene, 0x14) = 0;
    CSF_AT(char, currentScene, 0x04) = 0;

    CopyStringSubstring(
        currentScene, nullptr,
        reinterpret_cast<std::uint8_t*>(settings) + 0x48C, 0, 0xFFFFFFFFu);

    if (CompareStringSubstring(
            currentScene, nullptr, 0,
            CSF_AT(std::uint32_t, currentScene, 0x14),
            CSF_STR(0x0082E65C, ""), 0))
    {
        AddScene(self, nullptr, currentScene);
    }

    const std::uint32_t existsResult =
        SceneFileExists(CSF_STR(0x008419CC, "Menus/Escenas.txt"), 0);
    result = reinterpret_cast<void*>(existsResult);

    if (static_cast<std::uint8_t>(existsResult) != 0)
    {
        void* file = OpenSceneFile(
            CSF_STR(0x008419CC, "Menus/Escenas.txt"),
            CSF_STR(0x0083E130, "r"), 0);
        result = file;

        if (file)
        {
            char line[256];
            alignas(4) std::uint8_t lineString[28];
            using ReadLine = char* (__fastcall*)(void*, void*, char*, int);

            char* readResult =
                reinterpret_cast<ReadLine>(CSF_AT(void**, file, 0)[0x20 / 4])(
                    file, nullptr, line, 256);

            while (*readResult != *CSF_STR(0x0082E65C, ""))
            {
                char* selectedName = CSF_AT(char*, currentScene, 0x04);
                if (CSF_AT(std::uint32_t, currentScene, 0x18) < 16)
                    selectedName = reinterpret_cast<char*>(currentScene) + 4;

                if (!SceneNamesMatch(selectedName, line))
                {
                    CSF_AT(std::uint32_t, lineString, 0x18) = 15;
                    CSF_AT(std::uint32_t, lineString, 0x14) = 0;
                    CSF_AT(char, lineString, 0x04) = 0;

                    unsigned int length = 0;
                    while (line[length] != '\0')
                        ++length;

                    AssignStringRange(lineString, line, line, length);
                    AddScene(self, nullptr, lineString);

                    if (CSF_AT(std::uint32_t, lineString, 0x18) >= 16)
                    {
                        void* block = CSF_AT(void*, lineString, 0x04);
                        if (block)
                            FreeDebugMemory(block);
                    }

                    CSF_AT(std::uint32_t, lineString, 0x18) = 15;
                    CSF_AT(std::uint32_t, lineString, 0x14) = 0;
                    CSF_AT(char, lineString, 0x04) = 0;
                }

                readResult =
                    reinterpret_cast<ReadLine>(CSF_AT(void**, file, 0)[0x20 / 4])(
                        file, nullptr, line, 256);
            }

            SelectListItem(
                reinterpret_cast<void*>(self->field_0050), nullptr, 0);
            result = reinterpret_cast<void*>(CloseSceneFile(file));
        }
    }

    if (CSF_AT(std::uint32_t, currentScene, 0x18) >= 16)
    {
        result = CSF_AT(void*, currentScene, 0x04);
        if (result)
        {
            result = reinterpret_cast<void*>(
                static_cast<std::uint32_t>(FreeDebugMemory(result)));
        }
    }

    return result;
}

HOOK(0x0052FFD0, CMenuSceneSelect_PopulateSceneList, "auto");
