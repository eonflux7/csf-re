#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall S_00523cf0_ProcessSecondSubobject(
    S_00523cf0* self, void* /*edx*/, S_00691860* string)
{
    const uint32_t savedContext = CSF_AT(uint32_t, string, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, string, 0x00) == 0)
    {
        void* interfaceObject = reinterpret_cast<void*>(string->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
            CSF_AT(void**, interfaceObject, 0)[0x80 / 4])(
                interfaceObject, nullptr, 0, string);
    }

    CSF_AT(uint8_t, string, 0x00) = failed;
    CSF_AT(uint32_t, string, 0x1C) = 0;
    if (!failed)
    {
        reinterpret_cast<uint8_t*>(&string->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&string->field_0014)[2] = 0;
        Archive_SerializeString(reinterpret_cast<int>(self), string);
    }

    const bool shouldProcess = CSF_AT(uint8_t, string, 0x00) == 0;
    CSF_AT(uint32_t, string, 0x1C) = 0;
    if (shouldProcess)
    {
        void* interfaceObject = reinterpret_cast<void*>(string->field_0004);
        reinterpret_cast<uint8_t*>(&string->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&string->field_0014)[2] = 0;

        const int result =
            ((int(__fastcall*)(void*, void*, S_00523cf0*, S_00691860*))
                CSF_AT(void**, interfaceObject, 0)[0x54 / 4])(
                    interfaceObject, nullptr,
                    reinterpret_cast<S_00523cf0*>(
                        reinterpret_cast<uint8_t*>(self) + 0xC),
                    string);
        CSF_AT(uint8_t, string, 0x00) = result == -1;
    }

    const bool shouldFinish = CSF_AT(uint8_t, string, 0x00) == 0;
    CSF_AT(uint32_t, string, 0x1C) = savedContext;
    if (shouldFinish)
    {
        void* interfaceObject = reinterpret_cast<void*>(string->field_0004);
        const bool result =
            ((bool(__fastcall*)(void*, void*, S_00691860*))
                CSF_AT(void**, interfaceObject, 0)[0x7C / 4])(
                    interfaceObject, nullptr, string);
        if (result)
        {
            CSF_AT(uint8_t, string, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, string, 0x00) = 1;
    return 1;
}

HOOK(0x00449030, S_00523cf0_ProcessSecondSubobject, "auto");
