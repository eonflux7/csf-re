#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00749d70_FindObjectIndex, 0x00749D70, (S_00749d70* self, void* , int objectId));
CSF_FUNC(int, __fastcall, S_00749d70_FindFreeSlot, 0x00749E10, (void* self, void* edx));

static unsigned int __fastcall MpSpawnPoints_CheckPlayer(S_00749d70* self, void* /*edx*/, int a2)
{
    int result = S_00749d70_FindObjectIndex(self, nullptr, a2);
    if (result == -1)
    {
        result = S_00749d70_FindFreeSlot(self, nullptr);
        if (result != -1 && result >= 0 && result < 0x10)
        {
            reinterpret_cast<int*>(self)[result + 19] = a2;
        }
    }
    return static_cast<unsigned int>(result);
}

HOOK(0x0074A070, MpSpawnPoints_CheckPlayer, "auto");
