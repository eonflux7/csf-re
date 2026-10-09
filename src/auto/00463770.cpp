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

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, QueueTaskRecord, 0x004758B0, (int self, void* edx, uint32_t* taskRecord, char flag));

// Builds a 0x34-byte task record from a flagged node chain and submits it through the
// global object, when bit 27 of the node's flags is set. Otherwise returns the node pointer.
static int __stdcall SubmitFlaggedNodeCommand(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    uint32_t nodeA = a3[0];                                   // v3 = *a3
    uint32_t nodeB = CSF_AT(uint32_t, reinterpret_cast<uint32_t*>(nodeA), 4);
    uint32_t result = nodeB;                                  // result = *(v3 + 4)

    uint32_t flagsB = CSF_AT(uint32_t, reinterpret_cast<uint32_t*>(nodeB), 4);
    if ((flagsB & 0x8000000) != 0)
    {
        uint32_t rec[13];
        rec[0] = (uint32_t)(uintptr_t)CSF_VTBL(0x0082D478);
        rec[1] = 1;
        rec[2] = (uint32_t)(uintptr_t)CSF_VTBL(0x0082D448);
        rec[3] = 0xFFFFFFFFu;
        rec[4] = 0xFFFFFFFFu;
        rec[5] = (uint32_t)(uintptr_t)CSF_VTBL(0x0082D448);
        rec[6] = 0xFFFFFFFFu;
        rec[7] = 0xFFFFFFFFu;
        rec[8] = 0;
        rec[9] = 0;
        rec[10] = 0;
        rec[11] = 0;
        reinterpret_cast<uint8_t*>(&rec[12])[0] = 0;

        uint32_t flagsMasked = flagsB & 0xF7FFFFFFu;          // v5 with bit 27 cleared
        uint32_t nodeD = a3[1];                               // v6 = a3[1]
        uint32_t nodeE = CSF_AT(uint32_t, reinterpret_cast<uint32_t*>(nodeD), 4);
        rec[9] = flagsMasked;                                 // v15

        rec[3] = CSF_AT(uint32_t, reinterpret_cast<uint32_t*>(nodeE), 8);   // v9
        rec[4] = CSF_AT(uint32_t, reinterpret_cast<uint32_t*>(nodeE), 12);  // v10

        int globalObject = GetGlobalObjectField184();
        return static_cast<int>(QueueTaskRecord(globalObject, nullptr, rec, 0));
    }
    return static_cast<int>(result);
}

HOOK(0x00463770, SubmitFlaggedNodeCommand, "auto");
