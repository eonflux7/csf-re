#include <cstdint>
#include "globals.h"
#include "hook.h"

// Fetches the current object slot from the fetch routine (cdecl, no args).
CSF_FUNC(void*, __cdecl, FetchObject, 0x00648BE0, ());

// Processes the fetched object (__thiscall callee, passed as __fastcall).
CSF_FUNC(char, __fastcall, ProcessFetchedObject, 0x00648D60, (void* self, void* edx));

// Stores the two stdcall arguments into two freshly fetched objects, then
// hands the third fetched object to the processing routine.
static int __stdcall StoreArgsIntoFetchedSlotAndProcess(int firstValue, int secondValue)
{
    CSF_AT(int, FetchObject(), 4) = secondValue;
    CSF_AT(int, FetchObject(), 0) = firstValue;

    void* targetObject = FetchObject();
    char processResult = ProcessFetchedObject(targetObject, nullptr);
    return static_cast<int>(processResult);
}

HOOK(0x0073CC40, StoreArgsIntoFetchedSlotAndProcess, "auto");
