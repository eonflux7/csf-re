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

CSF_FUNC(uint32_t*, __fastcall, NetZ_ReadIntoOutParam, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, CMenuMpStats_SubmitStatsRecord, 0x00740790, (void* self, void* edx, int a2, int a3, uint8_t* a4));

// Reads three values out of the argument object through NetZ_ReadIntoOutParam,
// the first two into locals and the third back into the argument slot itself,
// then forwards them to CMenuMpStats_SubmitStatsRecord.
static char __fastcall CMenuMpStats_SubmitStatsFromArgument(CMenuMpStats* self, void* /*edx*/, C_008622e8* arg0)
{
    C_008622e8* argument = arg0;
    int a2 = 0;
    int a3 = 0;

    NetZ_ReadIntoOutParam(argument, nullptr, (int)(intptr_t)&a2);
    NetZ_ReadIntoOutParam(argument, nullptr, (int)(intptr_t)&a3);
    NetZ_ReadIntoOutParam(argument, nullptr, (int)(intptr_t)&arg0);

    return CMenuMpStats_SubmitStatsRecord(self, nullptr, a2, a3, reinterpret_cast<uint8_t*>(arg0));
}

HOOK(0x007426D0, CMenuMpStats_SubmitStatsFromArgument, "auto");
