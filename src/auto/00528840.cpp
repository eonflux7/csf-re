#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);

static void SetIndicatorHidden(void* indicator, int hidden)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, indicator, 0)[0xA8 / 4])(
            indicator, nullptr, hidden);
}

static void __fastcall MenuMessageBox_UpdateIndicators(
    void* self, void* /*edx*/, float elapsed)
{
    void* first = CSF_AT(void*, self, 0x08);
    if (!first)
        return;
    if (!CSF_AT(void*, self, 0x0C))
        return;
    if (!CSF_AT(void*, self, 0x10))
        return;

    // The original compares the unrounded x87 sum after storing it as float.
    const double accumulated =
        static_cast<double>(elapsed) +
        static_cast<double>(CSF_AT(float, self, 0x18));
    CSF_AT(float, self, 0x18) = static_cast<float>(accumulated);

    if (!(accumulated >= static_cast<double>(g_ValueScale)))
        return;

    CSF_AT(float, self, 0x18) = 0.0f;
    SetIndicatorHidden(first, 1);
    SetIndicatorHidden(CSF_AT(void*, self, 0x0C), 1);
    SetIndicatorHidden(CSF_AT(void*, self, 0x10), 1);

    const uint32_t next = CSF_AT(uint32_t, self, 0x14) + 1u;
    CSF_AT(uint32_t, self, 0x14) = next;
    const uint32_t selected = next <= 2u ? next : 0u;
    CSF_AT(uint32_t, self, 0x14) = selected;

    SetIndicatorHidden(CSF_AT(void*, self, 0x08 + selected * 4u), 0);
}

HOOK(0x00528840, MenuMessageBox_UpdateIndicators, "auto");
