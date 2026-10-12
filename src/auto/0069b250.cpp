#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00471340_RunOperation, 0x0069B100, (void* self, void* edx, int a2, unsigned int a3, char a4));

static unsigned int __fastcall S_00471340_StoreModeAndRun(S_00471340* self, void* /*edx*/, int a2, unsigned int a3, char a4)
{
    self->field_0040 = static_cast<uint8_t>(a4);
    unsigned int result = S_00471340_RunOperation(self, nullptr, a2, a3, a4);
    self->field_0038 = 50;
    if (result) {
        self->field_0041 = 1;
    }
    return result;
}

HOOK(0x0069B250, S_00471340_StoreModeAndRun, "auto");
