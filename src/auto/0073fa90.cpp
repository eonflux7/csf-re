#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));
CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, C_00858180_DispatchSpawnPayload, 0x0073F870,
         (void* self, void* edx, char a2, int arg4, int a4, int a5, int a6, int a7, int a8, int a9,
          int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, char a18,
          int a19, int a3));

// Fetches a spawn record, commits it, and forwards the arguments with the committed
// 64-bit value (plus a19) split into low and high dwords to the dispatch routine.
static bool __fastcall C_00858180_CommitSpawnAndForward(C_00858180* self, void* /*edx*/, char a2,
                                                        int a3, int a4, int a5, int a6, int a7,
                                                        int a8, int a9, int a10, int a11, int a12,
                                                        int a13, int a14, int a15, int a16, int a17,
                                                        char a18, unsigned int a19)
{
    alignas(8) uint8_t spawnRecordBuf[8];
    uint32_t* spawnRecord = MpSpawn_FetchSpawnRecord(reinterpret_cast<uint32_t*>(spawnRecordBuf));
    int64_t committed = MpSpawn_CommitSpawnRecord(spawnRecord, nullptr);

    const uint64_t sum = static_cast<uint64_t>(committed) + a19;
    const int sumLow = static_cast<int>(static_cast<uint32_t>(sum));
    const int sumHigh = static_cast<int>(static_cast<uint32_t>(sum >> 32));

    return C_00858180_DispatchSpawnPayload(self, nullptr, a2, a3, a4, a5, a6, a7, a8, a9,
                                           a10, a11, a12, a13, a14, a15, a16, a17, a18,
                                           sumLow, sumHigh);
}

HOOK(0x0073FA90, C_00858180_CommitSpawnAndForward, "auto");
