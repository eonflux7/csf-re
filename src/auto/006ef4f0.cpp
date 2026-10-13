#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(unsigned int, __fastcall, GfxSkin_RenderObject, 0x006B3900, (S_006b30b0* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

static char __fastcall GfxUvAnim_RenderWaterSceneContents(
    S_006b30b0** self, void* /*edx*/)
{
    if (CSF_AT(std::uint32_t, self, 0x20) == 0)
        return 0;

    alignas(8) std::uint8_t transformBytes[0x10];
    alignas(8) std::uint8_t savedStateBytes[0x4];

    void* globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    CSF_AT(std::uint32_t, transformBytes, 0x0) =
        CSF_AT(std::uint32_t, globals, 0x124);
    CSF_AT(std::uint32_t, transformBytes, 0x4) =
        CSF_AT(std::uint32_t, globals, 0x128);
    CSF_AT(std::uint32_t, transformBytes, 0x8) =
        CSF_AT(std::uint32_t, globals, 0x12C);

    CSF_AT(int, savedStateBytes, 0) = 0;
    reinterpret_cast<void (__cdecl**)(int, int*)>(
        g_pRwEngineInstance)[9](14, reinterpret_cast<int*>(savedStateBytes));

    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    CSF_AT(void*, transformBytes, 0xC) = sceneController;

    if (CSF_AT(std::uint8_t,
               CSF_AT(void*, sceneController, 0xBC), 0x34))
    {
        reinterpret_cast<int (__cdecl**)(int, int)>(
            g_pRwEngineInstance)[8](14, 1);
    }

    if (CSF_AT(std::uint32_t, self, 0x20) > 0)
    {
        auto* entry = reinterpret_cast<S_006b30b0**>(
            reinterpret_cast<std::uint8_t*>(self) + 0x24);
        std::uint32_t index = 0;
        do
        {
            if (*entry)
            {
                S_006b30b0* object = *entry;
                void* transformOwner =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, object, 0)[0x18 / 4])(
                            object, nullptr);

                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, transformOwner, 0)[0x14 / 4])(
                        transformOwner, nullptr, transformBytes);

                object = *entry;
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[0x3C / 4])(
                        object, nullptr, 1);

                GfxSkin_RenderObject(*entry, nullptr);

                object = *entry;
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[0x3C / 4])(
                        object, nullptr, 0);
            }

            ++index;
            ++entry;
        } while (index < CSF_AT(std::uint32_t, self, 0x20));

        sceneController = CSF_AT(void*, transformBytes, 0xC);
    }

    char enabled = CSF_AT(char,
                          CSF_AT(void*, sceneController, 0xBC), 0x34);
    if (enabled)
    {
        return static_cast<char>(
            reinterpret_cast<int (__cdecl**)(int, int)>(
                g_pRwEngineInstance)[8](
                    14, CSF_AT(int, savedStateBytes, 0)));
    }
    return enabled;
}

HOOK(0x006EF4F0, GfxUvAnim_RenderWaterSceneContents, "auto");
