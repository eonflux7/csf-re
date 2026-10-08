#include <cstdint>
#include "globals.h"
#include "hook.h"

// MSVC-generated range "copy-construct N records" step. The original forwards a
// context/allocator argument in ECX before each call, but the instantiated
// callee bodies do not consume ECX, so both are bound as __stdcall.
CSF_FUNC(int*, __stdcall, CopyConstructRecord, 0x00410B50,
         (void* destination, const void* source));

// MSVC-generated element destructor invoked by the cleanup continuation.
CSF_FUNC(int, __stdcall, DestroyRecord, 0x00469270, (void* record));

CSF_FUNC(void, __stdcall, RethrowCurrentException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, OriginalCxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH code/info referenced by the scope table: bind by
// address, they are not plain function entries.
CSF_GLOBAL(std::uint8_t, g_RangeExceptionHandlerCode, 0x00826D10);
CSF_GLOBAL(std::uint8_t, g_RangeExceptionCatchInfo, 0x0088AA74);

// Copies (copy-constructs) the half-open range [source, sourceEnd) into the
// uninitialized storage starting at "destination". Records are 0x1C bytes each.
// If any construction throws, everything already built is destroyed before the
// exception is rethrown. Returns the past-the-end destination pointer.
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
            destination = static_cast<std::uint8_t*>(destination) + 0x1C;
            source = static_cast<std::uint8_t*>(source) + 0x1C;
        }
    }
    catch (...)
    {
        // Destroy everything already constructed in [begin, current).
        void* constructed = destinationBegin;

        while (constructed != destination)
        {
            DestroyRecord(constructed);
            constructed = static_cast<std::uint8_t*>(constructed) + 0x1C;
        }

        RethrowCurrentException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x0050C120, CopyConstructRecordRange, "auto");

