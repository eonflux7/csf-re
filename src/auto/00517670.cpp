#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00840af8_ReadFileData, 0x00517480,
         (void* self, void* edx, void** a2, void* a4));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_OpenFileDescriptor, 0x00694660,
         (void* self, void* edx, int a2, uint32_t a3, char* a4, uint32_t* a5));
CSF_FUNC(char, __cdecl, Options_SetFlag1, 0x00697300, (char a1));
CSF_FUNC(char, __cdecl, GetOptionsByte1, 0x00697320, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

static char __fastcall C_00840af8_ReadFromFileDescriptor(
    C_00840af8* self, void* /*edx*/, int descriptorAddress,
    S_005170e0* output)
{
    auto* globalState = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalValue_75FDF0()));
    if (!CSF_AT(uint8_t, globalState, 0xCC))
        return 0;

    alignas(8) uint8_t fileBytes[0x40];
    // Preserve the packet's constructor-associated vtable binding.
    (void)CSF_VTBL(0x00692B10);
    MenuExtras_InitializeFile(fileBytes, nullptr);

    const char previousFlag = GetOptionsByte1();
    Options_SetFlag1(0);

    auto* descriptor = reinterpret_cast<void*>(
        static_cast<uintptr_t>(descriptorAddress));
    if (!MenuExtras_OpenFileDescriptor(
            fileBytes, nullptr,
            CSF_AT(int, descriptor, 0x0),
            CSF_AT(uint32_t, descriptor, 0x4),
            CSF_AT(char*, descriptor, 0x8),
            &CSF_AT(uint32_t, descriptor, 0xC)))
    {
        Options_SetFlag1(previousFlag);
        MenuExtras_CloseFile(fileBytes, nullptr);
        return 0;
    }

    Options_SetFlag1(previousFlag);
    const char result = C_00840af8_ReadFileData(
        self, nullptr, reinterpret_cast<void**>(fileBytes), output);
    MenuExtras_CloseFile(fileBytes, nullptr);
    return result;
}

HOOK(0x00517670, C_00840af8_ReadFromFileDescriptor, "auto");
