#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets the object's state field (+0x58) to 7 unless it already is 7, but only
// when the owner's virtual query at slot 35 (+0x8C) does not return -1.
static void __fastcall RefreshStateToSevenIfQuerySucceeds(void* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0x58) == 7)
        return;

    void* owner = CSF_AT(void*, self, 0x14);
    using OwnerQueryFn = int(__fastcall*)(void*, void*, int, int, int, int);
    OwnerQueryFn ownerQuery = reinterpret_cast<OwnerQueryFn>(CSF_AT(void**, owner, 0)[0x8C / 4]);
    int queryResult = ownerQuery(owner, nullptr, 13, 0, -1, 0);

    if (queryResult != -1)
        CSF_AT(int, self, 0x58) = 7;
}

HOOK(0x4D2020, RefreshStateToSevenIfQuerySucceeds, "auto");
