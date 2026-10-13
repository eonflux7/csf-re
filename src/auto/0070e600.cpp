#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

struct AudioFileCallbacks
{
    std::size_t (__cdecl* read)(void*, std::size_t, std::size_t, void*);
    int (__cdecl* seek)(void*, std::int64_t, int);
    int (__cdecl* close)(void*);
    long (__cdecl* tell)(void*);
};

CSF_FUNC(uint8_t*, __cdecl, Audio_ReplacePathCharacter, 0x00694BF0, (uint8_t* a1, char a2, char a3));
CSF_FUNC(char*, __cdecl, Audio_ApplyScenarioPath, 0x0069C400, (char* Str, int a2, int a3, int a4, int a5, int a6, int a7));
CSF_FUNC(int, __cdecl, Audio_GetSoundScenarioId, 0x0069C590, ());
CSF_FUNC(char*, __cdecl, __strupr, 0x007B302E, (char* String));
CSF_FUNC(int, __cdecl, ov_open_callbacks, 0x0081DD28,
         (void* source, void* file, const char* initial, long initialBytes, AudioFileCallbacks callbacks));
CSF_FUNC(double, __cdecl, ov_time_total, 0x0081DD22, (void* file, int link));
CSF_FUNC(int, __cdecl, ov_clear, 0x0081DD1C, (void* file));

using AudioCreateFileA = void* (__stdcall*)(
    const char*, std::uint32_t, std::uint32_t, void*,
    std::uint32_t, std::uint32_t, void*);
using AudioCloseHandle = int (__stdcall*)(void*);

CSF_GLOBAL(AudioCreateFileA, g_CreateFileAImport, 0x0082D09C);
CSF_GLOBAL(AudioCloseHandle, g_pfnCloseHandle, 0x0082D0E0);

static double __cdecl Audio_GetOggDuration(const char* path)
{
    alignas(8) std::uint8_t vorbisFileBytes[0x2D0];
    alignas(8) std::uint8_t filenameBytes[0x100];
    char* filename = reinterpret_cast<char*>(filenameBytes);

    std::memset(filename, 0, 250);
    CSF_AT(float, filenameBytes, 0xFC) = 0.0f;

    std::memcpy(filename, path, std::strlen(path) + 1);
    __strupr(filename);
    Audio_ReplacePathCharacter(filenameBytes, 47, 92);

    const int scenarioId = Audio_GetSoundScenarioId();
    Audio_ApplyScenarioPath(filename, 250, scenarioId, 0, 0, 0, 0);

    void* handle = g_CreateFileAImport(
        filename, 0x80000000, 1u, nullptr, 3u, 0, nullptr);

    if (handle != reinterpret_cast<void*>(static_cast<std::intptr_t>(-1)))
    {
        const AudioFileCallbacks callbacks{
            reinterpret_cast<decltype(AudioFileCallbacks::read)>(
                CSF_ADDR_FN(0x0070DF60)),
            reinterpret_cast<decltype(AudioFileCallbacks::seek)>(
                CSF_ADDR_FN(0x0070DFB0)),
            reinterpret_cast<decltype(AudioFileCallbacks::close)>(
                CSF_ADDR_FN(0x0070E010)),
            reinterpret_cast<decltype(AudioFileCallbacks::tell)>(
                CSF_ADDR_FN(0x0070E020))
        };

        if (ov_open_callbacks(handle, vorbisFileBytes, nullptr, 0, callbacks) == 0)
        {
            CSF_AT(float, filenameBytes, 0xFC) =
                static_cast<float>(ov_time_total(vorbisFileBytes, -1));
            ov_clear(vorbisFileBytes);
            return CSF_AT(float, filenameBytes, 0xFC);
        }

        g_pfnCloseHandle(handle);
    }

    return CSF_AT(float, filenameBytes, 0xFC);
}

HOOK(0x0070E600, Audio_GetOggDuration, "auto");
