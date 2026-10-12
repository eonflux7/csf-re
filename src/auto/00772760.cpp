#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies the source string at [source+0] into self (count -1 = whole string) and
// forwards the embedded params block at source+0x1C to the block copier at 0x772460.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, ParamsBlock_CopyFromSource, 0x00772460,
         (void* self, void* edx, int a2));

static int __stdcall C_0082d410_InitNameAndCopyParamsBlock(C_0082d410* self, C_0082d410* source)
{
    int result = 0;
    if (self)
    {
        CSF_AT(int, self, 0x18) = 15;
        CSF_AT(std::uint32_t, self, 0x14) = 0;
        CSF_AT(std::uint8_t, self, 0x04) = 0;

        AssignStringSubstring(self, nullptr, source, 0u, 0xFFFFFFFFu);

        void* selfParams = reinterpret_cast<uint8_t*>(self) + 0x1C;
        int sourceParams = static_cast<int>(reinterpret_cast<std::uintptr_t>(source) + 0x1C);
        result = ParamsBlock_CopyFromSource(selfParams, nullptr, sourceParams);
    }
    return result;
}

HOOK(0x00772760, C_0082d410_InitNameAndCopyParamsBlock, "auto");
