#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall MenuProgressInput_HandleProgressEvent(
    void* self, void* /*edx*/, int event, int /*eventData*/)
{
    if (event == 1)
    {
        void* listener = CSF_AT(void*, self, 0x0C);
        void* owner = static_cast<std::uint8_t*>(self) - 4;
        if (listener)
        {
            CSF_AT(std::uint32_t, owner, 0x54) = 0;
            return reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, listener, 0)[0x60 / 4])(
                    listener, nullptr, owner, 0);
        }
        return reinterpret_cast<int>(owner);
    }

    int result = static_cast<int>(static_cast<std::uint32_t>(event) - 28u);
    if (event == 28)
    {
        void* control = CSF_AT(void*, self, 0x10);
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x104 / 4])(control, nullptr);

        int expectedValue = CSF_AT(int, self, 0x20);
        if (expectedValue && expectedValue == result)
            CSF_AT(std::uint8_t, self, 0x54) = 1;
    }
    return result;
}

HOOK(0x0052BF20, MenuProgressInput_HandleProgressEvent, "auto");
