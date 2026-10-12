#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0,
         (void* self, void* edx, int entry));

static int __fastcall CMenuMpSessions_ConfigureSessionControls(
    CMenuMpSessions* self, void* /*edx*/)
{
    void* control = reinterpret_cast<void*>(self->field_0070);
    if (control)
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 0);

    control = reinterpret_cast<void*>(self->field_0034);
    if (control)
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 0);

    const uint32_t state = self->field_0018;
    if (state == 0 || state == 1 || state == 3)
    {
        int entry = MenuMessageBox_ResolveEntryByKey(1129132336, 1);
        C_00720DF0_ForwardCallToChildVtblSlot16(
            reinterpret_cast<void*>(self->field_0088), nullptr, entry);

        void** table = CSF_AT(
            void**, reinterpret_cast<void*>(self->field_0080), 0);
        entry = MenuMessageBox_ResolveEntryByKey(1129132596, 1);
        control = reinterpret_cast<void*>(self->field_0080);
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
            table[0x298 / 4])(control, nullptr, 0, entry);

        control = reinterpret_cast<void*>(self->field_0080);
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, control, 0)[0x29C / 4])(control, nullptr, 60, 0);

        control = reinterpret_cast<void*>(self->field_0080);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0x2A0 / 4])(
                control, nullptr, state == 0 ? 15 : 20);

        control = reinterpret_cast<void*>(self->field_0080);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0x2A8 / 4])(control, nullptr, 5);

        entry = MenuMessageBox_ResolveEntryByKey(
            state == 0 ? 1129132593 : 1129132337, 1);
        C_00720DF0_ForwardCallToChildVtblSlot16(
            reinterpret_cast<void*>(self->field_008c), nullptr, entry);

        table = CSF_AT(
            void**, reinterpret_cast<void*>(self->field_0084), 0);
        entry = MenuMessageBox_ResolveEntryByKey(1129132596, 1);
        control = reinterpret_cast<void*>(self->field_0084);
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
            table[0x298 / 4])(control, nullptr, 0, entry);

        control = reinterpret_cast<void*>(self->field_0084);
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, control, 0)[0x29C / 4])(
                control, nullptr, state == 3 ? 10 : 200, 0);

        control = reinterpret_cast<void*>(self->field_0084);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0x2A0 / 4])(
                control, nullptr, state == 0 ? 40 : (state == 1 ? 100 : 3));

        control = reinterpret_cast<void*>(self->field_0084);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0x2A8 / 4])(
                control, nullptr, state == 3 ? 1 : 10);

        if (state == 0)
        {
            control = reinterpret_cast<void*>(self->field_0090);
            if (control)
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 1);

            control = reinterpret_cast<void*>(self->field_0078);
            if (control)
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 1);
        }
        else
        {
            if (self->field_0090 && self->field_00ac)
            {
                void* source = reinterpret_cast<void*>(self->field_00ac);
                table = CSF_AT(
                    void**, reinterpret_cast<void*>(self->field_0090), 0);
                const int value =
                    reinterpret_cast<int (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, source, 0)[0x2AC / 4])(source, nullptr);

                // The original replaces only the low byte of the negated result.
                const uint32_t argument =
                    ((0u - static_cast<uint32_t>(value)) & 0xFFFFFF00u) |
                    static_cast<uint32_t>(value == 0);
                control = reinterpret_cast<void*>(self->field_0090);
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    table[0xA8 / 4])(
                        control, nullptr, static_cast<int32_t>(argument));
            }

            if (self->field_0078 && self->field_00ac)
            {
                void* source = reinterpret_cast<void*>(self->field_00ac);
                table = CSF_AT(
                    void**, reinterpret_cast<void*>(self->field_0078), 0);
                const int value =
                    reinterpret_cast<int (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, source, 0)[0x2AC / 4])(source, nullptr);

                const uint32_t argument =
                    ((0u - static_cast<uint32_t>(value)) & 0xFFFFFF00u) |
                    static_cast<uint32_t>(value == 0);
                control = reinterpret_cast<void*>(self->field_0078);
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    table[0xA8 / 4])(
                        control, nullptr, static_cast<int32_t>(argument));
            }
        }
    }

    control = reinterpret_cast<void*>(self->field_00a4);
    if (control)
        return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 0);
    return 0;
}

HOOK(0x00560BD0, CMenuMpSessions_ConfigureSessionControls, "auto");
