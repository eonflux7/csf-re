#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008611d4_ResetParticleState, 0x006D7F40,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008611d4_UpdateFirstParticleStream, 0x006D8B70,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008611d4_UpdateSecondParticleStream, 0x006D8BF0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008611d4_UpdateThirdParticleStream, 0x006D8C70,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008611d4_UpdateFourthParticleStream, 0x006D8D20,
         (void* self, void* edx, int a2));

static void C_008611d4_AccumulateParticleCount(C_008611d4* self, void* object)
{
    void* interfaceObject = static_cast<uint8_t*>(object) + 32;
    void* particleObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, interfaceObject, 0)[0x8 / 4])(
                interfaceObject, nullptr);

    void* particleInterface = static_cast<uint8_t*>(particleObject) + 32;
    void* particleData =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, particleInterface, 0)[0x24 / 4])(
                particleInterface, nullptr);

    if (particleData)
        CSF_AT(uint32_t, self, 0x130) += CSF_AT(uint32_t, particleData, 8);
}

static uint32_t __fastcall C_008611d4_UpdateParticleStreamsAndCount(
    C_008611d4* self, void* /*edx*/, int mode)
{
    if (mode == 0 || mode == 1 || mode == 2)
        C_008611d4_ResetParticleState(self, nullptr);

    if (mode == 1 || mode == 2)
    {
        uint32_t& firstBegin = mode == 1
            ? CSF_AT(uint32_t, self, 0x0C)
            : CSF_AT(uint32_t, self, 0x5C);
        uint32_t& firstEnd = mode == 1
            ? CSF_AT(uint32_t, self, 0x10)
            : CSF_AT(uint32_t, self, 0x60);

        for (uint32_t index = 0, byteOffset = 0; ; ++index, byteOffset += 28)
        {
            uint32_t begin = firstBegin;
            if (!begin ||
                index >= static_cast<uint32_t>(
                    static_cast<int32_t>(firstEnd - begin) / 28))
                break;

            C_008611d4_UpdateFirstParticleStream(
                self, nullptr, static_cast<int32_t>(firstBegin + byteOffset));
        }

        uint32_t& secondBegin = mode == 1
            ? CSF_AT(uint32_t, self, 0x1C)
            : CSF_AT(uint32_t, self, 0x6C);
        uint32_t& secondEnd = mode == 1
            ? CSF_AT(uint32_t, self, 0x20)
            : CSF_AT(uint32_t, self, 0x70);

        for (uint32_t index = 0, byteOffset = 0; ; ++index, byteOffset += 28)
        {
            uint32_t begin = secondBegin;
            if (!begin ||
                index >= static_cast<uint32_t>(
                    static_cast<int32_t>(secondEnd - begin) / 28))
                break;

            C_008611d4_UpdateSecondParticleStream(
                self, nullptr, static_cast<int32_t>(secondBegin + byteOffset));
        }

        uint32_t& thirdBegin = mode == 1
            ? CSF_AT(uint32_t, self, 0x2C)
            : CSF_AT(uint32_t, self, 0x7C);
        uint32_t& thirdEnd = mode == 1
            ? CSF_AT(uint32_t, self, 0x30)
            : CSF_AT(uint32_t, self, 0x80);

        for (uint32_t index = 0, byteOffset = 0; ; ++index, byteOffset += 28)
        {
            uint32_t begin = thirdBegin;
            if (!begin ||
                index >= static_cast<uint32_t>(
                    static_cast<int32_t>(thirdEnd - begin) / 28))
                break;

            C_008611d4_UpdateThirdParticleStream(
                self, nullptr, static_cast<int32_t>(thirdBegin + byteOffset));
        }

        uint32_t& fourthBegin = mode == 1
            ? CSF_AT(uint32_t, self, 0x3C)
            : CSF_AT(uint32_t, self, 0x8C);
        uint32_t& fourthEnd = mode == 1
            ? CSF_AT(uint32_t, self, 0x40)
            : CSF_AT(uint32_t, self, 0x90);

        for (uint32_t index = 0, byteOffset = 0; ; ++index, byteOffset += 28)
        {
            uint32_t begin = fourthBegin;
            if (!begin ||
                index >= static_cast<uint32_t>(
                    static_cast<int32_t>(fourthEnd - begin) / 28))
                break;

            C_008611d4_UpdateFourthParticleStream(
                self, nullptr, static_cast<int32_t>(fourthBegin + byteOffset));
        }
    }

    uint32_t initialCount = CSF_AT(uint32_t, self, 0xD0);
    CSF_AT(uint32_t, self, 0x130) = 0;

    if (initialCount)
    {
        uint32_t index = 0;
        do
        {
            C_008611d4_AccumulateParticleCount(
                self, CSF_AT(void**, self, 0xC8)[index]);
            ++index;
        }
        while (index < CSF_AT(uint32_t, self, 0xD0));
    }

    for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0xE0); ++index)
    {
        C_008611d4_AccumulateParticleCount(
            self, CSF_AT(void**, self, 0xD8)[index]);
    }

    for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0xB0); ++index)
    {
        C_008611d4_AccumulateParticleCount(
            self, CSF_AT(void**, self, 0xA8)[index]);
    }

    uint32_t result = CSF_AT(uint32_t, self, 0xC0);
    for (uint32_t index = 0; index < result; ++index)
    {
        C_008611d4_AccumulateParticleCount(
            self, CSF_AT(void**, self, 0xB8)[index]);
        result = CSF_AT(uint32_t, self, 0xC0);
    }
    return result;
}

HOOK(0x006D90A0, C_008611d4_UpdateParticleStreamsAndCount, "auto");
