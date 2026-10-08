#include <cstdint>
#include "globals.h"
#include "hook.h"

// MSVC-generated "copy-construct one 32-byte record" step of the range copy.
// The original forwards a context in ECX (__thiscall-style call), but the
// instantiated callee body does not consume ECX, so it is bound as __stdcall.
// First stack arg = destination (pushed last), second = source.
CSF_FUNC(int, __stdcall, MenuExtras_InitStringFromPair, 0x00547090,
         (int* destination, int* source));

// MSVC-generated element destructor invoked by the cleanup continuation.
CSF_FUNC(int, __stdcall, ResetStringStorage_5466C0, 0x005466C0, (int* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH data referenced by the scope table: bind by address,
// it is not a plain function entry.
CSF_GLOBAL(std::uint8_t, g_RangeExceptionCatchInfo, 0x0088B844);

static int* __cdecl ConstructRecordRange(
    int* source,
    int* sourceEnd,
    int* destination,
    void* allocatorContext)
{
    // Forwarded in ECX in the original; neither bound callee consumes ECX.
    (void)allocatorContext;

    int* const destinationBegin = destination;

    try
    {
        // Records are 32 bytes each; the pointer advances 8 dwords.
        while (source != sourceEnd)
        {
            MenuExtras_InitStringFromPair(destination, source);
            destination += 8;
            source += 8;
        }
    }
    catch (...)
    {
        // Destroy everything already constructed in [begin, current).
        int* constructed = destinationBegin;
        int* const constructedEnd = destination;

        while (constructed != constructedEnd)
        {
            ResetStringStorage_5466C0(constructed);
            constructed += 8;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x005470D0, ConstructRecordRange, "auto");
