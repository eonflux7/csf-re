#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuGameOptions_RefreshOptionControls, 0x0053B190, (CMenuGameOptions* self, void*));

static int __fastcall CMenuGameOptionsPC_RefreshMouseSlider(
    CMenuGameOptionsPC* self, void* /*edx*/)
{
    if (!self->field_0024)
        return 0;

    CMenuGameOptions_RefreshOptionControls(self, nullptr);

    if (!self->field_0058)
        return 0;

    using GetScalar = double (__fastcall*)(void*, void*);
    using GetCount = int (__fastcall*)(void*, void*, int);
    using SetPosition = void (__fastcall*)(void*, void*, int);
    using GetSelection = int (__fastcall*)(void*, void*);
    using ApplySelection = int (__fastcall*)(void*, void*, int);

    void* option = reinterpret_cast<void*>(self->field_0024);
    float minimum = static_cast<float>(
        reinterpret_cast<GetScalar>(CSF_AT(void**, option, 0)[0x38 / 4])(
            option, nullptr));

    option = reinterpret_cast<void*>(self->field_0024);
    float step = static_cast<float>(
        reinterpret_cast<GetScalar>(CSF_AT(void**, option, 0)[0x3C / 4])(
            option, nullptr));

    option = reinterpret_cast<void*>(self->field_0024);
    double current = reinterpret_cast<GetScalar>(
        CSF_AT(void**, option, 0)[0x34 / 4])(option, nullptr);
    float value = static_cast<float>(current);
    if (current <= minimum)
        value = 0.0f;

    void* slider = reinterpret_cast<void*>(self->field_0058);
    void** sliderVtable = CSF_AT(void**, slider, 0);
    int count = reinterpret_cast<GetCount>(
        CSF_AT(void**, slider, 0)[0x264 / 4])(slider, nullptr, 0);

    int position = static_cast<int>(
        static_cast<std::int64_t>((count - 1) * (value / step)));
    reinterpret_cast<SetPosition>(sliderVtable[0x26C / 4])(
        slider, nullptr, position);

    slider = reinterpret_cast<void*>(self->field_0058);
    int selection = reinterpret_cast<GetSelection>(
        CSF_AT(void**, slider, 0)[0x244 / 4])(slider, nullptr);
    if (!selection)
        return selection;

    slider = reinterpret_cast<void*>(self->field_0058);
    return reinterpret_cast<ApplySelection>(
        CSF_AT(void**, slider, 0)[0x158 / 4])(slider, nullptr, selection);
}

HOOK(0x00544130, CMenuGameOptionsPC_RefreshMouseSlider, "auto");
