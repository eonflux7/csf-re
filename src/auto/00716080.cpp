#include <cstdint>
#include "globals.h"
#include "hook.h"

// MSVC-generated range "construct one record" step. The original forwards the
// context/allocator argument in ECX (__thiscall-style call), but the
// instantiated callee body does not consume ECX, so it is bound as __stdcall.
CSF_FUNC(int*, __stdcall, CallHandlerIfNonNull_716060, 0x00716060,
         (int* destination, int* source));

// MSVC-generated element destructor invoked by the cleanup continuation.
CSF_FUNC(int, __stdcall, DestroyElementThroughContext, 0x006B5220, (int* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH code/info referenced by the scope table: bind by
// address, they are not plain function entries.
CSF_GLOBAL(std::uint8_t, g_RangeExceptionHandlerCode, 0x00827EB0);
CSF_GLOBAL(std::uint8_t, g_RangeExceptionCatchInfo, 0x00891A2C);

static int* __cdecl CopyConstructRecordRange(
    int* source,
    int* sourceEnd,
    int* destination,
    void* allocatorContext)
{
    // The original keeps this in EBX and reloads it into ECX for both callees;
    // neither bound callee consumes ECX.
    (void)allocatorContext;

    int* const destinationBegin = destination;

    try
    {
        // Records are 16 bytes each; the pointer advances 4 dwords.
        while (source != sourceEnd)
        {
            CallHandlerIfNonNull_716060(destination, source);
            destination += 4;
            source += 4;
        }
    }
    catch (...)
    {
        // Destroy everything already constructed in [begin, current).
        int* constructed = destinationBegin;
        int* const constructedEnd = destination;

        while (constructed != constructedEnd)
        {
            DestroyElementThroughContext(constructed);
            constructed += 4;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x00716080, CopyConstructRecordRange, "auto");
