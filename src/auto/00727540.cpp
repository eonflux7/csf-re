#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall(context, element, source): ecx = context, 2 stack args (ret 8).
CSF_FUNC(int, __fastcall, ConstructElementFromSource, 0x00727410,
         (void* context, void* edx, std::uint8_t* element, void* source));
// __thiscall(context, element): ecx = context, 1 stack arg (ret 4).
CSF_FUNC(void, __fastcall, DestroyElementThroughContext, 0x0069B820,
         (void* context, void* edx, std::uint8_t* element));
CSF_FUNC(void, __stdcall, RethrowGameException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Exception info structure installed by the SEH funclet at 0x828040.
CSF_GLOBAL(std::uint8_t, g_ElementArrayConstructionExceptionInfo, 0x008923D4);

// Copy-constructs `count` 36-byte elements into `destination`, pulling each
// element from `source` through `context`. On failure the elements already
// constructed are destroyed in reverse of construction order... actually
// forward from `destination` up to the current element, then the exception
// is rethrown.
static int __cdecl CopyConstructElementArray36(
    std::uint8_t* destination,
    unsigned int count,
    void* source,
    void* context)
{
    std::uint8_t* currentElement = destination;
    int result;

    try
    {
        while (count != 0)
        {
            result = ConstructElementFromSource(context, nullptr, currentElement, source);
            --count;
            currentElement += 36;
        }
    }
    catch (...)
    {
        std::uint8_t* element = destination;
        while (element != currentElement)
        {
            DestroyElementThroughContext(context, nullptr, element);
            element += 36;
        }

        RethrowGameException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00727540, CopyConstructElementArray36, "auto");
