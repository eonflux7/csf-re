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
// copy-constructs the embedded params block at source+0x1C into self+0x1C.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, WaterParams_CopyConstruct, 0x00709B20,
         (void* self, void* edx, void* source));

static C_0082d410** __stdcall C_0082d410_InitFromSourceAndCopyParams(C_0082d410* self, int source)
{
    C_0082d410** result = nullptr;
    if (self)
    {
        CSF_AT(int, self, 0x18) = 15;
        CSF_AT(std::uint32_t, self, 0x14) = 0;
        CSF_AT(std::uint8_t, self, 0x04) = 0;

        const void* sourceString = reinterpret_cast<const void*>(static_cast<std::uintptr_t>(source));
        AssignStringSubstring(self, nullptr, sourceString, 0u, 0xFFFFFFFFu);

        void* sourceParams = reinterpret_cast<void*>(static_cast<std::uintptr_t>(source) + 0x1Cu);
        void* selfParams = reinterpret_cast<uint8_t*>(self) + 0x1C;
        result = reinterpret_cast<C_0082d410**>(
            WaterParams_CopyConstruct(selfParams, nullptr, sourceParams));
    }
    return result;
}

HOOK(0x00775520, C_0082d410_InitFromSourceAndCopyParams, "auto");
