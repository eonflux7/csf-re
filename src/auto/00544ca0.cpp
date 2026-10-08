#include <cstdint>
#include "globals.h"
#include "hook.h"

// MSVC-generated range "copy-construct N records" step. The original forwards a
// context/allocator argument in ECX (the third incoming argument) before each
// call, but the instantiated callee body does not consume ECX, so both the
// constructor and the cleanup destructor are bound as __stdcall.
CSF_FUNC(int*, __stdcall, CopyConstructRecord, 0x00544C60,
         (void* destination, const void* source));

// MSVC-generated element destructor invoked by the cleanup continuation.
CSF_FUNC(int, __stdcall, DestroyRecord, 0x00469270, (void* record));

CSF_FUNC(void, __stdcall, RethrowCurrentException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, OriginalCxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH scope/catch info referenced by the handler frame:
// bind by address, it is not a plain function entry.
CSF_GLOBAL(std::uint8_t, g_RangeExceptionCatchInfo, 0x0088B630);

// Copies (copy-constructs) the half-open range [source, sourceEnd) into the
// uninitialized storage starting at "destination". Records are 0x24 (36) bytes
// each. If any construction throws, everything already built is destroyed
// before the exception is rethrown. Returns the past-the-end destination
// pointer.
static void* __cdecl CopyConstructRecordRange(
    void* source,
    void* sourceEnd,
    void* destination,
    void* /*allocatorContext*/)
{
    void* const destinationBegin = destination;

    try
    {
        while (source != sourceEnd)
        {
            CopyConstructRecord(destination, source);
            destination = static_cast<std::uint8_t*>(destination) + 0x24;
            source = static_cast<std::uint8_t*>(source) + 0x24;
        }
    }
    catch (...)
    {
        // Destroy everything already constructed in [begin, current).
        void* constructed = destinationBegin;

        while (constructed != destination)
        {
            DestroyRecord(constructed);
            constructed = static_cast<std::uint8_t*>(constructed) + 0x24;
        }

        RethrowCurrentException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x00544CA0, CopyConstructRecordRange, "auto");
