#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_008318c8_ProcessFireAction, 0x004791E0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0,
         (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, WeaponFirearms_ApplyRecoil, 0x00495500,
         (void* self, void* edx, float a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0,
         (CGuiControl* self, void*));
CSF_FUNC(void, __fastcall, WeaponFirearms_NotifyColorAnimation, 0x00444B20,
         (void* self, void* edx, void* a2, void* a3));

static void __fastcall C_008318c8_ApplyFireRecoil(
    C_008318c8* self, void* /*edx*/, int action)
{
    C_008318c8_ProcessFireAction(self, nullptr, action);

    struct RecoilParameters
    {
        float yaw;
        float pitch;
        std::uint8_t randomizeYaw;
        std::uint8_t randomizePitch;
        std::uint8_t padding[2];
        float damping;
    };

    RecoilParameters recoil{};
    recoil.damping = 1.0f;

    void* handle = reinterpret_cast<void*>(self->field_0070);
    void* definition;
    if (handle)
    {
        definition = reinterpret_cast<void*>(
            Handle_GetValuePointer(handle, nullptr));
        recoil.yaw = CSF_AT(float, definition, 0xF8);

        definition = reinterpret_cast<void*>(
            Handle_GetValuePointer(
                reinterpret_cast<void*>(self->field_0070), nullptr));
        recoil.pitch = CSF_AT(float, definition, 0xFC);

        definition = reinterpret_cast<void*>(
            Handle_GetValuePointer(
                reinterpret_cast<void*>(self->field_0070), nullptr));
    }
    else
    {
        definition = reinterpret_cast<void*>(self->field_0040);
        recoil.yaw = CSF_AT(float, definition, 0xF8);
        recoil.pitch = CSF_AT(float, definition, 0xFC);
    }
    recoil.damping = CSF_AT(float, definition, 0x100);

    const std::uint8_t suppressRecoil =
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(action), 0x21);
    recoil.randomizeYaw = 1;
    if (!suppressRecoil)
    {
        // The prescribed binding labels this stack slot as float, but the
        // forwarded callee actually consumes a pointer to these parameters.
        const std::uint32_t parameterAddress =
            static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(&recoil));
        float parameterSlot;
        std::memcpy(&parameterSlot, &parameterAddress, sizeof(parameterSlot));
        WeaponFirearms_ApplyRecoil(
            reinterpret_cast<void*>(self->field_0004),
            nullptr, parameterSlot);
    }

    if (C_008319c0_GetSubObjectField74(
            reinterpret_cast<C_008319c0*>(self), nullptr) == 10)
    {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        auto* control =
            reinterpret_cast<CGuiControl*>(GetGlobalValue_8B0568());
        void* colorAnimation =
            reinterpret_cast<void*>(CGuiControl_GetColorAnim(control, nullptr));
        WeaponFirearms_NotifyColorAnimation(
            colorAnimation, nullptr, nullptr, owner);
    }
}

HOOK(0x0047A010, C_008318c8_ApplyFireRecoil, "auto");
