#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __cdecl, Options_SetFlag1, 0x00697300, (char a1));
CSF_FUNC(char, __cdecl, GetOptionsByte1, 0x00697320, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(char, __fastcall, C_0083fdf4_CheckLoadedFile, 0x0050BDA0, (void* self, void* edx, int a2));

// Loads an options file through MenuExtras, validates it and reports success.
static char __fastcall C_0083fdf4_LoadOptionsFile(C_0083fdf4* self, void* /*edx*/, S_00693740* a2)
{
    // Stack object used by the MenuExtras file API; occupies ebp-0x40 up to the local at ebp-0x4.
    alignas(8) uint8_t fileBytes[0x3C];
    CSF_AT(void*, fileBytes, 0) = (void*)CSF_VTBL(0x00692B10);
    MenuExtras_InitializeFile(fileBytes, nullptr);

    char result = 0;
    char savedOptionsByte = GetOptionsByte1();
    Options_SetFlag1(0);

    uint8_t* globalBase = reinterpret_cast<uint8_t*>(GetGlobalValue_75FDF0());
    uint8_t globalFlag = CSF_AT(uint8_t, globalBase, 0xCC);

    self->field_0014 = 0;
    if (globalFlag != 0) {
        // vtable slot 0x20 / 4 = 8
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[8])(self, nullptr);
        uint8_t loadedFile = MenuExtras_LoadFile(fileBytes, nullptr, reinterpret_cast<const char*>(a2));
        self->field_0014 = loadedFile;
        // vtable slot 0x24 / 4 = 9, vtable re-read after the load
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[9])(self, nullptr);
    }

    Options_SetFlag1(savedOptionsByte);

    if (self->field_0014 != 0 &&
        C_0083fdf4_CheckLoadedFile(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(fileBytes)))) {
        result = 1;
    }

    self->field_0014 = result;
    MenuExtras_CloseFile(fileBytes, nullptr);
    return result;
}

HOOK(0x0050BEE0, C_0083fdf4_LoadOptionsFile, "auto");
