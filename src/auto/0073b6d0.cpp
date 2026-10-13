#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);

static int __fastcall BuildAlternatingKeySequence(void* self, void* /*edx*/, void* parameters)
{
    void* sequence = CSF_AT(void*, self, 0x08);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, sequence, 0)[0x1C / 4])(
        sequence, nullptr);

    alignas(8) uint8_t keyBytes[0x14];

    CSF_AT(uint32_t, keyBytes, 0x00) = CSF_AT(uint32_t, parameters, 0x24);
    CSF_AT(float, keyBytes, 0x10) = CSF_AT(float, parameters, 0x18) * g_ValueScale;
    CSF_AT(uint32_t, keyBytes, 0x08) = CSF_AT(uint32_t, parameters, 0x2C);
    CSF_AT(uint32_t, keyBytes, 0x0C) = CSF_AT(uint32_t, parameters, 0x30);
    CSF_AT(uint32_t, keyBytes, 0x04) = CSF_AT(uint32_t, parameters, 0x28);

    sequence = CSF_AT(void*, self, 0x08);
    ((int(__fastcall*)(void*, void*, void*, float))CSF_AT(void**, sequence, 0)[0x18 / 4])(
        sequence, nullptr, keyBytes, 0.0f);

    CSF_AT(uint32_t, keyBytes, 0x00) = CSF_AT(uint32_t, parameters, 0x34);
    float time = CSF_AT(float, keyBytes, 0x10);
    CSF_AT(uint32_t, keyBytes, 0x0C) = CSF_AT(uint32_t, parameters, 0x40);
    CSF_AT(uint32_t, keyBytes, 0x08) = CSF_AT(uint32_t, parameters, 0x3C);
    CSF_AT(uint32_t, keyBytes, 0x04) = CSF_AT(uint32_t, parameters, 0x38);

    sequence = CSF_AT(void*, self, 0x08);
    ((int(__fastcall*)(void*, void*, void*, float))CSF_AT(void**, sequence, 0)[0x18 / 4])(
        sequence, nullptr, keyBytes, time);

    time = CSF_AT(float, keyBytes, 0x10) + time;
    CSF_AT(uint32_t, keyBytes, 0x00) = CSF_AT(uint32_t, parameters, 0x24);
    CSF_AT(uint32_t, keyBytes, 0x0C) = CSF_AT(uint32_t, parameters, 0x30);
    CSF_AT(uint32_t, keyBytes, 0x04) = CSF_AT(uint32_t, parameters, 0x28);
    CSF_AT(uint32_t, keyBytes, 0x08) = CSF_AT(uint32_t, parameters, 0x2C);

    sequence = CSF_AT(void*, self, 0x08);
    ((int(__fastcall*)(void*, void*, void*, float))CSF_AT(void**, sequence, 0)[0x18 / 4])(
        sequence, nullptr, keyBytes, time);

    time = CSF_AT(float, keyBytes, 0x10) + time;
    CSF_AT(uint32_t, keyBytes, 0x00) = CSF_AT(uint32_t, parameters, 0x34);
    CSF_AT(uint32_t, keyBytes, 0x08) = CSF_AT(uint32_t, parameters, 0x3C);
    CSF_AT(uint32_t, keyBytes, 0x0C) = CSF_AT(uint32_t, parameters, 0x40);
    CSF_AT(uint32_t, keyBytes, 0x04) = CSF_AT(uint32_t, parameters, 0x38);

    sequence = CSF_AT(void*, self, 0x08);
    ((int(__fastcall*)(void*, void*, void*, float))CSF_AT(void**, sequence, 0)[0x18 / 4])(
        sequence, nullptr, keyBytes, time);

    CSF_AT(uint32_t, keyBytes, 0x00) = CSF_AT(uint32_t, parameters, 0x24);
    CSF_AT(uint32_t, keyBytes, 0x04) = CSF_AT(uint32_t, parameters, 0x28);
    CSF_AT(uint32_t, keyBytes, 0x0C) = CSF_AT(uint32_t, parameters, 0x30);
    CSF_AT(uint32_t, keyBytes, 0x08) = CSF_AT(uint32_t, parameters, 0x2C);
    time = CSF_AT(float, keyBytes, 0x10) + time;

    sequence = CSF_AT(void*, self, 0x08);
    int result =
        ((int(__fastcall*)(void*, void*, void*, float))CSF_AT(void**, sequence, 0)[0x18 / 4])(
            sequence, nullptr, keyBytes, time);

    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, parameters, 0x18);
    return result;
}

HOOK(0x0073B6D0, BuildAlternatingKeySequence, "auto");
