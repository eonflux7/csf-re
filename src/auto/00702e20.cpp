#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, C_008619e0_ConsumeFlagAndSetValue, 0x006E03E0, (void* self, void* edx, int a2, int a3, int a4));

using BinkSetSoundSystemFn = int (__stdcall*)(void* opener, uint32_t parameter);
using BinkSetMemoryFn = void (__stdcall*)(void* allocate, void* release);
using BinkSetSoundTrackFn = void (__stdcall*)(uint32_t count, int* tracks);
using BinkOpenFn = uint32_t (__stdcall*)(const char* name, uint32_t flags);
using BinkSetVolumeFn = void (__stdcall*)(uint32_t movie, uint32_t track, int32_t volume);

CSF_GLOBAL(BinkSetSoundSystemFn, g_BinkSetSoundSystem, 0x0082D3B8);
CSF_GLOBAL(void*, g_BinkOpenDirectSound, 0x0082D3BC);
CSF_GLOBAL(BinkSetMemoryFn, g_BinkSetMemory, 0x0082D3B4);
CSF_GLOBAL(BinkSetSoundTrackFn, g_BinkSetSoundTrack, 0x0082D3B0);
CSF_GLOBAL(BinkOpenFn, g_BinkOpen, 0x0082D3AC);
CSF_GLOBAL(BinkSetVolumeFn, g_BinkSetVolume, 0x0082D3A8);
CSF_GLOBAL(float, g_BinkVolumeScale, 0x00863190);

static char __fastcall C_00861a00_OpenVideo(
    C_00861a00* self, void* /*edx*/, int filename, int flags, float volume)
{
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[8 / 4])(self, nullptr);

    self->field_000c = 0;
    self->field_0014 = 0;
    self->field_0010 = 0;

    const char result = C_008619e0_ConsumeFlagAndSetValue(self, nullptr, filename, flags, 1065353216);
    if (!result)
        return result;

    const bool enableSound = (flags & 1) != 0;
    if (enableSound)
        g_BinkSetSoundSystem(g_BinkOpenDirectSound, 0);

    g_BinkSetMemory(
        reinterpret_cast<void*>(CSF_ADDR_FN(0x00403800)),
        reinterpret_cast<void*>(CSF_ADDR_FN(0x00702B60)));

    void* object = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* rasterSettings =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
    const int depth = CSF_AT(int, rasterSettings, 0x10);
    const int rasterFlags = depth != 32 ? 2560 : 1536;

    int soundTrack = (flags & 4) != 0;
    g_BinkSetSoundTrack(1, &soundTrack);

    void* filenameObject = reinterpret_cast<void*>(filename);
    const char* name;
    if (CSF_AT(uint32_t, filenameObject, 0x18) < 0x10u)
        name = reinterpret_cast<const char*>(filenameObject) + 4;
    else
        name = reinterpret_cast<const char*>(CSF_AT(uint32_t, filenameObject, 4));

    const uint32_t movie = g_BinkOpen(name, 0x4000);
    self->field_000c = movie;
    if (!movie)
        return 0;

    if (enableSound)
    {
        const int32_t scaledVolume =
            static_cast<int32_t>(static_cast<int64_t>(volume * g_BinkVolumeScale));
        g_BinkSetVolume(movie, 0, scaledVolume);
    }

    void* currentMovie = reinterpret_cast<void*>(self->field_000c);
    const int height = CSF_AT(int, currentMovie, 4);
    const int width = CSF_AT(int, currentMovie, 0);
    self->field_0014 = RwRasterCreate(width, height, depth, rasterFlags);
    return 1;
}

HOOK(0x00702E20, C_00861a00_OpenVideo, "auto");
