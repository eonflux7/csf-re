#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00730100: returns the dword stored at this+0x144 (field_0144).
static int __fastcall C_00865e38_GetField0144(C_00865e38* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0144);
}

HOOK(0x00730100, C_00865e38_GetField0144, "auto");
