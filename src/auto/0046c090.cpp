#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: releases a block through the mission-script allocator (cdecl, one arg).
CSF_FUNC(int, __cdecl, sub_4714E0, 0x004714E0, (void* Block));

// Clears the owned-block slot at +4 of the parent record, destroying the block first if present.
static int __stdcall MissionScript_ReleaseOwnedBlock(int a1)
{
    void* parentRecord = reinterpret_cast<void*>(a1);
    S_0069b1c0* ownedBlock = CSF_AT(S_0069b1c0*, parentRecord, 4);
    int result = 0;

    if (ownedBlock)
    {
        // Virtual slot 0 (deleting destructor), flag 0 = do not free storage here.
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedBlock, 0)[0])(ownedBlock, nullptr, 0);
        result = sub_4714E0(ownedBlock);
    }

    CSF_AT(S_0069b1c0*, parentRecord, 4) = nullptr;
    return result;
}

HOOK(0x0046C090, MissionScript_ReleaseOwnedBlock, "auto");
