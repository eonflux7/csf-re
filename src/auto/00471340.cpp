#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: per-table init (ecx = table, no stack args, returns table pointer).
CSF_FUNC(uint8_t*, __fastcall, sub_69B220, 0x0069B220, (void* self, void* edx));

// Callee: per-table configure (ecx = table, 3 stack args, ret 12).
CSF_FUNC(unsigned int, __fastcall, sub_69B250, 0x0069B250, (void* self, void* edx, int a2, unsigned int a3, char a4));

// Five consecutive tables, each 68 bytes apart, starting at this+0.
static char* __fastcall InitMissionScriptTables(void* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    // Init all five tables in order.
    sub_69B220(base, nullptr);
    sub_69B220(base + 68, nullptr);
    sub_69B220(base + 136, nullptr);
    sub_69B220(base + 204, nullptr);
    sub_69B220(base + 272, nullptr);

    // Configure each table: (count, timeout, flag = 0).
    sub_69B250(base, nullptr, 12, 0x2AAu, 0);
    sub_69B250(base + 68, nullptr, 1, 0xBB8u, 0);
    sub_69B250(base + 136, nullptr, 2, 0x3E8u, 0);
    sub_69B250(base + 204, nullptr, 8, 0x2EEu, 0);
    sub_69B250(base + 272, nullptr, 20, 0x190u, 0);

    return reinterpret_cast<char*>(base);
}

HOOK(0x00471340, InitMissionScriptTables, "auto");
