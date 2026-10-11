#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __stdcall, CopyWaypointRecord_75DF90, 0x0075DF90, (char* a1, int a2));
CSF_FUNC(char, __fastcall, DestroyWaypointRecord_75D430, 0x0075D430, (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

static std::uint8_t* __cdecl CopyWaypointRangeWithHandler_75DFB0(
    std::uint8_t* first,
    std::uint8_t* last,
    std::uint8_t* destination,
    void* context,
    int unused)
{
    (void)context;
    (void)unused;

    std::uint8_t* destinationBegin = destination;
    std::uint8_t* source = first;

    try
    {
        while (source != last)
        {
            CopyWaypointRecord_75DF90(reinterpret_cast<char*>(destination),
                                      reinterpret_cast<int>(source));

            destination += 0x48;
            source += 0x48;
        }
    }
    catch (...)
    {
        std::uint8_t* completed = destinationBegin;
        while (completed != destination)
        {
            DestroyWaypointRecord_75D430(completed, nullptr);
            completed += 0x48;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x0075DFB0, CopyWaypointRangeWithHandler_75DFB0, "auto");
