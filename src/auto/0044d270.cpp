#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(unsigned int, __stdcall, ObjectState_ProcessEmbeddedFields,
         0x0044CE40, (void* a1, int a2));

static int __fastcall ObjectState_ProcessFields(void* self, void* /*edx*/, void* state)
{
    bool stopped = false;
    if (CSF_AT(uint8_t, state, 0) == 0)
    {
        void* processor = CSF_AT(void*, state, 4);
        stopped = !((bool(__fastcall*)(void*, void*, int, void*))
                        CSF_AT(void**, processor, 0)[0x80 / 4])(
            processor, nullptr, 0, state);
    }

    uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);
    CSF_AT(uint8_t, state, 0) = stopped;
    CSF_AT(uint32_t, state, 0x1C) = 0;

    if (!stopped)
    {
        void* processor = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, processor, 0)[0x44 / 4])(
                processor, nullptr, static_cast<uint8_t*>(self) + 4, state) == -1;
    }

    bool processField = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (processField)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        ObjectState_ProcessEmbeddedFields(
            static_cast<uint8_t*>(self) + 8,
            static_cast<int>(reinterpret_cast<intptr_t>(state)));
    }

    processField = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (processField)
    {
        void* processor = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, processor, 0)[0x58 / 4])(
                processor, nullptr, static_cast<uint8_t*>(self) + 0x18, state) == -1;
    }

    processField = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (processField)
    {
        void* processor = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, processor, 0)[0x54 / 4])(
                processor, nullptr, static_cast<uint8_t*>(self) + 0x1C, state) == -1;
    }

    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, state, 0) == 0)
    {
        void* processor = CSF_AT(void*, state, 4);
        int result =
            ((int(__fastcall*)(void*, void*, void*))
                 CSF_AT(void**, processor, 0)[0x7C / 4])(
                processor, nullptr, state);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, state, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x0044D270, ObjectState_ProcessFields, "auto");
