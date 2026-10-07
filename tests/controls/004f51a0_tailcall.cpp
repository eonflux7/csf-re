#include <cstdint>
#include "hook.h"

// 0x4F51A0 (8 bytes) - pure tail-call thunk:
//   mov ecx, [ecx+4]            ; ecx = sub-object referenced by this+4
//   mov eax, [ecx]              ; eax = that sub-object's vtable
//   jmp dword ptr [eax+0x2C]    ; tail-call virtual method at slot 0x2C/4 = 11
// Takes no stack arguments (nothing is popped) and never touches edx.
static int __fastcall C_Unknown_InvokeSubobjectVfunc2C(void* self, void* /*edx*/)
{
    // mov ecx, dword ptr [ecx + 4]
    const uint32_t subobject_addr = *reinterpret_cast<const uint32_t*>(static_cast<const uint8_t*>(self) + 4);
    void* subobject = reinterpret_cast<void*>(subobject_addr);

    // mov eax, dword ptr [ecx]  ->  jmp dword ptr [eax + 0x2C]
    void** vtbl = *reinterpret_cast<void***>(subobject);
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[0x2C / 4])(subobject, nullptr);
}

HOOK(0x4F51A0, C_Unknown_InvokeSubobjectVfunc2C, "auto");
