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

CSF_FUNC(void*, __fastcall, sub_655DF0, 0x00655DF0, (void* self, void* edx, bool* a2));
CSF_FUNC(void*, __fastcall, sub_655E20, 0x00655E20, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(char, __cdecl, sub_799B30, 0x00799B30, (void* a1, int a2));

// Reads one DDL record: a header block, two flag bytes, two bools,
// a 32-byte block, a 512-byte block, and a trailing slot at offset 596.
static uint32_t* __cdecl NetGathering_ReadDdlRecordBlock(C_008622e8* a1, int a2)
{
    char* record = reinterpret_cast<char*>(a2);

    sub_799B30(a1, a2);
    sub_655E20(a1, nullptr, record + 48);
    sub_655E20(a1, nullptr, record + 49);
    sub_655DF0(a1, nullptr, reinterpret_cast<bool*>(record + 50));
    sub_655DF0(a1, nullptr, reinterpret_cast<bool*>(record + 51));

    for (unsigned int i = 0; i < 0x20; ++i)
        sub_655E20(a1, nullptr, record + i + 52);

    for (unsigned int j = 0; j < 0x200; ++j)
        sub_655E20(a1, nullptr, record + j + 84);

    return NetCogibleDdl_ReadIntoSlot(a1, nullptr, a2 + 596);
}

HOOK(0x6AB2E0, NetGathering_ReadDdlRecordBlock, "auto");
