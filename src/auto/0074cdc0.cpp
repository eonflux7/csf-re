#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, ReadSerializedByte, 0x00655E20,
               (void* self, void* edx, char* a2));
static CSF_FUNC(char, __fastcall, CMenuMpStats_ApplyByteSelection, 0x00747F60,
               (void* self, void* edx, char a2));

static char __fastcall CMenuMpStats_ReadByteAndApplySelection(
    CMenuMpStats* self, void* /*edx*/, C_008622e8* source)
{
    ReadSerializedByte(source, nullptr, reinterpret_cast<char*>(&source));
    return CMenuMpStats_ApplyByteSelection(
        self, nullptr, *reinterpret_cast<char*>(&source));
}

HOOK(0x0074CDC0, CMenuMpStats_ReadByteAndApplySelection, "auto");
