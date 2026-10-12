#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, BddTables_ProcessEntry, 0x00519F80, (void* self, void* edx, int a2));

static int* __stdcall BddTables_ProcessEntryIfNonNull(S_00519c10* entry, int argument)
{
    if (entry)
        return BddTables_ProcessEntry(entry, nullptr, argument);
    return nullptr;
}

HOOK(0x0051A200, BddTables_ProcessEntryIfNonNull, "auto");
