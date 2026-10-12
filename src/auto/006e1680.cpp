#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Processes one 60-byte (0x3c) particle emitter entry with a forwarded argument.
CSF_FUNC(int, __stdcall, ParticleEmitters_ProcessEntry, 0x006E1540,
         (void* a1, void* a2));

// Destroys one already-constructed 60-byte entry.
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270,
         (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 60-byte (0x3c) emitter entry.
static C_0082d410* ParticleEmitters_NextEntry(C_0082d410* entry)
{
    return reinterpret_cast<C_0082d410*>(reinterpret_cast<std::uint8_t*>(entry) + 0x3C);
}

// Runs ParticleEmitters_ProcessEntry on each of `count` consecutive emitter entries
// starting at `first`. If a call throws, the entries already processed are destroyed
// again and the exception is rethrown.
static int __cdecl ParticleEmitters_ApplyEntryRange(
    C_0082d410* first,
    std::uint32_t count,
    C_0082d410* forwardArg,
    void* /*context*/)
{
    C_0082d410* currentEntry = first;
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = ParticleEmitters_ProcessEntry(
                static_cast<void*>(currentEntry),
                static_cast<void*>(forwardArg));
            --count;
            currentEntry = ParticleEmitters_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        C_0082d410* entry = first;
        while (entry != currentEntry)
        {
            MissionScript_ReleaseRecordBuffer(reinterpret_cast<int*>(entry));
            entry = ParticleEmitters_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x006E1680, ParticleEmitters_ApplyEntryRange, "auto");
