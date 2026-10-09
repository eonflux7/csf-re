#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int32_t, g_Value_8B0674, 0x008B0674);

CSF_FUNC(int, __fastcall, PushOverlayRect, 0x00522460,
         (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, GrowOverlayRecordArray, 0x00522C40,
         (void* self, void* edx, int a2));

static int __fastcall AudioSoundManager_SetOverlayRectValue(
    void* self, void* /*edx*/, float* rect, int value, int /*unused*/)
{
    const int index = PushOverlayRect(
        reinterpret_cast<void*>(g_Value_8B0674), nullptr, rect);

    if (index >= 0)
    {
        const uint32_t scaledIndex = static_cast<uint32_t>(index) * 3;
        auto* records = CSF_AT(uint8_t*, self, 0x20);
        CSF_AT(int, records + scaledIndex * 8, 0x04) = value;
        return static_cast<int>(scaledIndex);
    }

    const uint32_t rect0 = CSF_AT(uint32_t, rect, 0x00);
    void* manager = reinterpret_cast<void*>(g_Value_8B0674);
    const uint32_t rect1 = CSF_AT(uint32_t, rect, 0x04);
    const uint32_t recordId = CSF_AT(uint32_t, manager, 0x5C);
    const uint32_t rect2 = CSF_AT(uint32_t, rect, 0x08);
    const uint32_t rect3 = CSF_AT(uint32_t, rect, 0x0C);

    auto* array = static_cast<uint8_t*>(self) + 0x20;
    if (CSF_AT(uint32_t, array, 0x08) >= CSF_AT(uint32_t, array, 0x04))
    {
        GrowOverlayRecordArray(
            array, nullptr, CSF_AT(int, array, 0x0C));
    }

    const uint32_t count = CSF_AT(uint32_t, array, 0x08);
    auto* record = CSF_AT(uint8_t*, array, 0x00) + 24 * count;
    CSF_AT(uint32_t, array, 0x08) = count + 1;
    CSF_AT(uint32_t, record, 0x00) = recordId;
    CSF_AT(int, record, 0x04) = value;
    CSF_AT(uint32_t, record, 0x08) = rect0;
    CSF_AT(uint32_t, record, 0x0C) = rect1;
    CSF_AT(uint32_t, record, 0x10) = rect2;
    CSF_AT(uint32_t, record, 0x14) = rect3;

    return static_cast<int>(reinterpret_cast<uintptr_t>(record));
}

HOOK(0x00523320, AudioSoundManager_SetOverlayRectValue, "auto");
