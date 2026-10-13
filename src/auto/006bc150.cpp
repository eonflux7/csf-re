#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(int, __cdecl, CoreFiles_CheckPageAvailability, 0x00698B20, (const char* path, char bypassList));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, RegisterEnvironmentTexturePaths, 0x006BC0F0, ());

static char __fastcall C_008605d8_LoadTexturePage(
    C_008605d8* /*self*/, void* /*edx*/, void* descriptor)
{
    const char* auxiliaryPath = CSF_AT(const char*, descriptor, 0x08);
    if (auxiliaryPath && *auxiliaryPath)
    {
        void* accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));

        void* registry = reinterpret_cast<void*>(
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr));

        ((void(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, registry, 0)[0x10 / 4])(
                registry, nullptr,
                CSF_AT(const char*, descriptor, 0x08));
    }

    alignas(8) char path[0x100];
    const char* source = CSF_AT(const char*, descriptor, 0x04);
    char* destination = path;
    char character;
    do
    {
        character = *source++;
        *destination++ = character;
    } while (character);

    Path_ReplaceExtension(
        path, 256, const_cast<char*>(CSF_STR(0x008605D0, ".txl")));

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x04))
    {
        CoreFiles_CheckPageAvailability(path, 0);
    }
    else
    {
        void* accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));

        void* registry = reinterpret_cast<void*>(
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr));

        ((void(__fastcall*)(void*, void*, char*))
            CSF_AT(void**, registry, 0)[0x34 / 4])(
                registry, nullptr, path);
    }

    RegisterEnvironmentTexturePaths();
    return 1;
}

HOOK(0x006BC150, C_008605d8_LoadTexturePage, "auto");
