#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00831210_InitWithCode, 0x0044E460, (void* self, void* edx, unsigned int a2));
CSF_FUNC(unsigned int, __fastcall, C_00831210_GetResult, 0x0044E580, (void* self, void* edx));

// If the word at +0x08 is still zero, initialize with code 200 first,
// then return whatever the result getter produces (tail call in the original).
static unsigned int __fastcall C_00831210_InitIfNeededAndGetResult(C_00831210* self, void* /*edx*/)
{
    if (!self->field_0008)
    {
        C_00831210_InitWithCode(self, nullptr, 0xC8u);
    }
    return C_00831210_GetResult(self, nullptr);
}

HOOK(0x44E600, C_00831210_InitIfNeededAndGetResult, "auto");
