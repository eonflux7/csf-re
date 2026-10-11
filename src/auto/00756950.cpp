#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, IntArray_Reserve, 0x004FD1A0,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(unsigned int, __fastcall, IntArray_PushBack, 0x004FD290,
         (void* self, void* edx, uint32_t* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40,
         (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770,
         (void* self, void* edx, const char* path));

CSF_GLOBAL(const char*, g_pszKey_INTERFAZ_SELECCION, 0x008A8958);
CSF_GLOBAL(const char*, g_pszConfig_sonidos, 0x008A8AEC);

static const auto g_FileInitializerVtable = CSF_VTBL(0x00692B10);

static char __fastcall GameConfig_LoadInterfaceSoundIds(
    void* self, void* /*edx*/, const char* path)
{
    alignas(4) std::uint8_t file[0x3C];
    MenuExtras_InitializeFile(file, nullptr);

    if (!MenuExtras_LoadFile(file, nullptr, path))
    {
        MenuExtras_CloseFile(file, nullptr);
        return 0;
    }

    void* oldStorage = CSF_AT(void*, self, 0x08);
    auto* soundIds = reinterpret_cast<S_0046c410*>(
        static_cast<std::uint8_t*>(self) + 4);

    if (oldStorage)
        Mem_FreeDebug(oldStorage);

    soundIds->field_0004 = 0;
    soundIds->field_0008 = 0;
    soundIds->field_000c = 0;
    IntArray_Reserve(soundIds, nullptr, 101);

    auto* properties = CSF_AT(std::uint8_t*, file, 0x00);
    const char** keyword = &g_pszKey_INTERFAZ_SELECCION;
    do
    {
        std::uint8_t* property =
            FindProperty(properties, nullptr, const_cast<char*>(*keyword));

        std::uint32_t soundId;
        if (property)
        {
            soundId = static_cast<std::uint32_t>(
                ParseIntegerComponent(reinterpret_cast<std::uint32_t>(property)));
        }
        else
        {
            soundId = static_cast<std::uint32_t>(-1);
        }

        IntArray_PushBack(soundIds, nullptr, &soundId);
        ++keyword;
    }
    while (reinterpret_cast<std::intptr_t>(keyword) <
           reinterpret_cast<std::intptr_t>(&g_pszConfig_sonidos));

    MenuExtras_CloseFile(file, nullptr);
    return 1;
}

HOOK(0x00756950, GameConfig_LoadInterfaceSoundIds, "auto");
