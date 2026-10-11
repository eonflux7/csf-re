#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall ProcessFieldsWithContext(void* self, void* /*edx*/, void* context)
{
    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);

    bool stopped = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        stopped = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, processor, 0)[0x80 / 4])(
                processor, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0x00) = stopped;

    static constexpr uint32_t fieldOffsets[] = {
        0, 4, 8, 12, 16, 20, 24, 28, 32, 36,
        40, 44, 48, 52, 56, 60, 64, 68, 72
    };

    for (uint32_t offset : fieldOffsets)
    {
        const bool processField = CSF_AT(uint8_t, context, 0x00) == 0;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (processField)
        {
            void* processor = CSF_AT(void*, context, 0x04);
            CSF_AT(uint8_t, context, 0x15) = 0;
            CSF_AT(uint8_t, context, 0x16) = 0;

            const int status = ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0)[0x48 / 4])(
                    processor, nullptr,
                    static_cast<uint8_t*>(self) + offset, context);
            CSF_AT(uint8_t, context, 0x00) = status == -1;
        }
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0x00) != 0)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    void* processor = CSF_AT(void*, context, 0x04);
    const bool result = ((bool(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, processor, 0)[0x7C / 4])(
            processor, nullptr, context);
    if (!result)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0x00) = 0;
    return result;
}

HOOK(0x00445290, ProcessFieldsWithContext, "auto");
