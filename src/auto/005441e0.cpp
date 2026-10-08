#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: CMenuGameOptions::HandleOptionButtonEvent (__thiscall, 1 stack arg, ret 4)
CSF_FUNC(void, __fastcall, CMenuGameOptions_HandleOptionButtonEvent, 0x0053B340, (void* self, void* edx, void* event));

// Forwards slider position to the slider object's virtual methods and returns the
// result of its final virtual call. Original: CMenuGameOptionsPC::sub_5441E0, __thiscall, ret 4.
static int __fastcall CMenuGameOptionsPC_ApplySliderPosition(CMenuGameOptionsPC* self, void* /*edx*/, int a2)
{
    void* eventData = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));

    if (self->field_0024 == 0)
        return static_cast<int>(self->field_0024);

    CMenuGameOptions_HandleOptionButtonEvent(self, nullptr, eventData);

    uint32_t target = self->field_0058;
    if (target == 0)
        return static_cast<int>(target);
    if (target != CSF_AT(uint32_t, eventData, 0x4))
        return static_cast<int>(target);

    // slider object (vtable slot 0x3C): returns float maximum
    void* slider = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0024));
    float sliderRange = ((float(__fastcall*)(void*, void*))CSF_AT(void**, slider, 0)[0x3C / 4])(slider, nullptr);

    // target object: vtable slot 0x274 returns float, slot 0x264 returns int count
    void* targetObj = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0058));
    float targetValue = ((float(__fastcall*)(void*, void*))CSF_AT(void**, targetObj, 0)[0x274 / 4])(targetObj, nullptr);
    int stepCount = ((int(__fastcall*)(void*, void*))CSF_AT(void**, targetObj, 0)[0x264 / 4])(targetObj, nullptr);

    float sliderValue = static_cast<float>(
        static_cast<double>(targetValue) / static_cast<double>(stepCount - 1) * static_cast<double>(sliderRange));

    // slider object: vtable slot 0x30 (set value), then slot 0x44 (returned)
    slider = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0024));
    ((void(__fastcall*)(void*, void*, float))CSF_AT(void**, slider, 0)[0x30 / 4])(slider, nullptr, sliderValue);

    slider = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0024));
    return ((int(__fastcall*)(void*, void*, float))CSF_AT(void**, slider, 0)[0x44 / 4])(slider, nullptr, sliderValue);
}

HOOK(0x005441E0, CMenuGameOptionsPC_ApplySliderPosition, "auto");
