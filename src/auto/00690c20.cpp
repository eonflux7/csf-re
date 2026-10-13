#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_655CC0, 0x00655CC0, (void* self, void* edx, char* a2, unsigned int a3));

static char* __fastcall MemDebug_RegisterBlockAndReturnPtr(char* self, void* /*edx*/, C_008622e8* arg0)
{
    sub_655CC0(arg0, nullptr, self, 1u);
    return self;
}

HOOK(0x00690C20, MemDebug_RegisterBlockAndReturnPtr, "auto");
