#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveControl, 0x0052A040,
         (int source, char mode));

static int __fastcall CMenuMessageBox_InitializeFourControls(
    CMenuMessageBox* self, void* /*edx*/, int source)
{
    int resolved = MenuMessageBox_ResolveControl(source, 1);
    self->field_005c = static_cast<uint32_t>(resolved);

    if (resolved != 0)
    {
        void* control = reinterpret_cast<void*>(resolved);
        void* related =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, control, 0)[0x110 / 4])(control, nullptr);

        if (related != nullptr)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, related, 0)[0xA8 / 4])(related, nullptr, 0);
        }

        void* container = reinterpret_cast<void*>(self->field_0894);
        control = reinterpret_cast<void*>(self->field_005c);
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, container, 0)[0xF8 / 4])(container, nullptr, control);

        control = reinterpret_cast<void*>(self->field_005c);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 1);
    }

    int remaining = 3;
    unsigned index = 0;
    do
    {
        uint32_t& slot = index == 0
            ? self->field_0060
            : CSF_AT(uint32_t, self, 0x60 + index * 4);

        resolved = MenuMessageBox_ResolveControl(source, 1);
        slot = static_cast<uint32_t>(resolved);

        if (resolved != 0)
        {
            void* control = reinterpret_cast<void*>(resolved);
            void* related =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, control, 0)[0x110 / 4])(control, nullptr);

            if (related != nullptr)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, related, 0)[0xA8 / 4])(related, nullptr, 0);
            }

            void* container = reinterpret_cast<void*>(self->field_0894);
            control = reinterpret_cast<void*>(slot);
            ((void(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, container, 0)[0xF8 / 4])(container, nullptr, control);

            control = reinterpret_cast<void*>(slot);
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 1);
        }

        ++index;
        --remaining;
    } while (remaining != 0);

    return remaining;
}

HOOK(0x0052A0F0, CMenuMessageBox_InitializeFourControls, "auto");
