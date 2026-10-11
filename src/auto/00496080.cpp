#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_692200, 0x00692200, (void* self, void* edx, unsigned int a2, int a3, int a4, char a5));

static int __cdecl Archive_SerializeString(int archiveAddress, S_00691860* string)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    S_00691860* original = string;

    if (static_cast<uint8_t>(original->field_0014))
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, archive, 0)[0x10 / 4])(archive, nullptr);

        string = reinterpret_cast<S_00691860*>(
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, archive, 0)[0x14 / 4])(archive, nullptr));

        bool failed = false;
        if (!original->pad_0000[0])
        {
            void* handler = reinterpret_cast<void*>(original->field_0004);
            failed = !((bool(__fastcall*)(void*, void*, S_00691860**, S_00691860*))
                CSF_AT(void**, handler, 0)[0x8C / 4])(
                    handler, nullptr, &string, original);
        }

        void* destination = reinterpret_cast<void*>(original->field_000c);
        S_00691860* value = string;
        original->pad_0000[0] = failed;

        ((void(__fastcall*)(void*, void*, S_00691860*))
            CSF_AT(void**, destination, 0)[8 / 4])(
                destination, nullptr, value);
    }
    else
    {
        bool failed = false;
        if (!original->pad_0000[0])
        {
            void* handler = reinterpret_cast<void*>(original->field_0004);
            failed = !((bool(__fastcall*)(void*, void*, S_00691860**, S_00691860*))
                CSF_AT(void**, handler, 0)[0x8C / 4])(
                    handler, nullptr, &string, original);
        }

        original->pad_0000[0] = failed;

        int archiveValue = ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, archive, 0)[0x14 / 4])(archive, nullptr);

        sub_692200(
            CSF_AT(void*, original, 0x08),
            nullptr,
            reinterpret_cast<unsigned int>(string),
            archiveValue,
            reinterpret_cast<int>(original),
            0);
    }

    return ((int(__fastcall*)(void*, void*, S_00691860*))
        CSF_AT(void**, archive, 0)[8 / 4])(archive, nullptr, original);
}

HOOK(0x00496080, Archive_SerializeString, "auto");

