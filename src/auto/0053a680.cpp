#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0053A680
//   mov ecx, [ecx+0x10]      ; inner object*
//   mov eax, [ecx]           ; its vtable
//   push 0
//   push 0
//   call dword ptr [eax+0x64] ; virtual slot 0x64/4 = 25, __thiscall, two zero args
//   ret
// Result is returned unchanged (widened to full eax by the callee).
static int __fastcall CMenuMain_InvokeInnerVirtual(CMenuMain* self, void* /*edx*/)
{
    void* inner = CSF_AT(void*, self, 0x10);
    void** innerVtbl = *reinterpret_cast<void***>(inner);
    return (reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(innerVtbl[25]))(inner, nullptr, 0, 0);
}

HOOK(0x53A680, CMenuMain_InvokeInnerVirtual, "auto");
