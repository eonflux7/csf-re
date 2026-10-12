#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, ClampContactComponent, 0x0040C920, (float* a1, int a2, int a3));
CSF_FUNC(int, __fastcall, C_00860288_SetColor, 0x006B3780, (void* self, void* edx, float* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00860288_GetColor, 0x006B40D0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00860288_BeginColorTransition, 0x006B45D0, (void* self, void* edx, int a2, float a3));

CSF_GLOBAL(float, g_ColorTransitionEpsilon, 0x00860194);
CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);

static uint32_t* __fastcall C_00860288_UpdateColorTransition(
    C_00860288* self, void* /*edx*/, float elapsed)
{
    uint32_t state = self->field_0088;
    if (state == 0 || state == 5)
        return reinterpret_cast<uint32_t*>(state);

    alignas(8) uint8_t colorBytes[0x14] = {};
    float remaining;
    std::memcpy(&remaining, &self->field_00ac, sizeof(remaining));
    remaining -= elapsed;
    CSF_AT(float, colorBytes, 0x10) = remaining;

    // The original also takes this path for an unordered comparison.
    if (!(remaining <= g_ColorTransitionEpsilon))
    {
        C_00860288_GetColor(
            self, nullptr, reinterpret_cast<uint32_t*>(colorBytes));

        float firstScratch =
            elapsed * CSF_AT(float, self, 0xB0) +
            CSF_AT(float, colorBytes, 0x00);
        float secondScratch = 1.0f;
        int lowerBound = 0;
        CSF_AT(float, colorBytes, 0x00) = static_cast<float>(
            ClampContactComponent(
                &firstScratch,
                reinterpret_cast<int>(&lowerBound),
                reinterpret_cast<int>(&secondScratch)));

        secondScratch =
            elapsed * CSF_AT(float, self, 0xB4) +
            CSF_AT(float, colorBytes, 0x04);
        firstScratch = 1.0f;
        lowerBound = 0;
        CSF_AT(float, colorBytes, 0x04) = static_cast<float>(
            ClampContactComponent(
                &secondScratch,
                reinterpret_cast<int>(&lowerBound),
                reinterpret_cast<int>(&firstScratch)));

        elapsed =
            elapsed * CSF_AT(float, self, 0xB8) +
            CSF_AT(float, colorBytes, 0x08);
        firstScratch = 1.0f;
        lowerBound = 0;
        CSF_AT(float, colorBytes, 0x08) = static_cast<float>(
            ClampContactComponent(
                &elapsed,
                reinterpret_cast<int>(&lowerBound),
                reinterpret_cast<int>(&firstScratch)));

        int result = C_00860288_SetColor(
            self, nullptr, reinterpret_cast<float*>(colorBytes));
        std::memcpy(
            &self->field_00ac, colorBytes + 0x10, sizeof(float));
        return reinterpret_cast<uint32_t*>(static_cast<uint32_t>(result));
    }

    self->field_00ac = 0;
    C_00860288_SetColor(
        self, nullptr, reinterpret_cast<float*>(&self->field_009c));

    state = self->field_0088;
    uint32_t* result = reinterpret_cast<uint32_t*>(state - 1);
    switch (state)
    {
    case 1:
    case 4:
        self->field_0088 = 3;
        return C_00860288_BeginColorTransition(
            self, nullptr, reinterpret_cast<int>(&g_ColorBlack), 0.5f);

    case 2:
        self->field_0088 = 0;
        break;

    case 3:
        CSF_AT(uint32_t, colorBytes, 0x00) = self->field_008c;
        CSF_AT(uint32_t, colorBytes, 0x04) = self->field_0090;
        CSF_AT(uint32_t, colorBytes, 0x08) = self->field_0094;
        CSF_AT(uint32_t, colorBytes, 0x0C) = self->field_0098;
        self->field_0088 = 4;
        return C_00860288_BeginColorTransition(
            self, nullptr, reinterpret_cast<int>(colorBytes), 0.5f);

    default:
        break;
    }
    return result;
}

HOOK(0x006B4720, C_00860288_UpdateColorTransition, "auto");
