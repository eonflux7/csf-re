#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(char, __fastcall, CMenuSoundOptions_StoreSliderValueAndRefresh,
         0x0053CEE0, (float* self, void* edx, int updateArgument));

static int __fastcall CMenuSoundOptionsPC_UpdateXFiControls(
    CMenuSoundOptionsPC* self, void* /*edx*/, int updateArgument)
{
    CMenuSoundOptions_StoreSliderValueAndRefresh(
        reinterpret_cast<float*>(self), nullptr, updateArgument);

    int result = static_cast<int>(self->field_005c);
    if (!result)
        return result;

    result = static_cast<int>(self->field_0060);
    if (!result)
        return result;

    result = static_cast<int>(self->field_0064);
    if (!result)
        return result;

    // Capture full EAX even for calls whose predicates use only AL:
    // the original can return that unmodified register value.
    void* control = reinterpret_cast<void*>(self->field_005c);
    result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[0x6c / 4])(control, nullptr);

    if (static_cast<std::uint8_t>(result) != 0)
    {
        control = reinterpret_cast<void*>(self->field_005c);
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x2ac / 4])(control, nullptr);

        if (result == 0)
        {
            control = reinterpret_cast<void*>(self->field_0060);
            result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, control, 0)[0xac / 4])(control, nullptr);

            if (static_cast<std::uint8_t>(result) != 0)
            {
                control = reinterpret_cast<void*>(self->field_0060);
                reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, control, 0)[0xa8 / 4])(
                        control, nullptr, 0);

                control = reinterpret_cast<void*>(self->field_0064);
                reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, control, 0)[0xa8 / 4])(
                        control, nullptr, 1);
            }
        }
    }

    control = reinterpret_cast<void*>(self->field_005c);
    result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[0x6c / 4])(control, nullptr);

    if (static_cast<std::uint8_t>(result) != 0)
    {
        control = reinterpret_cast<void*>(self->field_005c);
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x2ac / 4])(control, nullptr);

        if (result == 0)
            return result;
    }

    control = reinterpret_cast<void*>(self->field_0064);
    result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[0xac / 4])(control, nullptr);

    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    control = reinterpret_cast<void*>(self->field_0064);
    reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, control, 0)[0xa8 / 4])(control, nullptr, 0);

    control = reinterpret_cast<void*>(self->field_0060);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, control, 0)[0xa8 / 4])(control, nullptr, 1);
}

HOOK(0x00548960, CMenuSoundOptionsPC_UpdateXFiControls, "auto");
