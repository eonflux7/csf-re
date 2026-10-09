#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_769700, 0x00769700, (void* self, void* edx, int a2));

static unsigned int __fastcall MissionScript_ResolveFlagsWithTagBit(void* self, void* /*edx*/, int flags) {
    if ((flags & 0x800000) != 0) {
        void* target = CSF_AT(void*, self, 0x48);
        unsigned int resolved = (unsigned int)sub_769700(target, nullptr, flags);
        return resolved | 0x800000;
    }
    void* owner = CSF_AT(void*, self, 0x44);
    void* target = CSF_AT(void*, owner, 0x28);
    return (unsigned int)sub_769700(target, nullptr, flags);
}

HOOK(0x0046BE40, MissionScript_ResolveFlagsWithTagBit, "auto");
