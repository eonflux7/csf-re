#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_4276C0, 0x004276C0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, sub_4276F0, 0x004276F0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, UpdateColorFade, 0x00427FD0, (float* self, void* , float deltaTime));
CSF_FUNC(void, __fastcall, sub_4280A0, 0x004280A0, (void* self, void* edx, float a2));

// Original's tail-jump table at 0x42833C references S_004276c0's vtable slot address 0x4276C0.
static int __fastcall HudObjectives_UpdateAuxiliaryWidget(S_004276c0* self, void* /*edx*/, float a2)
{
    // Vtable reference bound by address (base-class store equivalent in the original's chain).
    // Code-address annotation: function value, not a vtable.
    const void* vtblRef = reinterpret_cast<void**>(CSF_ADDR_FN(0x004276C0));
    (void)vtblRef;

    if (self->field_0008 && self->field_0009)
    {
        switch (self->field_0000)
        {
        case 0u:
            return UpdateColorFade(reinterpret_cast<float*>(self), nullptr, a2);
        case 1u:
            sub_4280A0(self, nullptr, a2);
            return 0;
        case 2u:
            sub_4276C0(self, nullptr, a2);
            return 0;
        case 3u:
            return sub_4276F0(self, nullptr, a2);
        default:
            // 0x428300: `mov eax, [ecx]; cmp eax, 3; ja 0x428337` leaves the whole field in eax
            return static_cast<int>(self->field_0000);
        }
    }
    return 0;
}

HOOK(0x428300, HudObjectives_UpdateAuxiliaryWidget, "auto");
