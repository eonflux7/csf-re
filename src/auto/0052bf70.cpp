#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Sub_7A8ED0,
         0x007A8ED0, (int input));

static int __fastcall MenuProgressInput_HandleProgressInput(
    void* self, void* /*edx*/, int input, int /*eventData*/)
{
    int result = Sub_7A8ED0(input);
    if (static_cast<std::uint8_t>(result) != 0)
    {
        void* listener = CSF_AT(void*, self, 0x0C);
        void* owner = static_cast<std::uint8_t*>(self) - 4;
        result = reinterpret_cast<int>(owner);
        if (listener)
        {
            CSF_AT(std::uint32_t, owner, 0x54) = 0;
            return reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, listener, 0)[0x60 / 4])(
                    listener, nullptr, owner, 0);
        }
    }
    return result;
}

HOOK(0x0052BF70, MenuProgressInput_HandleProgressInput, "auto");
