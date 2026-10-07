#include <cstdint>
#include "hook.h"

// Copies the dword range [first, last) to dst and returns the pointer one past
// the last element written (std::copy semantics).
static uint32_t* __cdecl CopyDwordRange(uint32_t* first, uint32_t* last, uint32_t* dst)
{
    uint32_t* result = dst;
    while (first != last)
    {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

HOOK(0x0072BB20, CopyDwordRange, "auto");
