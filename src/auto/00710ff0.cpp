#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// DSBUFFERDESC.guid3DAlgorithm source GUID (all zero = default algorithm), four dwords
CSF_GLOBAL(uint32_t, g_Guid3DAlgo0, 0x863CC0);
CSF_GLOBAL(uint32_t, g_Guid3DAlgo1, 0x863CC4);
CSF_GLOBAL(uint32_t, g_Guid3DAlgo2, 0x863CC8);
CSF_GLOBAL(uint32_t, g_Guid3DAlgo3, 0x863CCC);

// KSDATAFORMAT_SUBTYPE_PCM {00000001-0000-0010-8000-00AA00389B71} in .rdata, four dwords
CSF_GLOBAL(uint32_t, g_SubtypePcm0, 0x863CB0);
CSF_GLOBAL(uint32_t, g_SubtypePcm1, 0x863CB4);
CSF_GLOBAL(uint32_t, g_SubtypePcm2, 0x863CB8);
CSF_GLOBAL(uint32_t, g_SubtypePcm3, 0x863CBC);

typedef int(__stdcall* CreateSoundBufferFn)(void* device, void* desc, void** outBuffer, void* outer);
typedef int(__stdcall* QueryInterfaceFn)(void* obj, const void* iid, void* out);
typedef uint32_t(__stdcall* ReleaseFn)(void* obj);

static bool __fastcall SoundBuffer_Create(uint8_t* self, void* /*edx*/, int* format, uint8_t* context)
{
    // The dword values the sub-format GUID at 0x863CB0 holds (what the original ends up storing).
    [[maybe_unused]] static constexpr uint32_t kSubtypePcmValue[4] = {1, 0x100000, 0xAA000080, 0x719B3800};

    // Frame mirrors the original: WAVEFORMATEXTENSIBLE at 0x00, DSBUFFERDESC at 0x28,
    // plain WAVEFORMATEX at 0x4C.
    alignas(8) uint8_t frame[0x64];
    void* device = CSF_AT(void*, context, 0x7C);

    CSF_AT(int, self, 8) = format[0];
    int bufferBytes = format[0];
    void* outBuffer = nullptr;

    uint8_t* desc = frame + 0x28;
    CSF_AT(int, desc, 0x0C) = 0;
    CSF_AT(int, desc, 0x08) = bufferBytes;
    CSF_AT(uint32_t, desc, 0x14) = g_Guid3DAlgo0;
    CSF_AT(uint32_t, desc, 0x18) = g_Guid3DAlgo1;
    CSF_AT(uint32_t, desc, 0x1C) = g_Guid3DAlgo2;
    CSF_AT(uint32_t, desc, 0x20) = g_Guid3DAlgo3;
    CSF_AT(int, desc, 0x00) = 36;
    CSF_AT(int, desc, 0x04) = 0x10080;

    int channels = format[1];
    if (channels <= 2) {
        int samplesPerSec = format[2];
        uint16_t bitsPerSample = CSF_AT(uint16_t, format, 0x0C);
        uint8_t* waveFormat = frame + 0x4C;
        uint16_t blockAlign = (uint16_t)((int)bitsPerSample * channels / 8);
        CSF_AT(uint16_t, waveFormat, 0x0E) = bitsPerSample;
        CSF_AT(uint16_t, waveFormat, 0x02) = (uint16_t)channels;
        CSF_AT(uint16_t, waveFormat, 0x0C) = blockAlign;
        CSF_AT(uint16_t, waveFormat, 0x00) = 1;
        CSF_AT(int, waveFormat, 0x04) = samplesPerSec;
        CSF_AT(int, waveFormat, 0x08) = samplesPerSec * (int)blockAlign;
        CSF_AT(uint16_t, waveFormat, 0x10) = 0;
        CSF_AT(void*, desc, 0x10) = waveFormat;
        CSF_AT(int, desc, 0x04) = 0x100C0;
    } else {
        uint16_t bitsPerSample = CSF_AT(uint16_t, format, 0x0C);
        int samplesPerSec = format[2];
        uint16_t blockAlign = (uint16_t)((int)bitsPerSample * channels / 8);
        CSF_AT(uint16_t, frame, 0x02) = (uint16_t)channels;
        CSF_AT(uint16_t, frame, 0x0C) = blockAlign;
        CSF_AT(int, frame, 0x08) = samplesPerSec * (int)blockAlign;
        CSF_AT(uint32_t, frame, 0x18) = g_SubtypePcm0;
        CSF_AT(uint32_t, frame, 0x1C) = g_SubtypePcm1;
        CSF_AT(int, frame, 0x04) = samplesPerSec;
        CSF_AT(uint16_t, frame, 0x00) = 0xFFFE;
        CSF_AT(uint16_t, frame, 0x0E) = bitsPerSample;
        CSF_AT(uint16_t, frame, 0x10) = 22;
        CSF_AT(uint16_t, frame, 0x12) = bitsPerSample;
        CSF_AT(uint32_t, frame, 0x20) = g_SubtypePcm2;
        CSF_AT(uint32_t, frame, 0x24) = g_SubtypePcm3;
        CSF_AT(uint32_t, frame, 0x14) = 63;
        CSF_AT(void*, desc, 0x10) = frame;
    }

    CreateSoundBufferFn createBuffer = (CreateSoundBufferFn)CSF_AT(void**, device, 0)[3];
    bool created = createBuffer(device, desc, &outBuffer, nullptr) == 0;
    if (created) {
        QueryInterfaceFn queryInterface = (QueryInterfaceFn)CSF_AT(void**, outBuffer, 0)[0];
        bool queried = queryInterface(outBuffer,
                                      reinterpret_cast<const void*>(CSF_ADDR_DATA(0x872C14)),
                                      self + 4) == 0;
        ReleaseFn release = (ReleaseFn)CSF_AT(void**, outBuffer, 0)[2];
        release(outBuffer);
        return queried;
    }
    return created;
}

HOOK(0x710FF0, SoundBuffer_Create, "auto");
