#include <cstdint>
#include "globals.h"
#include "hook.h"
static int32_t __fastcall SharedHelper(void* self, void*) {
    uint32_t begin = CSF_AT(uint32_t, self, 0x10);
    return begin ? static_cast<int32_t>(CSF_AT(uint32_t, self, 0x14) - begin) >> 2 : 0;
}
HOOK(0x00672AC0, SharedHelper, "stl");
