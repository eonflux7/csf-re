#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, ForwardRequest, 0x0048E570,
                (void* self, void* edx, int* a2));

static int __fastcall S_0048e570_ForwardRequest(
    S_0048e570* self, void* /*edx*/, int* request)
{
    return ForwardRequest(self, nullptr, request);
}

HOOK(0x00491F80, S_0048e570_ForwardRequest, "auto");
