#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __stdcall Transfer32Bytes(std::uint32_t buffer, void* context)
{
    int size = 32;
    bool failed = false;
    if (CSF_AT(std::uint8_t, context, 0) == 0)
    {
        void* stream = CSF_AT(void*, context, 4);
        failed = !((bool(__fastcall*)(void*, void*, int*, void*))
            CSF_AT(void**, stream, 0)[120 / 4])(stream, nullptr, &size, context);
    }
    CSF_AT(std::uint8_t, context, 0) = failed;

    std::uint32_t index = 0;
    for (;;)
    {
        failed = false;
        if (CSF_AT(std::uint8_t, context, 0) == 0)
        {
            void* stream = CSF_AT(void*, context, 4);
            failed = ((int(__fastcall*)(void*, void*, std::uint32_t, void*))
                CSF_AT(void**, stream, 0)[76 / 4])(
                    stream, nullptr, buffer + index, context) == -1;
        }
        CSF_AT(std::uint8_t, context, 0) = failed;
        if (failed)
            break;

        if (++index >= 0x20)
        {
            void* stream = CSF_AT(void*, context, 4);
            if (((bool(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, stream, 0)[124 / 4])(stream, nullptr, context))
            {
                CSF_AT(std::uint8_t, context, 0) = 0;
                return 0;
            }
            break;
        }
    }
    CSF_AT(std::uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004912B0, Transfer32Bytes, "auto");
