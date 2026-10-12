#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxScreenEffects_ResetResources, 0x006F9D90,
         (void* self, void* edx));
CSF_FUNC(bool, __fastcall, GfxScreenEffects_CreateResources, 0x006F9E60,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScreenEffects_InsertRepeatedElements, 0x006FAAD0,
         (void* self, void* edx, char* a1, unsigned int a3, void* arg8));
CSF_FUNC(uint32_t, __fastcall, S_004482c0_ResizeWithDefaults, 0x006FB000,
         (S_004482c0* self, void* edx, unsigned int count,
          int arg8, int arg12, int arg16, int arg20, int arg24));
CSF_FUNC(char, __fastcall, InitializeScreenEffect, 0x007091D0,
         (void* self, void* edx, int a2));

static bool __fastcall C_00862c74_Initialize(
    C_00862c74* self, void* /*edx*/, int configuration)
{
    GfxScreenEffects_ResetResources(self, nullptr);
    self->field_007c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(uint32_t, self, 0x70) = 0;
    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;
    self->field_0068 = 0;
    self->field_0030 = 1;
    self->field_0080 = 0;

    InitializeScreenEffect(self, nullptr, configuration);
    self->vSlot2C();
    const bool succeeded = GfxScreenEffects_CreateResources(self, nullptr);

    if (succeeded)
    {
        S_004482c0_ResizeWithDefaults(
            reinterpret_cast<S_004482c0*>(
                reinterpret_cast<uint8_t*>(self) + 0x84),
            nullptr, 4u, 0, 0, 0, 0, 0);

        const uint32_t begin = self->field_0088;
        const int count = begin
            ? static_cast<int32_t>(
                  CSF_AT(uint32_t, self, 0x8C) - begin) / 20
            : 0;

        for (int index = 0; index < count; ++index)
        {
            uint8_t* element = reinterpret_cast<uint8_t*>(self->field_0088)
                             + 20 * index;
            CSF_AT(uint32_t, element, 0x0) = 0;
            CSF_AT(uint32_t, element, 0x4) = 0;
            CSF_AT(float, element, 0x8) = -1.0f;
        }

        S_004482c0_ResizeWithDefaults(
            reinterpret_cast<S_004482c0*>(
                reinterpret_cast<uint8_t*>(self) + 0x94),
            nullptr, 2u, 0, 0, 0, 0, 0);
        S_004482c0_ResizeWithDefaults(
            reinterpret_cast<S_004482c0*>(
                reinterpret_cast<uint8_t*>(self) + 0xB4),
            nullptr, 4u, 0, 0, 0, 0, 0);

        const uint32_t recordsBegin = self->field_00a8;
        alignas(8) uint8_t recordBytes[0x48];
        std::memset(recordBytes + 0x20, 0, 32);
        std::memcpy(recordBytes, recordBytes + 0x20, 32);

        if (!recordsBegin)
        {
            GfxScreenEffects_InsertRepeatedElements(
                reinterpret_cast<uint8_t*>(self) + 0xA4, nullptr,
                reinterpret_cast<char*>(CSF_AT(uint32_t, self, 0xAC)),
                3, recordBytes);
        }
        else
        {
            const uint32_t recordsEnd = CSF_AT(uint32_t, self, 0xAC);
            const unsigned int recordCount =
                static_cast<int32_t>(recordsEnd - recordsBegin) >> 5;

            if (recordCount < 3)
            {
                const int currentCount = static_cast<int32_t>(
                    CSF_AT(uint32_t, self, 0xAC) - recordsBegin) >> 5;
                GfxScreenEffects_InsertRepeatedElements(
                    reinterpret_cast<uint8_t*>(self) + 0xA4, nullptr,
                    reinterpret_cast<char*>(CSF_AT(uint32_t, self, 0xAC)),
                    3 - currentCount, recordBytes);
            }
            else if (recordCount > 3)
            {
                const uint32_t newEnd = recordsBegin + 96;
                if (newEnd != recordsEnd)
                    CSF_AT(uint32_t, self, 0xAC) = newEnd;
            }
        }
    }

    self->field_0008 = 1;
    if (!succeeded)
    {
        GfxScreenEffects_ResetResources(self, nullptr);
        return false;
    }
    return succeeded;
}

HOOK(0x006FB0B0, C_00862c74_Initialize, "auto");
