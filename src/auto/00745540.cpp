#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008622e8_WriteWordField, 0x00751AD0, (C_008622e8* self, void* , C_008622e8* destination, int , int , int));

static void* __stdcall NetPlayerDDL_ForwardWriteWordField(
    int /*unused1*/, C_008622e8* self, int /*unused10*/,
    int a3, int a4, C_008622e8* destination, int a5)
{
    return C_008622e8_WriteWordField(self, nullptr, destination, a3, a4, a5);
}

HOOK(0x00745540, NetPlayerDDL_ForwardWriteWordField, "auto");
