#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CInputDirectInput_ReleaseDeviceAndInterface(
    CInputDirectInput* self, void* /*edx*/)
{
    if (self->field_0024)
    {
        void* device = reinterpret_cast<void*>(self->field_0024);
        void** deviceVtable = CSF_AT(void**, device, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            deviceVtable[0x20 / 4])(deviceVtable, nullptr, device);

        device = reinterpret_cast<void*>(self->field_0024);
        void** releaseVtable = CSF_AT(void**, device, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            releaseVtable[0x8 / 4])(deviceVtable, releaseVtable, device);
        self->field_0024 = 0;
    }

    if (self->field_0020)
    {
        void* device = reinterpret_cast<void*>(self->field_0020);
        void** deviceVtable = CSF_AT(void**, device, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            deviceVtable[0x20 / 4])(deviceVtable, nullptr, device);

        device = reinterpret_cast<void*>(self->field_0020);
        void** releaseVtable = CSF_AT(void**, device, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            releaseVtable[0x8 / 4])(deviceVtable, releaseVtable, device);
        self->field_0020 = 0;
    }

    int result = 0;
    if (self->field_001c)
    {
        void* directInput = reinterpret_cast<void*>(self->field_001c);
        void** interfaceVtable = CSF_AT(void**, directInput, 0);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            interfaceVtable[0x8 / 4])(interfaceVtable, nullptr, directInput);
        self->field_001c = 0;
    }
    return result;
}

HOOK(0x712F00, CInputDirectInput_ReleaseDeviceAndInterface, "auto");
