#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(std::uint32_t*, __stdcall, ConstructRecordCopy, 0x007236A0,
         (std::uint32_t* destination, std::uint32_t* source));

CSF_FUNC(int, __stdcall, DestroyRecord, 0x00723390, (int* record));

CSF_FUNC(void, __stdcall, RethrowCurrentException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, OriginalCxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH stub: bind its address, not a function entry.
CSF_GLOBAL(std::uint8_t, g_CopyRangeExceptionHandlerCode, 0x00827FA0);
CSF_GLOBAL(std::uint8_t, g_CopyRangeExceptionInfo, 0x00891FFC);

static std::uint32_t* __cdecl CopyConstructRecordRange(
    std::uint32_t* source,
    std::uint32_t* sourceEnd,
    std::uint32_t* destination,
    void* allocatorContext)
{
    // The supplied stdcall callee prototypes do not consume ECX.
    (void)allocatorContext;

    std::uint32_t* const destinationBegin = destination;

    try
    {
        while (source != sourceEnd)
        {
            ConstructRecordCopy(destination, source);
            destination += 15;
            source += 15;
        }
    }
    catch (...)
    {
        std::uint32_t* constructed = destinationBegin;
        std::uint32_t* const constructedEnd = destination;

        while (constructed != constructedEnd)
        {
            DestroyRecord(reinterpret_cast<int*>(constructed));
            constructed += 15;
        }

        RethrowCurrentException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x007236C0, CopyConstructRecordRange, "auto");
