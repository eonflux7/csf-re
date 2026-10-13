#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_008310f8_Initialize, 0x00446550, (C_008310f8* self, void* , int owner));
CSF_FUNC(int, __fastcall, C_008310f8_RegisterRecord, 0x004744B0, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_008310f8_AllocAndRegisterRecord(C_008310f8* self, void* /*edx*/)
{
    C_008310f8* record = reinterpret_cast<C_008310f8*>(CoreMemory_AllocUntagged(96));
    if (record)
    {
        uint32_t* newRecord = C_008310f8_Initialize(record, nullptr, -1);
        C_008310f8_RegisterRecord(self, nullptr, reinterpret_cast<int>(newRecord));
        return newRecord;
    }

    C_008310f8_RegisterRecord(self, nullptr, 0);
    return nullptr;
}

HOOK(0x004749B0, C_008310f8_AllocAndRegisterRecord, "auto");
