#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x00727410: copy-constructs a single 36-byte (0x24) record at `destination` from `source`.
CSF_FUNC(int, __stdcall, CopyConstructRecord_727410, 0x00727410,
         (std::uint32_t* destination, std::uint32_t* source));

// 0x0069B820: releases/destroys a single record during exception unwinding.
CSF_FUNC(int, __stdcall, DestroyRecord_69B820, 0x0069B820,
         (std::uint32_t* record));

// 0x007AF666: __CxxThrowException@8 - rethrows the active exception.
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

// 0x007AF226: ___CxxFrameHandler - referenced by the compiler-generated EH thunk.
CSF_FUNC(int, __cdecl, OriginalCxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH stub and its exception type descriptor (bound by address).
CSF_GLOBAL(std::uint8_t, g_ConstructRangeExceptionHandlerCode, 0x00828030);
CSF_GLOBAL(std::uint8_t, g_ConstructRangeExceptionInfo, 0x00892384);

static std::uint32_t* __cdecl CopyConstructRecordRange(
    std::uint32_t* source,
    std::uint32_t* sourceEnd,
    std::uint32_t* destination,
    void* allocatorContext)
{
    // The bound __stdcall callees do not consume ECX, so the allocator is unused.
    (void)allocatorContext;

    std::uint32_t* const destinationBegin = destination;

    try
    {
        while (source != sourceEnd)
        {
            CopyConstructRecord_727410(destination, source);
            destination += 9;   // records are 36 (0x24) bytes = 9 dwords
            source += 9;
        }
    }
    catch (...)
    {
        std::uint32_t* constructed = destinationBegin;
        std::uint32_t* const constructedEnd = destination;

        while (constructed != constructedEnd)
        {
            DestroyRecord_69B820(constructed);
            constructed += 9;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x00727450, CopyConstructRecordRange, "auto");
