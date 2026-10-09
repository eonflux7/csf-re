#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies one 0x20-byte mission-script entry into a destination slot. The original
// loads ecx = a4 before the call and pushes (source, destination), and the callee
// returns with ret 8, so it reads `this` from ecx: bound as a fastcall-style method.
CSF_FUNC(std::uint32_t, __fastcall, MissionScript_AssignEntryToSlot_697AD0, 0x00697AD0,
         (void* self, void* edx, C_0082d410* source, C_0082d410* destination));

// Element destructor used by the cleanup path (stride 0x20 records).
CSF_FUNC(int, __stdcall, DestroyRecord_469270, 0x00469270, (void* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH thunk and FuncInfo referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_MissionScriptRangeExceptionHandlerCode, 0x00826910);
CSF_GLOBAL(std::uint8_t, g_MissionScriptRangeFuncInfo, 0x00888F2C);

// Walks the half-open range [sourceBegin, sourceEnd) of 0x20-byte entries and copies
// each one into the matching destination slot (also 0x20-byte stride) through the
// entry-assign helper, using contextEcx as its `this`. If a copy throws, the slots
// already written are destroyed before rethrowing. Returns the past-the-end
// destination pointer.
static int __cdecl MissionScript_CopyEntryRange(
    void* sourceBegin,
    void* sourceEnd,
    void* destinationBegin,
    void* contextEcx,
    int /*unusedArg5*/)
{
    C_0082d410* source = reinterpret_cast<C_0082d410*>(sourceBegin);
    C_0082d410* destination = reinterpret_cast<C_0082d410*>(destinationBegin);

    try
    {
        while (source != reinterpret_cast<C_0082d410*>(sourceEnd))
        {
            MissionScript_AssignEntryToSlot_697AD0(contextEcx, nullptr, source, destination);
            destination = reinterpret_cast<C_0082d410*>(
                static_cast<std::uint8_t*>(static_cast<void*>(destination)) + 0x20);
            source = reinterpret_cast<C_0082d410*>(
                static_cast<std::uint8_t*>(static_cast<void*>(source)) + 0x20);
        }
    }
    catch (...)
    {
        // Destroy every destination slot already written in [begin, current).
        C_0082d410* handled = reinterpret_cast<C_0082d410*>(destinationBegin);

        while (handled != destination)
        {
            DestroyRecord_469270(handled);
            handled = reinterpret_cast<C_0082d410*>(
                static_cast<std::uint8_t*>(static_cast<void*>(handled)) + 0x20);
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x0046C820, MissionScript_CopyEntryRange, "auto");
