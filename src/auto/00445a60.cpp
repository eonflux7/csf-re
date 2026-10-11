#include "hook.h"

static char* __fastcall GetNeighborList(char* self, void* /*edx*/, int listKind)
{
    return listKind == 1 ? self + 0x40 : self + 0x50;
}

HOOK(0x00445A60, GetNeighborList, "auto");
