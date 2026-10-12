#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Processes one 16-byte BDD table entry if its slot is non-null.
CSF_FUNC(int*, __stdcall, BddTables_ProcessEntryIfNonNull, 0x0051A200,
         (S_00519c10* entry, int argument));

// Destroys one already-constructed 16-byte BDD table entry.
CSF_FUNC(int, __stdcall, DestroyElementThroughContext, 0x006B5220,
         (int* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 16-byte (0x10) BDD table entry.
static S_00519c10* BddTables_NextEntry(S_00519c10* entry)
{
    return reinterpret_cast<S_00519c10*>(reinterpret_cast<std::uint8_t*>(entry) + 0x10);
}

// Runs BddTables_ProcessEntryIfNonNull over `count` consecutive entries starting at
// `first`. If a call throws, the entries already processed are destroyed again
// and the exception is rethrown.
static int* __cdecl BddTables_ProcessEntries(
    S_00519c10* first,
    std::uint32_t count,
    int argument,
    void* /*context*/)
{
    S_00519c10* currentEntry = first;
    int* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = BddTables_ProcessEntryIfNonNull(currentEntry, argument);
            --count;
            currentEntry = BddTables_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        S_00519c10* entry = first;
        while (entry != currentEntry)
        {
            DestroyElementThroughContext(reinterpret_cast<int*>(entry));
            entry = BddTables_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0051A300, BddTables_ProcessEntries, "auto");
