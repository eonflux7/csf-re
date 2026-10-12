#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall Component_ProcessSelfAndSubobjects(
    void* self, void* /*edx*/, void* state)
{
    const std::uint32_t savedContext = CSF_AT(std::uint32_t, state, 0x1C);

    bool failed = false;
    if (CSF_AT(std::uint8_t, state, 0x00) == 0)
    {
        void* object = CSF_AT(void*, state, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
                       CSF_AT(void**, object, 0)[0x80 / 4])(
            object, nullptr, 0, state);
    }

    CSF_AT(std::uint8_t, state, 0x00) = failed;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;
    if (!failed)
    {
        void* object = CSF_AT(void*, state, 0x04);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, object, 0)[0x38 / 4])(
                object, nullptr, self, state) == -1;
    }

    const bool processSecond = CSF_AT(std::uint8_t, state, 0x00) == 0;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;
    if (processSecond)
    {
        void* object = CSF_AT(void*, state, 0x04);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, object, 0)[0x54 / 4])(
                object, nullptr, static_cast<std::uint8_t*>(self) + 12, state) == -1;
    }

    const bool processThird = CSF_AT(std::uint8_t, state, 0x00) == 0;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;
    if (processThird)
    {
        void* object = CSF_AT(void*, state, 0x04);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, object, 0)[0x54 / 4])(
                object, nullptr, static_cast<std::uint8_t*>(self) + 16, state) == -1;
    }

    CSF_AT(std::uint32_t, state, 0x1C) = savedContext;
    if (CSF_AT(std::uint8_t, state, 0x00) == 0)
    {
        void* object = CSF_AT(void*, state, 0x04);
        // The result is tested through AL but returned through EAX unchanged.
        const int result =
            ((int(__fastcall*)(void*, void*, void*))
                 CSF_AT(void**, object, 0)[0x7C / 4])(
                object, nullptr, state);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, state, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, state, 0x00) = 1;
    return 1;
}

HOOK(0x00443F80, Component_ProcessSelfAndSubobjects, "auto");
