#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original callees (bound by address).
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __cdecl, Options_SetFlag1, 0x00697300, (char a1));
CSF_FUNC(char, __cdecl, GetOptionsByte1, 0x00697320, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(char, __fastcall, C_00840af8_ApplyLoadedSaveFile, 0x00517480, (void* self, void* edx, void** a2, void* a4));

// Original: char __thiscall sub_517710(C_00840af8* this, int a2, S_005170e0* a3); ret 8
// C_00840af8 has no declared struct in auto_classes.h, so `this` is carried as void*.
static char __fastcall C_00840af8_TryLoadSaveFile(void* self, void* /*edx*/, int pathArg, void* applyArg)
{
    // Frame object [ebp-0x40], 0x40 bytes, filled by MenuExtras_InitializeFile / LoadFile.
    alignas(8) uint8_t fileBytes[0x40];

    if (!CSF_AT(uint8_t, GetGlobalValue_75FDF0(), 0xCC))
        return 0;

    // Records the file object's vtable slot (same value the initializer installs).
    CSF_AT(void*, fileBytes, 0) = CSF_VTBL(0x00692B10);
    MenuExtras_InitializeFile(fileBytes, nullptr);

    char savedOptionsByte = GetOptionsByte1();
    Options_SetFlag1(0);

    if (!MenuExtras_LoadFile(fileBytes, nullptr, reinterpret_cast<const char*>(static_cast<intptr_t>(pathArg))))
    {
        Options_SetFlag1(savedOptionsByte);
        MenuExtras_CloseFile(fileBytes, nullptr);
        return 0;
    }

    Options_SetFlag1(savedOptionsByte);
    char result = C_00840af8_ApplyLoadedSaveFile(self, nullptr, reinterpret_cast<void**>(fileBytes), applyArg);
    MenuExtras_CloseFile(fileBytes, nullptr);
    return result;
}

HOOK(0x517710, C_00840af8_TryLoadSaveFile, "auto");
