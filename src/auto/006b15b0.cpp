#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);

// Forwards to the object stored at +0x8c through its vtable slot 7 (+0x1c) when set;
// otherwise the original returns g_MissingMetricValue (-1.0 in the pseudocode is the
// intended value; the disassembly loads g_MissingMetricValue).
static hook::Tail __fastcall ForwardMetricOrMissingValue(void* self, const uint32_t* args) {
    (void)args;
    void* targetObj = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8C));
    if (targetObj == nullptr) {
        uint32_t missingBits = 0;
        std::memcpy(&missingBits, &g_MissingMetricValue, sizeof(missingBits));
        return hook::tail_ret(missingBits);
    }
    return hook::tail_jump(targetObj, (*reinterpret_cast<void***>(targetObj))[0x1C / 4]);
}

HOOK_TAIL(0x6B15B0, ForwardMetricOrMissingValue, "auto");
