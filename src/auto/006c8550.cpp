#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, RtRandom, 0x005867D0, ());

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_FullRotationDegrees, 0x00830D04);

static unsigned int __fastcall WeaponFirearms_ActivateContext(
    void* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x24) =
        CSF_AT(std::uint32_t, self, 0x28);

    void* initialEffect = CSF_AT(void*, self, 0x04);
    CSF_AT(std::uint8_t, self, 0x2D) = 1;

    if (initialEffect)
    {
        const unsigned int variant = RtRandom() % 3u;
        void* effect = CSF_AT(void*, self, 0x04);
        void** vtable = CSF_AT(void**, effect, 0);
        const float rotations[3] = {0.0f, 15.0f, 345.0f};

        // 006c8597 pushes only the angle; edi is a saved register, not an argument.
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            vtable[0x24 / 4])(effect, nullptr, rotations[variant]);

        effect = CSF_AT(void*, self, 0x04);
        vtable = CSF_AT(void**, effect, 0);
        const unsigned int speedVariant = RtRandom() & 3u;
        const float speed =
            static_cast<double>(speedVariant) * g_flt_0082DC84 +
            g_TransitionOne;

        // 006c85c8 stores the speed into the one pushed stack argument.
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            vtable[0x2C / 4])(CSF_AT(void*, self, 0x04), nullptr, speed);

        effect = CSF_AT(void*, self, 0x04);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, effect, 0)[0x34 / 4])(effect, nullptr, 1);
    }

    const unsigned int context = CSF_AT(unsigned int, self, 0x08);
    if (context)
    {
        const unsigned int random = RtRandom();
        const unsigned int rotationOffset = random % 0x64 - 50u;
        // fild reads a signed dword; the negative path adds the unsigned bias.
        const auto signedOffset = static_cast<std::int32_t>(rotationOffset);
        double unsignedOffset = static_cast<double>(signedOffset);
        if (signedOffset < 0)
            unsignedOffset += g_UnsignedIntegerBias;
        CSF_AT(float, self, 0x30) =
            unsignedOffset *
            g_ProgressScale * g_FullRotationDegrees;
        return random / 0x64;
    }

    return context;
}

HOOK(0x006C8550, WeaponFirearms_ActivateContext, "auto");
