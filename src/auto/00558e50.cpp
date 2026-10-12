#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-element copy constructor (stdcall, 20-byte records). Sets up the copy of one record
// into the destination slot. Bound as the packet specifies.
CSF_FUNC(int*, __stdcall, MpRules_ConstructRecord_558E20, 0x00558E20, (int a1, uint32_t* a2));

// Record destructor used by the cleanup path (stdcall, one argument).
CSF_FUNC(int, __stdcall, MpRules_ReleaseRecord_558CF0, 0x00558CF0, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_MpRulesRangeFuncInfo, 0x0088C4A0);

// Walks the half-open range [sourceBegin, sourceEnd) of 20-byte records and copy-constructs
// each one into the matching destination slot (same 20-byte stride). If a copy throws,
// the destination slots already constructed are released before rethrowing.
// Returns the past-the-end destination pointer.
static int* __cdecl MpRules_CopyConstructRange(
    int* sourceBegin,
    int* sourceEnd,
    int* destination,
    void* /*contextEcx*/,
    int /*unusedArg5*/)
{
    int* const destinationBegin = destination;

    try
    {
        while (sourceBegin != sourceEnd)
        {
            MpRules_ConstructRecord_558E20(
                static_cast<int>(reinterpret_cast<std::intptr_t>(destination)),
                reinterpret_cast<uint32_t*>(sourceBegin));
            destination = reinterpret_cast<int*>(static_cast<std::uint8_t*>(
                static_cast<void*>(destination)) + 0x14);
            sourceBegin = reinterpret_cast<int*>(static_cast<std::uint8_t*>(
                static_cast<void*>(sourceBegin)) + 0x14);
        }
    }
    catch (...)
    {
        // Release every destination record already constructed in [begin, current).
        int* handled = destinationBegin;

        while (handled != destination)
        {
            MpRules_ReleaseRecord_558CF0(handled);
            handled = reinterpret_cast<int*>(static_cast<std::uint8_t*>(
                static_cast<void*>(handled)) + 0x14);
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x00558E50, MpRules_CopyConstructRange, "auto");
