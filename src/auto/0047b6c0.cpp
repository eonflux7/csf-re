#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static int __fastcall C_00834fa8_LookupCharTaskByStoredOffset(C_00834fa8* self, void* /*edx*/, int /*a2*/, int /*a3*/) {
    // Original: mov eax,[ecx+0xc]; add eax,0x28; push eax; call CharTask_LookupByOffset; ret 8
    // ecx (this) is still intact at the call, so it is passed as the callee's self.
    return CharTask_LookupByOffset(self, nullptr, CSF_AT(int, self, 0x0C) + 0x28);
}

HOOK(0x0047B6C0, C_00834fa8_LookupCharTaskByStoredOffset, "auto");
