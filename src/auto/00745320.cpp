#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, ReadSerializedByte, 0x00655E20,
               (void* self, void* edx, char* a2));
static CSF_FUNC(char, __fastcall, CMenuMpStats_IncrementStatCounter, 0x00740D50,
               (CMenuMpStats* self, void*, char selection));

static char __fastcall CMenuMpStats_ReadByteAndIncrementStatCounter(
    CMenuMpStats* self, void* /*edx*/, C_008622e8* source)
{
    ReadSerializedByte(source, nullptr, reinterpret_cast<char*>(&source));
    return CMenuMpStats_IncrementStatCounter(
        self, nullptr, *reinterpret_cast<char*>(&source));
}

HOOK(0x00745320, CMenuMpStats_ReadByteAndIncrementStatCounter, "auto");
