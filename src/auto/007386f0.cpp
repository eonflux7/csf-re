#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));

// Releases the two owned blocks, clears the bookkeeping fields, then tail-jumps
// through the object's vtable slot at +0x24 (vClearFields) with the caller's args untouched.
static hook::Tail __fastcall C_00867358_ReleaseBlocksAndForwardClear(void* self, const uint32_t* args)
{
    (void)args;
    C_00867358* obj = reinterpret_cast<C_00867358*>(self);

    if (obj->field_0010) {
        FreeBlock_690F70(reinterpret_cast<void*>(obj->field_0010));
        obj->field_0010 = 0;
    }
    if (obj->field_0004) {
        FreeBlock_690F70(reinterpret_cast<void*>(obj->field_0004));
        obj->field_0004 = 0;
    }

    obj->field_0008 = 0;
    obj->field_0014 = 0;

    return hook::tail_jump(self, (*reinterpret_cast<void***>(obj))[0x24 / 4]);
}

HOOK_TAIL(0x007386F0, C_00867358_ReleaseBlocksAndForwardClear, "auto");
