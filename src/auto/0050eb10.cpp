#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, GameBddLoader_GetDefaultLoadResult, 0x00518940, ());
CSF_FUNC(int, __cdecl, GetValue_8B0574, 0x00518980, ());
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x00698350, (const char* path, const char* mode, int flags));
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static int __fastcall GameBddLoader_PrepareFrameDump(char* self, void* /*edx*/)
{
    alignas(8) std::uint8_t bufferBytes[0x104];
    std::memset(bufferBytes, 0, 0x100);
    char* buffer = reinterpret_cast<char*>(bufferBytes);

    int dumpMode = 0;
    if (!CSF_AT(char, self, 0xD0))
    {
        void* options = reinterpret_cast<void*>(GetGlobalInt());
        if (!CSF_AT(char, options, 0x4))
        {
            void* state = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            dumpMode = CSF_AT(int, state, 0x4AC);
        }
    }

    if (dumpMode == 1)
    {
        auto* state = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
        std::uint32_t capacity = CSF_AT(std::uint32_t, state, 0x8F4);
        auto* filenameStorage = state + 0x8DC;
        const char* source;
        if (capacity < 0x10)
            source = reinterpret_cast<const char*>(filenameStorage + 0x4);
        else
            source = CSF_AT(char*, filenameStorage, 0x4);

        char* destination = buffer;
        char character;
        do
        {
            character = *source++;
            *destination++ = character;
        } while (character);
    }
    else if (dumpMode == 2)
    {
        char savedOption = GetOptionsByteAt2();
        SceneLoader_SetFileOption(0);

        bool created = false;
        for (int index = 0; index < 1000; ++index)
        {
            if (created)
                break;

            _sprintf(buffer, CSF_STR(0x008405C0, "framedump%03d.fd"), index);
            void* file = OpenSceneFile(buffer, CSF_STR(0x0083E130, "r"), 0);
            if (file)
            {
                CloseSceneFile(file);
            }
            else
            {
                file = OpenSceneFile(buffer, CSF_STR(0x00840044, "w"), 0);
                if (file)
                {
                    CloseSceneFile(file);
                    created = true;
                }
            }

            if (index == 999)
                dumpMode = 0;
        }

        SceneLoader_SetFileOption(savedOption);
    }

    void* result = GameBddLoader_GetDefaultLoadResult();
    if (dumpMode)
        return GetValue_8B0574();

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x0050EB10, GameBddLoader_PrepareFrameDump, "auto");
