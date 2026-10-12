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

// Reads one value out of the message stream into the 8-byte slot passed as the out buffer.
CSF_FUNC(void*, __fastcall, NetStream_ReadIntoSlot, 0x00655FC0, (void* self, void* edx, char* a2));

// Applies six spawn identifiers to the spawn-point manager.
CSF_FUNC(int, __fastcall, MpSpawnPoints_SetSpawnIdentifiers, 0x007483E0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7));

// Reads three 8-byte groups from the stream (each group yields two dwords),
// then forwards the six dwords to MpSpawnPoints_SetSpawnIdentifiers.
static int __fastcall NetGameDDL_ApplySpawnPointIdentifiers(C_00858180* self, void* /*edx*/, C_008622e8* streamArg)
{
    alignas(8) uint8_t firstSlot[8];
    alignas(8) uint8_t secondSlot[8];
    alignas(8) uint8_t thirdSlot[8];

    NetStream_ReadIntoSlot(streamArg, nullptr, reinterpret_cast<char*>(firstSlot));
    NetStream_ReadIntoSlot(streamArg, nullptr, reinterpret_cast<char*>(secondSlot));
    NetStream_ReadIntoSlot(streamArg, nullptr, reinterpret_cast<char*>(thirdSlot));

    const int identifierA = *reinterpret_cast<int*>(firstSlot + 0);
    const int identifierB = *reinterpret_cast<int*>(firstSlot + 4);
    const int identifierC = *reinterpret_cast<int*>(secondSlot + 0);
    const int identifierD = *reinterpret_cast<int*>(secondSlot + 4);
    const int identifierE = *reinterpret_cast<int*>(thirdSlot + 0);
    const int identifierF = *reinterpret_cast<int*>(thirdSlot + 4);

    return MpSpawnPoints_SetSpawnIdentifiers(self, nullptr,
        identifierA, identifierB, identifierC, identifierD, identifierE, identifierF);
}

HOOK(0x0074CD10, NetGameDDL_ApplySpawnPointIdentifiers, "auto");
