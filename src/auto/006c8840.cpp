#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_FUNC(int, __fastcall, ProcessWeaponContext, 0x006C8800, (void* self, void* edx));

[[maybe_unused]] static const auto kWeaponContextVtable = CSF_VTBL(0x006C8800);

static void __fastcall S_006c8800_UpdateWeaponContext(
    S_006c8800* self, void* /*edx*/, float elapsedTime)
{
    const bool active = self->field_0024 > g_fZero;

    // The original initializes this argument from self, then replaces only
    // its low byte with the active flag before passing the entire dword.
    const uint32_t stateArgument =
        (static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self)) & 0xFFFFFF00u) |
        static_cast<uint32_t>(active);

    void* object = reinterpret_cast<void*>(self->field_0004);
    if (object && !self->field_002c)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, object, 0)[0x34 / 4])(
                object, nullptr, stateArgument);
    }

    object = reinterpret_cast<void*>(self->field_0008);
    if (object)
    {
        if (active)
        {
            void* context =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

            reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, context, 0)[0x38 / 4])(
                    context, nullptr, self->field_0030);
        }

        if (!self->field_002c)
        {
            object = reinterpret_cast<void*>(self->field_0008);
            reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, object, 0)[0x3C / 4])(
                    object, nullptr, stateArgument);
        }
    }

    object = reinterpret_cast<void*>(self->field_000c);
    if (object)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, object, 0)[0x2C / 4])(
                object, nullptr, stateArgument);
    }

    if (active && !self->field_002d)
    {
        const float remainingTime = self->field_0024 - elapsedTime;
        self->field_0024 = remainingTime;
        if (remainingTime <= g_fZero)
            ProcessWeaponContext(self, nullptr);
    }

    self->field_002d = 0;
}

HOOK(0x006C8840, S_006c8800_UpdateWeaponContext, "auto");
