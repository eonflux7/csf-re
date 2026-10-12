#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies one 0x28-byte status icon entry (dst, src).
CSF_FUNC(uint32_t*, __stdcall, HudStatusIcon_CopyEntry, 0x00458760, (uint32_t* a1, uint32_t* a2));

// Rethrows the current C++ exception (_CxxThrowException(0, 0)).
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

// Copies entries from [srcBegin, srcEnd) to dstBegin, 0x28 bytes each.
// On an exception, entries already written get their vtable at +0x1C reset
// to C_0082d410's vtable, then the exception is rethrown.
static uint32_t* __cdecl HudStatusIcons_CopyElementRange(uint32_t* srcBegin, uint32_t* srcEnd, uint32_t* dstBegin, int copyContext, int unusedArg)
{
    (void)copyContext;
    (void)unusedArg;

    uint8_t* src = reinterpret_cast<uint8_t*>(srcBegin);
    uint8_t* dst = reinterpret_cast<uint8_t*>(dstBegin);
    uint8_t* dstFirst = dst;

    try
    {
        while (reinterpret_cast<uint32_t*>(src) != srcEnd)
        {
            HudStatusIcon_CopyEntry(reinterpret_cast<uint32_t*>(dst), reinterpret_cast<uint32_t*>(src));
            dst += 0x28;
            src += 0x28;
        }
    }
    catch (...)
    {
        for (uint8_t* p = dstFirst; p != dst; p += 0x28)
        {
            CSF_AT(void*, p, 0x1C) = CSF_VTBL(0x0082D410);
        }
        RethrowCppException(nullptr, nullptr);
    }

    return reinterpret_cast<uint32_t*>(dst);
}

HOOK(0x004587A0, HudStatusIcons_CopyElementRange, "auto");
