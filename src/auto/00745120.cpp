#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, ReadSerializedByte, 0x00655E20,
               (void* self, void* edx, char* a2));
static CSF_FUNC(char, __fastcall, S_00740ff0_SetByteAndMarkChanged, 0x00740FF0,
               (S_00740ff0* self, void*, char a2));

static char __fastcall S_00740ff0_ReadByteAndMarkChanged(
    S_00740ff0* self, void* /*edx*/, C_008622e8* source)
{
    (void)CSF_VTBL(0x00740FF0);
    ReadSerializedByte(source, nullptr, reinterpret_cast<char*>(&source));
    return S_00740ff0_SetByteAndMarkChanged(
        self, nullptr, *reinterpret_cast<char*>(&source));
}

HOOK(0x00745120, S_00740ff0_ReadByteAndMarkChanged, "auto");
