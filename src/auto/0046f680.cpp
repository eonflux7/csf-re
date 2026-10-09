#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original: 0x0046F450, __cdecl, 8 stack args. Bound with the packet's signature.
CSF_FUNC(int*, __cdecl, MissionScript_ParseRecordCore, 0x0046F450,
         (int* a1, int a2, uint32_t* a3, int a4, uint32_t* a5, int a6, uint32_t* a7, int a8));

// Forwards the seven parsed arguments to the core parser, passing the first
// argument a second time as the trailing eighth argument, then returns it.
static int* __cdecl MissionScript_ForwardParseRecord(int* a1, int a2, uint32_t* a3, int a4,
                                                     uint32_t* a5, int a6, uint32_t* a7)
{
    MissionScript_ParseRecordCore(a1, a2, a3, a4, a5, a6, a7, reinterpret_cast<int>(a1));
    return a1;
}

HOOK(0x46F680, MissionScript_ForwardParseRecord, "auto");
