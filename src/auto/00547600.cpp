#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Constructs one element in the destination array from 'source', using
// 'context' as the allocator/callback object.  The original is a __thiscall
// (ecx = context, stack = element, source); declared as __fastcall so the
// register arguments line up.
CSF_FUNC(std::uint32_t*, __fastcall, ConstructStringElement, 0x00547090,
         (void* context, void* edx, std::uint32_t* element, std::uint32_t* source));

// Destroys one already-constructed element (__thiscall, ecx = context).
CSF_FUNC(int, __fastcall, DestroyStringElement, 0x005466C0,
         (void* context, void* edx, std::uint32_t* element));

// Rethrows the in-flight exception (MSVC _CxxThrowException).
CSF_FUNC(void, __stdcall, RethrowGameException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// MSVC C++ frame handler referenced by the exception-handler thunk.
CSF_FUNC(int, __cdecl, HandleGameCppException, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception-type data loaded by the exception-handler thunk before jumping
// to _CxxFrameHandler.
CSF_GLOBAL(std::uint8_t, g_ArrayConstructionExceptionInfo, 0x0088B968);

// Constructs 'count' elements of stride 0x20 at 'destination' by copying from
// 'source' through 'context'.  If any construction throws, the elements already
// built are destroyed and the exception is rethrown.
static std::uint32_t* __cdecl MenuExtras_ConstructStringElementArray(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    std::uint32_t* currentElement = destination;
    std::uint32_t* result;

    try
    {
        while (count != 0)
        {
            result = ConstructStringElement(context, nullptr, currentElement, source);
            --count;
            currentElement += 8; // element size == 0x20 bytes
        }
    }
    catch (...)
    {
        std::uint32_t* element = destination;
        while (element != currentElement)
        {
            DestroyStringElement(context, nullptr, element);
            element += 8;
        }

        RethrowGameException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00547600, MenuExtras_ConstructStringElementArray, "auto");
