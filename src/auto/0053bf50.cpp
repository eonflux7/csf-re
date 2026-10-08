#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original game functions called by this routine, bound by address.
CSF_FUNC(void*, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());
CSF_FUNC(void, __cdecl, CMenuProfiles_RebuildProfileListBox, 0x541090, (void* control));
CSF_FUNC(int*, __fastcall, GetScenarioCollection, 0x515CE0, (int* owner, void* edx));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x718660, (void* self, void* edx));

// The record vector is viewed as int[]: [1] = first record, [2] = end. Records are 80 bytes.
static int32_t MenuProfiles_RecordSpanBytes(const int32_t* recordVector)
{
    const uint32_t endAddress = static_cast<uint32_t>(recordVector[2]);
    const uint32_t beginAddress = static_cast<uint32_t>(recordVector[1]);
    return static_cast<int32_t>(endAddress - beginAddress);
}

static int32_t MenuProfiles_CountRecords(const int32_t* recordVector)
{
    if (recordVector[1] == 0)
        return 0;
    return MenuProfiles_RecordSpanBytes(recordVector) / 80;
}

// The original pushes the whole eax after `setne al`, so the high 24 bits of the
// count survive into the argument; only the low byte is the boolean flag.
static int MenuProfiles_ComposeLowByteFlag(int32_t count)
{
    const uint32_t highBits = static_cast<uint32_t>(count) & 0xFFFFFF00u;
    const uint32_t lowFlag = (count != 0) ? 1u : 0u;
    return static_cast<int>(highBits | lowFlag);
}

// Calls the widget's virtual SetEnabled-style slot (+0x68) with (flag, 1, 1)
// when the widget at self+widgetOffset is non-null.
static void MenuProfiles_SetWidgetEnabledByRecordCount(CMenuProfiles* self, uint32_t widgetOffset, int* recordOwner)
{
    if (!CSF_AT(void*, self, widgetOffset))
        return;

    int32_t* recordVector = GetScenarioCollection(recordOwner, nullptr);
    const int32_t recordCount = MenuProfiles_CountRecords(recordVector);

    void* widget = CSF_AT(void*, self, widgetOffset);
    void** widgetVtable = CSF_AT(void**, widget, 0);
    ((void(__fastcall*)(void*, void*, int, int, int))widgetVtable[26])(
        widget, nullptr, MenuProfiles_ComposeLowByteFlag(recordCount), 1, 1);
}

static int __fastcall CMenuProfiles_SyncButtonsToProfileCount(CMenuProfiles* self, void* /*edx*/)
{
    CMenuProfiles_RebuildProfileListBox(CSF_AT(void*, self, 0x14));

    int* recordOwner = CSF_AT(int*, GetGlobalValue_8B056C(), 8);

    MenuProfiles_SetWidgetEnabledByRecordCount(self, 0x18, recordOwner);
    MenuProfiles_SetWidgetEnabledByRecordCount(self, 0x20, recordOwner);
    MenuProfiles_SetWidgetEnabledByRecordCount(self, 0x14, recordOwner);

    int32_t* recordVector = GetScenarioCollection(recordOwner, nullptr);
    int result = 0;
    bool sliderLookupNeeded = false;

    if (recordVector[1] == 0) {
        result = static_cast<int>(reinterpret_cast<uintptr_t>(recordVector));
        sliderLookupNeeded = true;
    } else {
        const int32_t spanBytes = MenuProfiles_RecordSpanBytes(recordVector);
        // Low dword of 0x66666667 * span is what the original leaves in eax.
        const int64_t magicProduct = static_cast<int64_t>(1717986919) * static_cast<int64_t>(spanBytes);
        result = static_cast<int>(static_cast<uint32_t>(magicProduct));
        sliderLookupNeeded = (spanBytes / 80 == 0);
    }

    if (sliderLookupNeeded) {
        void* slider = CSF_AT(void*, self, 0x1C);
        if (slider)
            result = CGuiSlider_ForwardSelfToParent(slider, nullptr);
    }

    return result;
}

HOOK(0x0053BF50, CMenuProfiles_SyncButtonsToProfileCount, "auto");
