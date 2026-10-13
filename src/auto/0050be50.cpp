#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_OpenFileDescriptor, 0x00694660, (void* self, void* edx, int a2, uint32_t a3, char* a4, uint32_t* a5));
CSF_FUNC(char, __cdecl, Options_SetFlag1, 0x00697300, (char a1));
CSF_FUNC(char, __cdecl, GetOptionsByte1, 0x00697320, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(char, __fastcall, C_0083fdf4_ApplyOptionsSections, 0x0050BDA0, (void* self, void* edx, int a2));

// The file-state object is initialized by MenuExtras_InitializeFile, which stores its vtable.
[[maybe_unused]] static const auto kFileStateVtbl = CSF_VTBL(0x00692B10);

static char __fastcall C_0083fdf4_ReadOptionsFile(C_0083fdf4* self, void* /*edx*/, uint32_t* a2)
{
    // Stack object at [ebp-0x40], 0x40 bytes.
    alignas(8) uint8_t fileStateBytes[0x40];
    S_00692b10& fileState = *reinterpret_cast<S_00692b10*>(fileStateBytes);

    MenuExtras_InitializeFile(&fileState, nullptr);
    char savedOptionsByte = GetOptionsByte1();
    Options_SetFlag1(0);

    uint8_t* globalsBase = reinterpret_cast<uint8_t*>(GetGlobalValue_75FDF0());
    uint8_t openedOk;
    if (CSF_AT(uint8_t, globalsBase, 0xCC))
    {
        openedOk = MenuExtras_OpenFileDescriptor(&fileState, nullptr,
            static_cast<int>(a2[0]),
            a2[1],
            reinterpret_cast<char*>(a2[2]),
            &a2[3]);
    }
    else
    {
        openedOk = 0;
    }

    self->field_0014 = openedOk;
    if (openedOk)
    {
        self->field_0014 = C_0083fdf4_ApplyOptionsSections(self, nullptr,
            reinterpret_cast<int>(&fileState));
    }

    Options_SetFlag1(savedOptionsByte);
    uint8_t result = self->field_0014;
    MenuExtras_CloseFile(&fileState, nullptr);
    return static_cast<char>(result);
}

HOOK(0x0050BE50, C_0083fdf4_ReadOptionsFile, "auto");
