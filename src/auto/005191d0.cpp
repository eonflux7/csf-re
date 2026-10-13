#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// 0x519190 takes 4 stack args (cdecl). The original forwards its three
// caller-supplied arguments and passes the 4th slot, which it reads from an
// unwritten [ebp-4] local, as 0.
CSF_FUNC(void*, __cdecl, CopyAssignRecordRange, 0x00519190, (void* a1, void* a2, void* a3, int a4));

static void* __cdecl CopyAssignRecordPrefix(void* a1, void* a2, void* a3)
{
    return CopyAssignRecordRange(a1, a2, a3, 0);
}

HOOK(0x005191D0, CopyAssignRecordPrefix, "auto");
