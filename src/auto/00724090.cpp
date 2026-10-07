#include <cstdint>
#include "globals.h"
#include "hook.h"

// Constructs one 16-byte record at 'destination' from 'source' (stdcall, pops 8).
CSF_FUNC(std::uint32_t*, __stdcall, CallSub723B00IfNonNull, 0x00724020,
         (std::uint32_t* destination, std::uint32_t* source));

// Destroys one already-constructed record (stdcall, pops 4).
CSF_FUNC(int, __stdcall, DestroyConstructedRecord, 0x00724040,
         (int* record));

// _CxxThrowException(0, 0): rethrows the exception currently being handled.
CSF_FUNC(void, __stdcall, RethrowCurrentException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

// __CxxFrameHandler, reached through the compiler-generated EH stub below.
CSF_FUNC(int, __cdecl, OriginalCxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH stub (mov eax,<info>; jmp __CxxFrameHandler) and its
// per-function exception scope table, both referenced by the SEH frame.
CSF_GLOBAL(std::uint8_t, g_CopyRecordRangeExceptionHandlerCode, 0x00827FE0);
CSF_GLOBAL(std::uint8_t, g_CopyRecordRangeExceptionInfo, 0x008921C0);

static std::uint32_t* __cdecl CopyConstructRecordRange(
    std::uint32_t* source,
    std::uint32_t* sourceEnd,
    std::uint32_t* destination,
    void* allocatorContext)
{
    // The supplied stdcall callee prototypes do not consume ECX; the 4th
    // argument only feeds ECX in the original code.
    (void)allocatorContext;

    std::uint32_t* const destinationBegin = destination;

    try
    {
        while (source != sourceEnd)
        {
            CallSub723B00IfNonNull(destination, source);
            destination += 4; // records are 16 bytes (0x10)
            source += 4;
        }
    }
    catch (...)
    {
        std::uint32_t* constructed = destinationBegin;
        std::uint32_t* const constructedEnd = destination;

        while (constructed != constructedEnd)
        {
            DestroyConstructedRecord(reinterpret_cast<int*>(constructed));
            constructed += 4;
        }

        RethrowCurrentException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x00724090, CopyConstructRecordRange, "auto");
