#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t SetIndicatorHidden(void* indicator, int hidden)
{
    return reinterpret_cast<uint32_t(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, indicator, 0)[0xA8 / 4])(
            indicator, nullptr, hidden);
}

static uint8_t __fastcall MenuMessageBox_AdvanceActiveIndicator(
    void* self, void* /*edx*/)
{
    uint8_t result = CSF_AT(uint8_t, self, 0x8B8);
    if (result)
        return result;

    result = CSF_AT(uint8_t, self, 0x8B9);
    if (!result)
        return result;

    void* first = CSF_AT(void*, self, 0x6C);
    if (!first)
        return result;
    if (!CSF_AT(void*, self, 0x70))
        return 0;
    if (!CSF_AT(void*, self, 0x74))
        return 0;

    SetIndicatorHidden(first, 1);
    SetIndicatorHidden(CSF_AT(void*, self, 0x70), 1);
    SetIndicatorHidden(CSF_AT(void*, self, 0x74), 1);

    const uint32_t next = CSF_AT(uint32_t, self, 0x9A8) + 1u;
    CSF_AT(uint32_t, self, 0x9A8) = next;
    const uint32_t selected = next <= 2u ? next : 0u;
    CSF_AT(uint32_t, self, 0x9A8) = selected;

    return static_cast<uint8_t>(
        SetIndicatorHidden(
            CSF_AT(void*, self, 0x6C + selected * 4u), 0));
}

HOOK(0x00529E50, MenuMessageBox_AdvanceActiveIndicator, "auto");
