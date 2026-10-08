#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint8_t, __cdecl, IsConfirmationInput, 0x007A8ED0, (int input));

static int __fastcall MenuMessageBox_HandleInput(
    void* self, void* /*edx*/, int input, int /*unused*/)
{
    if (IsConfirmationInput(input))
    {
        if (CSF_AT(uint32_t, self, 0x78) & 0x800u)
            CSF_AT(uint8_t, self, 0x81) = 1;
    }
    else if (input == 5)
    {
        if (CSF_AT(uint32_t, self, 0x78) & 0x1000u)
            CSF_AT(uint8_t, self, 0x80) = 1;
    }
    else if (input == 12)
    {
        if (CSF_AT(uint32_t, self, 0x78) & 0x2000u)
            CSF_AT(uint8_t, self, 0x7F) = 1;
    }

    int result = CSF_AT(int32_t, self, 0x78);
    if (result)
    {
        CSF_AT(int32_t, self, 0x994) = 0;
    }
    else
    {
        result = CSF_AT(int32_t, self, 0x994);
        CSF_AT(int32_t, self, 0x994) = result;
    }
    return result;
}

HOOK(0x00528DF0, MenuMessageBox_HandleInput, "auto");
