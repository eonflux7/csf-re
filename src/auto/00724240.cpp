#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Constructs one element (from source) through the given context, skipping
// null elements. Wrapper for the element-size-specific copy routine.
CSF_FUNC(std::uint32_t*, __fastcall, CallSub723B00IfNonNull, 0x00724020,
         (void* context, void* edx, std::uint32_t* element, int source));

// Destroys a single constructed element through the given context.
CSF_FUNC(int, __fastcall, ResetWideStringRecordStorage, 0x00724040,
         (void* context, void* edx, std::uint32_t* element));

// Rethrows the currently handled C++ exception.
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// MSVC C++ frame handler referenced by the compiler-generated EH stub.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_ArrayConstructionExceptionInfo, 0x00892210);

// Copy-constructs `count` elements (16 bytes each) into the destination array,
// each produced via CallSub723B00IfNonNull with the given context. If a
// constructor throws, every already-constructed element is destroyed and the
// exception is rethrown.
static std::uint32_t* __cdecl ConstructElementArray(
    std::uint32_t* destination,
    std::uint32_t count,
    int source,
    void* context)
{
    std::uint32_t* currentElement = destination;
    std::uint32_t* result;

    try
    {
        while (count != 0)
        {
            result = CallSub723B00IfNonNull(
                context, nullptr, currentElement, source);
            --count;
            currentElement += 4;
        }
    }
    catch (...)
    {
        std::uint32_t* element = destination;
        while (element != currentElement)
        {
            ResetWideStringRecordStorage(context, nullptr, element);
            element += 4;
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00724240, ConstructElementArray, "auto");
