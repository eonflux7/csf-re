#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(bool, __cdecl, Audio_FinalizePath, 0x00694B70, (char* Str));
CSF_FUNC(uint8_t*, __cdecl, Audio_ReplacePathCharacter, 0x00694BF0, (uint8_t* a1, char a2, char a3));
CSF_FUNC(char*, __cdecl, Audio_ExpandPath, 0x0069C400, (char* Str, int a2, int a3, int a4, int a5, int a6, int a7));
CSF_FUNC(int, __cdecl, Audio_GetPathExpansionContext, 0x0069C590, ());
CSF_FUNC(char*, __stdcall, Audio_InsertStereoDirectory, 0x0070E080, (char* path));
CSF_FUNC(uintptr_t, __cdecl, __beginthread, 0x007B07F5, (void (__cdecl* startAddress)(void*), unsigned stackSize, void* arg));
CSF_FUNC(char*, __cdecl, __strupr, 0x007B302E, (char* String));

static void __fastcall C_008639b0_StartLoadingSound(
    C_008639b0* self, void* /*edx*/,
    const char* path, float volume, char option, char alternateMode)
{
    if (!path || !*path)
    {
        self->field_0045 = 1;
        return;
    }

    if (volume > g_TransitionOne)
        volume = 1.0f;
    else if (volume < g_fZero)
        volume = 0.0f;

    alignas(8) uint8_t pathBytes[0xFC];
    char* normalizedPath = reinterpret_cast<char*>(pathBytes);
    std::memset(normalizedPath, 0, 250);
    std::memcpy(normalizedPath, path, std::strlen(path) + 1);

    __strupr(normalizedPath);
    Audio_ReplacePathCharacter(pathBytes, 47, 92);

    if (self->field_0047)
        Audio_InsertStereoDirectory(normalizedPath);

    const int context = Audio_GetPathExpansionContext();
    int firstWord;
    int secondWord;
    int thirdWord;
    std::memcpy(&firstWord, pathBytes, sizeof(firstWord));
    std::memcpy(&secondWord, pathBytes + 4, sizeof(secondWord));
    std::memcpy(&thirdWord, pathBytes + 8, sizeof(thirdWord));
    Audio_ExpandPath(normalizedPath, 250, context, 0,
                     firstWord, secondWord, thirdWord);
    Audio_FinalizePath(normalizedPath);

    char* storedPath = reinterpret_cast<char*>(
        CoreMemory_AllocUntagged(std::strlen(normalizedPath) + 1));
    self->field_0024 = reinterpret_cast<uint32_t>(storedPath);
    std::strcpy(storedPath, normalizedPath);

    self->field_0039 = 0;
    self->field_0038 = 0;
    self->field_003b = option;
    self->field_0004 = volume;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0028 = alternateMode != 0 ? 0 : 2;

    __beginthread(
        reinterpret_cast<void (__cdecl*)(void*)>(CSF_ADDR_FN(0x0070EA60)),
        0, self);
}

HOOK(0x0070EA70, C_008639b0_StartLoadingSound, "auto");
