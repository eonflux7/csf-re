#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall AudioSoundManager_LoadModeResources(void* self, void* /*edx*/)
{
    auto* configuration = reinterpret_cast<std::uint8_t*>(GetValue8B0554()) + 0x54;
    int result = GetValue8B0554();
    const int mode = CSF_AT(int, configuration, 0x908);
    void* audio = CSF_AT(void*, reinterpret_cast<void*>(result), 0x1FD0);

    if (mode == 1)
    {
        void* scene = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0()) + 0x22A4;
        int resource = SceneLoader_GetResourceById(scene, nullptr, 62);
        CSF_AT(int, self, 0x24) = resource;
        result = reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, audio, 0)[0x1C / 4])(audio, nullptr, resource);

        if (result)
        {
            scene = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0()) + 0x22A4;
            resource = SceneLoader_GetResourceById(scene, nullptr, 64);
            CSF_AT(int, self, 0x1C) = resource;
            result = reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, audio, 0)[0x1C / 4])(audio, nullptr, resource);

            if (result)
            {
                scene = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0()) + 0x22A4;
                resource = SceneLoader_GetResourceById(scene, nullptr, 63);
                CSF_AT(int, self, 0x20) = resource;
                result = reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, audio, 0)[0x1C / 4])(audio, nullptr, resource);
                CSF_AT(std::uint8_t, self, 0x14) = static_cast<std::uint8_t>(result);
                return result;
            }
        }
    }
    else
    {
        if (mode != 2)
            return result;

        void* scene = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0()) + 0x22A4;
        int resource = SceneLoader_GetResourceById(scene, nullptr, 65);
        CSF_AT(int, self, 0x24) = resource;
        result = reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, audio, 0)[0x1C / 4])(audio, nullptr, resource);

        if (result)
        {
            scene = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0()) + 0x22A4;
            resource = SceneLoader_GetResourceById(scene, nullptr, 66);
            CSF_AT(int, self, 0x1C) = resource;
            result = reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, audio, 0)[0x1C / 4])(audio, nullptr, resource);
        }
        CSF_AT(int, self, 0x20) = -1;
    }

    CSF_AT(std::uint8_t, self, 0x14) = static_cast<std::uint8_t>(result);
    return result;
}

HOOK(0x00525730, AudioSoundManager_LoadModeResources, "auto");
