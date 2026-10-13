#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(bool, __fastcall, S_0070e170_ConfigureBuffer, 0x0070E170, (S_0070e170* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, S_0070e170_RunPlaybackLoop, 0x0070E830, (S_0070e170* self, void*));

using CreateFileAFunction = void* (__stdcall*)(
    const char*, std::uint32_t, std::uint32_t, void*,
    std::uint32_t, std::uint32_t, void*);
using CloseHandleFunction = int (__stdcall*)(void*);

CSF_GLOBAL(CreateFileAFunction, g_CreateFileAImport, 0x0082D09C);
CSF_GLOBAL(CloseHandleFunction, g_pfnCloseHandle, 0x0082D0E0);

using AudioReadCallback =
    std::size_t (__cdecl*)(void*, std::size_t, std::size_t, void*);
using AudioSeekCallback = int (__cdecl*)(void*, std::int64_t, int);
using AudioCloseCallback = int (__cdecl*)(void*);
using AudioTellCallback = long (__cdecl*)(void*);

struct AudioFileCallbacks
{
    AudioReadCallback read;
    AudioSeekCallback seek;
    AudioCloseCallback close;
    AudioTellCallback tell;
};

CSF_FUNC(int, __cdecl, ov_open_callbacks, 0x0081DD28,
         (void* file, void* decoder, const char* initialData,
          long initialBytes, AudioFileCallbacks callbacks));
CSF_FUNC(void*, __cdecl, ov_info, 0x0081DD34,
         (void* decoder, int link));
CSF_FUNC(double, __cdecl, ov_time_total, 0x0081DD22,
         (void* decoder, int link));
CSF_FUNC(int, __cdecl, ov_clear, 0x0081DD1C,
         (void* decoder));

// The packet also lists the ConfigureBuffer code address as a vtable.
[[maybe_unused]] static const auto g_ReportedConfigureBufferVtable =
    CSF_VTBL(0x0070E170);

static char __fastcall S_0070e170_ProcessEffectEntry(
    S_0070e170* self, void* /*edx*/)
{
    void* file = g_CreateFileAImport(
        reinterpret_cast<const char*>(self->field_0024),
        0x80000000, 1, nullptr, 3, 0, nullptr);

    char result = static_cast<char>(-1);
    if (file != reinterpret_cast<void*>(static_cast<std::intptr_t>(-1)))
    {
        self->field_003c = CoreMemory_AllocUntagged(720);

        const AudioFileCallbacks callbacks = {
            reinterpret_cast<AudioReadCallback>(CSF_ADDR_FN(0x0070DF60)),
            reinterpret_cast<AudioSeekCallback>(CSF_ADDR_FN(0x0070DFB0)),
            reinterpret_cast<AudioCloseCallback>(CSF_ADDR_FN(0x0070E010)),
            reinterpret_cast<AudioTellCallback>(CSF_ADDR_FN(0x0070E020))
        };

        if (ov_open_callbacks(
                file, reinterpret_cast<void*>(self->field_003c),
                nullptr, 0, callbacks))
        {
            g_pfnCloseHandle(file);
        }
        else
        {
            void* info = ov_info(
                reinterpret_cast<void*>(self->field_003c), -1);

            if (info)
            {
                const int sampleRate = CSF_AT(int, info, 0x8);
                const int channels = CSF_AT(int, info, 0x4);
                if (S_0070e170_ConfigureBuffer(
                        self, nullptr, channels, sampleRate))
                {
                    self->field_000c = static_cast<float>(
                        ov_time_total(
                            reinterpret_cast<void*>(self->field_003c), -1));
                    S_0070e170_RunPlaybackLoop(self, nullptr);
                }
            }

            void* buffer = reinterpret_cast<void*>(self->field_0018);
            if (buffer)
            {
                reinterpret_cast<void (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, buffer, 0)[3])(buffer, nullptr);
            }

            ov_clear(
                reinterpret_cast<void*>(self->field_003c));
        }

        result = static_cast<char>(self->field_003c);
        if (self->field_003c)
        {
            result = static_cast<char>(CoreMemory_Free(
                reinterpret_cast<void*>(self->field_003c)));
            self->field_003c = 0;
        }
    }

    self->field_0045 = 1;
    return result;
}

HOOK(0x0070E970, S_0070e170_ProcessEffectEntry, "auto");
