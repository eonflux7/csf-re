#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int* __fastcall AdvanceTreeIterator(int* self, void* /*edx*/)
{
    int* result = CSF_AT(int*, self, 0x00);
    if (CSF_AT(std::uint8_t, result, 0x11) != 0)
        return result;

    int* right = CSF_AT(int*, result, 0x08);
    if (CSF_AT(std::uint8_t, right, 0x11) == 0)
    {
        result = CSF_AT(int*, right, 0x00);
        while (CSF_AT(std::uint8_t, result, 0x11) == 0)
        {
            right = result;
            result = CSF_AT(int*, right, 0x00);
        }
        CSF_AT(int*, self, 0x00) = right;
    }
    else
    {
        result = CSF_AT(int*, result, 0x04);
        while (CSF_AT(std::uint8_t, result, 0x11) == 0)
        {
            int* current = CSF_AT(int*, self, 0x00);
            if (current != CSF_AT(int*, result, 0x08))
                break;

            CSF_AT(int*, self, 0x00) = result;
            result = CSF_AT(int*, result, 0x04);
        }
        CSF_AT(int*, self, 0x00) = result;
    }
    return result;
}

HOOK(0x00733C10, AdvanceTreeIterator, "auto");
