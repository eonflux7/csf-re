// mission_script: reserves table space for the parsed entries and returns the current entry count.
#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bindings (names chosen here; signatures as given in the packet).
CSF_FUNC(unsigned int, __fastcall, MissionScript_ReallocPrimaryTable, 0x004703B0,
         (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, MissionScript_ReallocSecondaryTable, 0x00470F50,
         (void* self, void* edx, unsigned int a2, char a3));

// __thiscall(this, a2) -> int, ret 4.
// Reads the table object at this+0x6c, computes its element count from the begin/end
// pointers (begin at +4, end at +8, 4-byte elements), reserves count + a2 entries in the
// primary table and in the secondary table at +16, and returns the count.
static int __fastcall MissionScript_ExtendTableAndReturnCount(void* self, void* /*edx*/, int a2)
{
    void* table = CSF_AT(void*, self, 0x6C);

    int count = 0;
    if (CSF_AT(int, table, 4) != 0)
    {
        int byteSpan = CSF_AT(int, table, 8) - CSF_AT(int, table, 4);
        count = byteSpan >> 2;
    }

    int reserveCount = count + a2;

    MissionScript_ReallocPrimaryTable(table, nullptr, static_cast<unsigned int>(reserveCount), 0);

    void* secondaryTable = static_cast<void*>(static_cast<uint8_t*>(table) + 16);
    MissionScript_ReallocSecondaryTable(secondaryTable, nullptr, static_cast<unsigned int>(reserveCount), 1);

    return count;
}

HOOK(0x00471060, MissionScript_ExtendTableAndReturnCount, "auto");
