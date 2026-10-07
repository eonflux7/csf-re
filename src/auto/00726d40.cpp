#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copy-constructs one element of a pointer range: source and destination are
// passed on the stack, the construction context travels in ECX (ret 8).
CSF_FUNC(std::uint32_t*, __fastcall, CopyConstructPointerElement,
         0x00730830,
         (void* constructionContext, void* /*edx*/,
          std::uint32_t* destination, std::uint32_t* source));

// MSVC 7.1 C++ exception helpers used by the generated SEH frame.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, HandleCppExceptionFrame, 0x007AF226, (...));
CSF_GLOBAL(std::uint8_t, g_PointerRangeCopyExceptionInformation, 0x008922E4);

// Copies the pointer range [source, sourceEnd) into the destination range,
// copy-constructing each element with the caller-supplied construction
// context (passed to the element copier in ECX). Returns the end of the
// destination range.
static int __cdecl CopyConstructPointerRange(
    std::uint32_t source,
    std::uint32_t sourceEnd,
    std::uint32_t destination,
    void* constructionContext)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyConstructPointerElement(
                constructionContext,
                nullptr,
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<std::uint32_t*>(source));
            destination += 4u;
            source += 4u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x00726D40, CopyConstructPointerRange, "auto");
