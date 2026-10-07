#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t*, __fastcall, CopyElementWithContext, 0x00730830,
         (void* context, void* edx, std::uint32_t* destination,
          std::uint32_t* source));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, HandleCppExceptionFrame, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// SEH thunk pushed by the function prologue for its C++ try/catch frame.
CSF_GLOBAL(std::uint8_t, g_CopyElementsExceptionHandlerStub, 0x00828080);
CSF_GLOBAL(std::uint8_t, g_CopyElementsExceptionMetadata, 0x0089257C);

static int __cdecl CopyFourByteElementsWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    // When count is zero the original returns whatever EAX happened to hold.
    int result;

    try
    {
        while (count != 0)
        {
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
                CopyElementWithContext(context, nullptr, destination, source)));

            --count;
            destination = reinterpret_cast<std::uint32_t*>(
                reinterpret_cast<std::uintptr_t>(destination) + 4u);
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0072A770, CopyFourByteElementsWithContext, "auto");
