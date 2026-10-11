#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall SaveGameManager_SerializeSlots(void* self, void* /*edx*/, void* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
                       CSF_AT(void**, processor, 0)[0x80 / 4])(
            processor, nullptr, 0, context);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, processor, 0)[0x58 / 4])(
                processor, nullptr, self, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, processor, 0)[0x58 / 4])(
                processor, nullptr, static_cast<uint8_t*>(self) + 4, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, processor, 0)[0x58 / 4])(
                processor, nullptr, static_cast<uint8_t*>(self) + 8, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, processor, 0)[0x58 / 4])(
                processor, nullptr, static_cast<uint8_t*>(self) + 12, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, processor, 0)[0x58 / 4])(
                processor, nullptr, static_cast<uint8_t*>(self) + 16, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, processor, 0)[0x54 / 4])(
                processor, nullptr, static_cast<uint8_t*>(self) + 20, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (!failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, void*))
                 CSF_AT(void**, processor, 0)[0x7C / 4])(
                processor, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00511E40, SaveGameManager_SerializeSlots, "auto");
