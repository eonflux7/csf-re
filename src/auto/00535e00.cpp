#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Gfx/Interfaz.txt: loads the interface scene file and initializes the HUD, dramatic windows and extras.

CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(std::uint8_t*, __fastcall, Hud_InitializeWidgets, 0x00436420, (void* self, void* edx, std::uint8_t* configuration));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(S_00692b80*, __fastcall, CommandoAnims_LoadDramaticWindows, 0x004FC4A0, (void* self, void* edx, S_00692b80* properties));
CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(void, __fastcall, sub_4F4AD0, 0x004F4AD0, (void* self, void* edx, void* a2));

static char Interface_LoadInterfazScene()
{
    // Stack-resident MenuExtras file object (0x3C bytes in the original frame).
    alignas(8) std::uint8_t fileBytes[0x3C];
    S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);

    (void)CSF_VTBL(0x00692B10);

    MenuExtras_InitializeFile(&file, nullptr);
    MenuExtras_LoadFile(reinterpret_cast<void*>(&file), nullptr, CSF_STR(0x00841F84, "Gfx/Interfaz.txt"));

    // field_0000 is a pointer-sized value stored as uint32_t.
    S_00692b80* sceneProperties = reinterpret_cast<S_00692b80*>(file.field_0000);

    int32_t hudOwner = GetGlobal8afc78();
    Hud_InitializeWidgets(reinterpret_cast<void*>(static_cast<std::intptr_t>(hudOwner)), nullptr,
                          reinterpret_cast<std::uint8_t*>(sceneProperties));

    int commandoAnimsOwner = GetGlobalValue_4FBDE0();
    CommandoAnims_LoadDramaticWindows(reinterpret_cast<void*>(static_cast<std::intptr_t>(commandoAnimsOwner)),
                                      nullptr, sceneProperties);

    int extrasOwner = GetGlobal8B0558();
    sub_4F4AD0(reinterpret_cast<void*>(static_cast<std::intptr_t>(extrasOwner)), nullptr, sceneProperties);

    return static_cast<char>(MenuExtras_CloseFile(reinterpret_cast<void*>(&file), nullptr));
}

HOOK(0x00535E00, Interface_LoadInterfazScene, "auto");
