#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4DB440, 0x004DB440, (void* self, void* edx, int a2));

static int __fastcall C_00834fa8_RunStateHandlerAtOffset14(C_00834fa8* self, void* /*edx*/) {
    int handlerArg = CSF_AT(int, self, 0x0C) + 0x14;
    return sub_4DB440(self, nullptr, handlerArg);
}

HOOK(0x004210C0, C_00834fa8_RunStateHandlerAtOffset14, "auto");
