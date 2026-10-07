#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// 0x00686C10: __thiscall (ecx = object), no stack args, returns int (ret 0).
CSF_FUNC(int, __fastcall, Sub_686C10, 0x686C10, (void* self, void* edx));

// 0x00728AA0: gets a child via vtable slot 6 (offset 0x18), processes it through 0x686C10,
// and if that yields a non-null result, forwards (a2, processed) to vtable slot 33 (offset 0x84).
static int __fastcall C_00865a10_ForwardProcessedChild(C_00865a10* self, void* /*edx*/, int a2)
{
    int result = ((int (__fastcall*)(void*, void*))self->vtbl[6])(self, nullptr);
    if (result)
    {
        result = Sub_686C10(reinterpret_cast<void*>(result), nullptr);
        if (result)
            return ((int (__fastcall*)(void*, void*, int, int))self->vtbl[33])(self, nullptr, a2, result);
    }
    return result;
}

HOOK(0x728AA0, C_00865a10_ForwardProcessedChild, "auto");
