#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copy-constructs one 0x1C-byte record into the destination slot from the source entry.
CSF_FUNC(int*, __stdcall, CopyConstructRecord_410B50, 0x00410B50,
         (void* destination, const void* source));

// Destroys one 0x1C-byte record in the cleanup path.
CSF_FUNC(int, __stdcall, DestroyRecord_469270, 0x00469270, (void* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_CopyRecordRangeFuncInfo, 0x00888F7C);

// Copies the half-open range [source, sourceEnd) of 0x1C-byte records into the
// destination array, copy-constructing each element in order. If a copy throws,
// the destination slots already constructed are destroyed before rethrowing.
// Returns the past-the-end destination pointer.
static int __cdecl CopyConstructRecordRange(
    void* source,
    void* sourceEnd,
    void* destination,
    void* /*contextEcx*/,
    int /*unusedArg5*/)
{
    void* const destinationBegin = destination;

    try
    {
        while (source != sourceEnd)
        {
            CopyConstructRecord_410B50(destination, source);
            destination = static_cast<std::uint8_t*>(destination) + 0x1C;
            source = static_cast<std::uint8_t*>(source) + 0x1C;
        }
    }
    catch (...)
    {
        // Destroy every destination slot already constructed in [begin, current).
        void* constructed = destinationBegin;

        while (constructed != destination)
        {
            DestroyRecord_469270(constructed);
            constructed = static_cast<std::uint8_t*>(constructed) + 0x1C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x0046C8B0, CopyConstructRecordRange, "auto");
