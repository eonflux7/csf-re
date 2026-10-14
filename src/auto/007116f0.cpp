#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int32_t, __fastcall, Audio_DirectSoundCaptureCreate, 0x007F78EC,
         (void* self, void* edx, void* deviceGuid, void** capture, void* outer));

static bool __fastcall Audio_CreateCaptureBuffer(
    void* self, void* /*edx*/, int32_t* parameters)
{
    if (Audio_DirectSoundCaptureCreate(
            self, nullptr, nullptr, &CSF_AT(void*, self, 0x4), nullptr) != 0)
        return false;

    alignas(8) uint8_t descriptorBytes[0x30];

    CSF_AT(uint16_t, descriptorBytes, 0x1E) =
        CSF_AT(uint16_t, parameters, 0x4);

    const uint16_t bitsPerSample = CSF_AT(uint16_t, parameters, 0xC);
    CSF_AT(uint16_t, descriptorBytes, 0x2A) = bitsPerSample;

    const int32_t channelBits = static_cast<int32_t>(
        static_cast<uint32_t>(CSF_AT(int32_t, parameters, 0x4)) *
        bitsPerSample);
    const uint16_t blockAlignment =
        static_cast<uint16_t>(channelBits / 8);

    const int32_t sampleRate = CSF_AT(int32_t, parameters, 0x8);
    const int32_t bufferBytes = CSF_AT(int32_t, parameters, 0x0);

    CSF_AT(uint16_t, descriptorBytes, 0x28) = blockAlignment;
    CSF_AT(uint32_t, descriptorBytes, 0x0) = 0;
    CSF_AT(uint32_t, descriptorBytes, 0x4) = 0;
    CSF_AT(uint32_t, descriptorBytes, 0x8) = 0;
    CSF_AT(uint32_t, descriptorBytes, 0xC) = 0;
    CSF_AT(uint32_t, descriptorBytes, 0x24) =
        static_cast<uint32_t>(sampleRate) * blockAlignment;
    CSF_AT(void*, descriptorBytes, 0x10) = nullptr;
    CSF_AT(uint32_t, descriptorBytes, 0x14) = 0;
    CSF_AT(void*, descriptorBytes, 0x10) = descriptorBytes + 0x1C;
    CSF_AT(uint32_t, descriptorBytes, 0x18) = 0;

    void* capture = CSF_AT(void*, self, 0x4);

    CSF_AT(uint16_t, descriptorBytes, 0x1C) = 1;
    CSF_AT(int32_t, descriptorBytes, 0x20) = sampleRate;
    CSF_AT(uint16_t, descriptorBytes, 0x2C) = 0;
    CSF_AT(uint32_t, descriptorBytes, 0x0) = 28;
    CSF_AT(int32_t, descriptorBytes, 0x8) = bufferBytes;

    // Original: mov eax,[edi+4]; mov ecx,[eax]; push 0; push edi+8; push edx; push eax; call [ecx+0xC].
    // The callee's vtable is loaded into ecx and the object, descriptor, outer slot and 0 are four stack
    // arguments (stdcall callee pops 0x10); edx still holds the descriptor address.
    void** captureVtbl = CSF_AT(void**, capture, 0x0);

    return reinterpret_cast<int32_t(__fastcall*)(
        void*, void*, void*, void*, void**, void*)>(
        captureVtbl[0xC / 4])(
            captureVtbl, descriptorBytes, capture, descriptorBytes, &CSF_AT(void*, self, 0x8), nullptr) == 0;
}

HOOK(0x007116F0, Audio_CreateCaptureBuffer, "auto");
