#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, CheckSubObjectCondition, 0x004D4C80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static int __fastcall C_00834fa8_ResolveTargetLookup(C_00834fa8* self, void* /*edx*/) {
    if (CheckSubObjectCondition(CSF_AT(void*, self, 0x8), nullptr)) {
        return CharTask_LookupByOffset(self, nullptr, CSF_AT(int, self, 0xC) + 60);
    }
    return CharTask_LookupByOffset(self, nullptr, CSF_AT(int, self, 0xC) + 40);
}

HOOK(0x004D4D30, C_00834fa8_ResolveTargetLookup, "auto");
