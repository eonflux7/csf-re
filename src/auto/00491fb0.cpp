#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(char, __fastcall, ForwardInputEvent, 0x0040F460,
               (void* self, void* edx, char a2, int a3));

static char __fastcall ForwardInputEventToDelegate(void* self, void* /*edx*/,
                                                   char event, int value)
{
    return ForwardInputEvent(CSF_AT(void*, self, 0x4C), nullptr, event, value);
}

HOOK(0x00491FB0, ForwardInputEventToDelegate, "auto");
