#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082e6a0_InitStateByCode, 0x0041EB30, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0082e6a0_GetStateResult, 0x0041EC30, (void* self, void* edx));

// If the state word at +0x08 is still zero, initialize it with code 10 first,
// then return whatever the result getter produces (tail call in the original).
static uint32_t* __fastcall C_0082e6a0_InitIfNeededAndGet(C_0082e6a0* self, void* /*edx*/)
{
    if (!self->field_0008)
    {
        C_0082e6a0_InitStateByCode(self, nullptr, 0xAu);
    }
    return C_0082e6a0_GetStateResult(self, nullptr);
}

HOOK(0x41ECC0, C_0082e6a0_InitIfNeededAndGet, "auto");
