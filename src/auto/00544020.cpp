#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuOptions_ResolvePanelAndApplyScale, 0x00548C40,
         (void* self, void* edx, int a2, int a3));

static int __fastcall CMenuGameOptionsPC_SyncSliderMouseValue(
    CMenuGameOptionsPC* self, void* /*edx*/, int a2, int a3)
{
    int result = CMenuOptions_ResolvePanelAndApplyScale(self, nullptr, a2, a3);

    uint32_t optionsHandle = self->field_001c;
    if (optionsHandle == 0)
        return result;

    void* options = reinterpret_cast<void*>(optionsHandle);
    uint32_t sliderHandle =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, options, 0)[0xFC / 4])(
                options, nullptr, CSF_STR(0x00843A2C, "SliderMouse"));

    self->field_0058 = sliderHandle;
    if (sliderHandle == 0)
        return 0;

    void* slider = reinterpret_cast<void*>(sliderHandle);
    void* sliderSource =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, slider, 0)[0x244 / 4])(slider, nullptr);

    if (sliderSource == nullptr)
    {
        void* sliderNow = reinterpret_cast<void*>(self->field_0058);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, sliderNow, 0)[0x260 / 4])(
                sliderNow, nullptr, 100, 1, 0);
    }

    const int* divisorPtr =
        reinterpret_cast<const int*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, sliderSource, 0)[0xA4 / 4])(
                sliderSource, nullptr);

    // The original reloads the slider before dereferencing the divisor.
    void* sliderForRange = reinterpret_cast<void*>(self->field_0058);
    int divisor = *divisorPtr;

    int quotient =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, sliderForRange, 0)[0x268 / 4])(
                sliderForRange, nullptr) / divisor;

    if (!(quotient % 2))
        --quotient;

    void* sliderFinal = reinterpret_cast<void*>(self->field_0058);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int)>(
        CSF_AT(void**, sliderFinal, 0)[0x260 / 4])(
            sliderFinal, nullptr, quotient, 1, 0);
}

HOOK(0x00544020, CMenuGameOptionsPC_SyncSliderMouseValue, "auto");
