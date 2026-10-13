#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Binds to the original S_0050f230 insert-unique-key routine (__thiscall, ret 8).
CSF_FUNC(int, __fastcall, S_0050f230_InsertUniqueKey, 0x00519EC0,
         (S_0050f230* self, void* /*edx*/, int resultAddress, uint32_t* value));

// Binds to the original SceneLoader sector record getter (__thiscall, ret 4).
CSF_FUNC(int, __fastcall, SceneLoader_GetSectorRecord, 0x00762470,
         (void* self, void* /*edx*/, int sectorIndex));

// Element stride of the sector table referenced via this->field_0008 (0x22C bytes).
static constexpr uint32_t kSectorRecordStride = 0x22C;

// Computes the address of sector record sectorIndex in the table at this+0x8,
// inserts it under a unique key via the sub-object at this+0xC, then returns
// the sector record for the same index.
static int __fastcall C_008406e8_ActivateSector(C_008406e8* self, void* /*edx*/, int sectorIndex)
{
    // Stack slot the callee writes its result into (original frame: [ebp-8], 8 bytes).
    alignas(8) uint8_t resultBytes[8];

    // Table descriptor stored at this+0x8; its +4 dword is the record array base.
    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);
    uint32_t tableDescriptorAddr = CSF_AT(uint32_t, selfBytes, 0x8);
    uint8_t* tableDescriptor = reinterpret_cast<uint8_t*>(tableDescriptorAddr);
    uint32_t recordArrayBase = CSF_AT(uint32_t, tableDescriptor, 4);
    uint32_t* record = reinterpret_cast<uint32_t*>(
        recordArrayBase + static_cast<uint32_t>(sectorIndex) * kSectorRecordStride);

    // Original passes this + 0xC (lea ecx, [esi+0xc]) as the S_0050f230 sub-object.
    S_0050f230* keyTable = reinterpret_cast<S_0050f230*>(selfBytes + 0xC);
    S_0050f230_InsertUniqueKey(keyTable, nullptr,
                               static_cast<int>(reinterpret_cast<uintptr_t>(resultBytes)),
                               record);

    return SceneLoader_GetSectorRecord(self, nullptr, sectorIndex);
}

HOOK(0x0051B9E0, C_008406e8_ActivateSector, "auto");
