#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0,
         (void* self, void* edx));

static int __fastcall CMenuProgressBar_BindProgressControl(
    CMenuProgressBar* self, void* /*edx*/, int context, int /*unused*/)
{
    self->field_001c = static_cast<uint32_t>(context);

    MenuProgressInput_UpdateGlobalState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    void* screen = reinterpret_cast<void*>(self->field_0020);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    screen = reinterpret_cast<void*>(self->field_0020);
    void* progressControl =
        reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screen, 0)[0xFC / 4])(
                screen, nullptr, CSF_STR(0x00841538, "BarraProgreso"));

    self->field_0010 =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(progressControl));

    if (progressControl)
    {
        void* valueProvider =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, progressControl, 0)[0x1CC / 4])(
                    progressControl, nullptr);

        if (valueProvider && self->field_0014 == UINT32_MAX)
        {
            void* valueAddress =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, valueProvider, 0)[0x28 / 4])(
                        valueProvider, nullptr);
            self->field_0014 = CSF_AT(uint32_t, valueAddress, 0);
        }
    }

    return static_cast<int>(self->field_0020);
}

HOOK(0x0052D550, CMenuProgressBar_BindProgressControl, "auto");
