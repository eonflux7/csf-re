#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// CErrorLog::ForwardToInnerSink: returns -1 when no inner sink is attached,
// otherwise tail-jumps to the inner sink's vtable slot +0x10 with the sink as `this`.
static hook::Tail __fastcall CErrorLog_ForwardToInnerSink(void* self, const uint32_t* /*args*/)
{
    C_0085ea2c* log = static_cast<C_0085ea2c*>(self);
    uint32_t innerSink = log->field_0004;
    if (innerSink != 0) {
        void* sink = reinterpret_cast<void*>(innerSink);
        return hook::tail_jump(sink, (*reinterpret_cast<void***>(sink))[0x10 / 4]);
    }
    return hook::tail_ret(-1);
}

HOOK_TAIL(0x0069D610, CErrorLog_ForwardToInnerSink, "auto");
