#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, sub_778120, 0x00778120, (void* a1, int a2));
CSF_FUNC(char, __fastcall, sub_778140, 0x00778140, (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_GameDataRangeExceptionMetadata, 0x00894BE8);

static int __cdecl ProcessGameDataRangeWithHandler(
    std::uint32_t first,
    std::uint32_t last,
    std::uint32_t destination,
    std::uint32_t context,
    std::uint32_t unusedArg)
{
    const std::uint32_t destinationBegin = destination;
    std::uint32_t source = first;

    try
    {
        while (source != last)
        {
            sub_778120(reinterpret_cast<void*>(destination),
                       static_cast<int>(source));
            destination += 0x48u;
            source += 0x48u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = destinationBegin;
        while (completed != destination)
        {
            sub_778140(reinterpret_cast<void*>(completed), nullptr);
            completed += 0x48u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(destination);
}

HOOK(0x00778A00, ProcessGameDataRangeWithHandler, "auto");
