#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __fastcall, C_008414e8_ResetProgressLayoutState, 0x0052D0D0, (C_008414e8* self, void* edx));
CSF_FUNC(int, __fastcall, C_008414e8_CenterContentPanel, 0x0052C6E0, (C_008414e8* self, void* edx));
CSF_FUNC(int, __fastcall, C_008414e8_QueryRectFromAnchors, 0x0052C830, (C_008414e8* self, void* edx));
CSF_FUNC(int, __fastcall, C_008414e8_LayoutInputRows, 0x0052C890, (C_008414e8* self, void* edx));
CSF_FUNC(int, __fastcall, MenuProgressInput_ApplyCollapsedBounds, 0x0052C9D0, (C_008414e8* self, void* edx));

// Returns the number of consecutive filled slots (fields +0x24..+0x30) when a slot is empty,
// or 0 when the gating fields +0x34/+0x38 are empty. When all four slots are filled, it performs
// the layout pass, snapshots two bound pairs from the +0x34 provider, and applies collapsed bounds.
static int __fastcall C_008414e8_TryLayoutFullProgressPanel(C_008414e8* self, void* /*edx*/)
{
    using RectPairGetterFn = void*(__fastcall*)(void*, void*);

    if (self->field_0034 == 0)
        return 0;
    if (self->field_0038 == 0)
        return 0;

    int filledSlotCount = 0;
    while (true)
    {
        const uint32_t slotValue = CSF_AT(uint32_t, self, 0x24 + filledSlotCount * 4);
        if (slotValue == 0)
            return filledSlotCount;
        ++filledSlotCount;
        if (filledSlotCount >= 4)
            break;
    }

    C_008414e8_ResetProgressLayoutState(self, nullptr);
    C_008414e8_CenterContentPanel(self, nullptr);
    C_008414e8_QueryRectFromAnchors(self, nullptr);
    C_008414e8_LayoutInputRows(self, nullptr);

    void* rectSource = reinterpret_cast<void*>(self->field_0034);
    uint32_t* expandedRect = static_cast<uint32_t*>(
        reinterpret_cast<RectPairGetterFn>(CSF_AT(void**, rectSource, 0)[10])(rectSource, nullptr));
    self->field_0148 = expandedRect[0];
    self->field_014c = expandedRect[1];

    rectSource = reinterpret_cast<void*>(self->field_0034);
    uint32_t* collapsedRect = static_cast<uint32_t*>(
        reinterpret_cast<RectPairGetterFn>(CSF_AT(void**, rectSource, 0)[8])(rectSource, nullptr));
    self->field_0140 = collapsedRect[0];
    self->field_0144 = collapsedRect[1];

    return MenuProgressInput_ApplyCollapsedBounds(self, nullptr);
}

HOOK(0x0052D330, C_008414e8_TryLayoutFullProgressPanel, "auto");
