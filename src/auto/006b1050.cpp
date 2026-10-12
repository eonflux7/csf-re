#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address with the packet's signatures.
CSF_FUNC(unsigned int, __fastcall, S_006b0320_Reset, 0x006B0B10, (void* self, void* edx));
CSF_FUNC(char, __cdecl, Record_HasValidHeader, 0x006AFE10, (uint32_t* a1));
CSF_FUNC(int, __fastcall, S_006b0320_CopyRecord, 0x006B0320, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, Record_FinalizeState, 0x006AFEF0, (uint32_t* a1));
CSF_FUNC(char, __fastcall, S_006b0320_CommitPending, 0x006B0B90, (int* a1));

// __thiscall(S_006b0320* this, uint32_t* record) -> char, ret 4
static char __fastcall S_006b0320_TryApplyRecord(S_006b0320* self, void* /*edx*/, uint32_t* record)
{
    S_006b0320_Reset(self, nullptr);

    if (CSF_AT(uint32_t, record, 0x8) < 3u || !Record_HasValidHeader(record))
        return 0;

    (void)CSF_VTBL(0x006B0320);
    S_006b0320_CopyRecord(self, nullptr, reinterpret_cast<int>(record));
    Record_FinalizeState(reinterpret_cast<uint32_t*>(self));
    S_006b0320_CommitPending(reinterpret_cast<int*>(self));
    return 1;
}

HOOK(0x006B1050, S_006b0320_TryApplyRecord, "auto");
