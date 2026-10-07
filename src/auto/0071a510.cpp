#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x0071A2E0: copy-constructs one 8-byte element into `destination` from `source`.
// The construction context (the target's fourth argument) is passed in ECX, so the
// callee is a __thiscall with two stack arguments (ret 8) -> bound as __fastcall.
CSF_FUNC(void, __fastcall, ConstructEightByteElement,
         0x0071A2E0, (void* constructionContext, void* /*edx*/,
                      std::uint32_t* destination, std::uint32_t* source));

// Exception machinery referenced by the SEH frame of the original function.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, HandleCppExceptionFrame,
         0x007AF226, (void* exceptionRecord, void* frame, void* contextRecord, void* dispatcherContext));
CSF_GLOBAL(std::uint8_t, g_EightByteRangeCopyExceptionInformation, 0x00891C74);

// Copy-constructs every 8-byte element of [source, sourceEnd) into destination,
// returning the past-the-end destination pointer. The fourth argument is the
// construction context forwarded in ECX to each constructor call. If a
// construction throws, the exception is re-raised (__CxxThrowException(0, 0)).
static std::uint32_t* __cdecl CopyConstructEightByteElementRange(
    std::uint32_t* source,
    std::uint32_t* sourceEnd,
    std::uint32_t* destination,
    void* constructionContext)
{
    try
    {
        while (source != sourceEnd)
        {
            ConstructEightByteElement(constructionContext, nullptr, destination, source);
            destination += 2; // +8 bytes: one element
            source += 2;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x0071A510, CopyConstructEightByteElementRange, "auto");
