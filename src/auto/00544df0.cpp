#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Constructs one 36-byte array entry inside `context` from the supplied source.
CSF_FUNC(std::uint32_t*, __fastcall, ConstructElement, 0x00544C60,
         (void* context, void* edx, std::uint32_t* element, std::uint32_t* source));

// Destroys one already-constructed 36-byte array entry inside `context`
// (used while unwinding a partially built array).
CSF_FUNC(void, __fastcall, DestroyElement, 0x00469270,
         (void* context, void* edx, std::uint32_t* element));

CSF_FUNC(void, __stdcall, RethrowGameException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, HandleGameCppException, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_ElementArrayExceptionInfo, 0x0088B680);

// Builds `count` consecutive 36-byte entries starting at `destination`, each one
// initialized from `source` through `context`. If any construction throws, the
// entries constructed so far are destroyed again and the exception is rethrown.
static std::uint32_t* __cdecl MenuSaveGame_ConstructElementArray(
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
            result = ConstructElement(context, nullptr, currentElement, source);
            --count;
            currentElement += 9; // 36 bytes per entry
        }
    }
    catch (...)
    {
        std::uint32_t* element = destination;
        while (element != currentElement)
        {
            DestroyElement(context, nullptr, element);
            element += 9; // 36 bytes per entry
        }

        RethrowGameException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00544DF0, MenuSaveGame_ConstructElementArray, "auto");
