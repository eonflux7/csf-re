#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, WeaponFirearms_InitializeEventRecord, 0x004747B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00831658_RegisterEventRecord, 0x004745D0, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_00831658_AllocEventRecord(C_00831658* self, void* /*edx*/)
{
    C_00831658* record = reinterpret_cast<C_00831658*>(CoreMemory_AllocUntagged(96));
    if (record)
    {
        uint32_t* eventRecord = WeaponFirearms_InitializeEventRecord(record, nullptr, -1);
        C_00831658_RegisterEventRecord(self, nullptr, reinterpret_cast<int>(eventRecord));
        return eventRecord;
    }

    C_00831658_RegisterEventRecord(self, nullptr, 0);
    return nullptr;
}

HOOK(0x00474A70, C_00831658_AllocEventRecord, "auto");
