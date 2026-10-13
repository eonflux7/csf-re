#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Import slot for kernel32!CreateMutexA (indirect call through the IAT).
CSF_GLOBAL(void*, g_pCreateMutexA_Import, 0x0082D19C);

// Calling the stdcall import through a fastcall-typed pointer keeps ecx equal to
// the incoming `this` (as in the original) and edx equal to the incoming edx.
// The stack holds the same three arguments; the stdcall callee pops them (ret 12),
// so the fastcall caller pops nothing, as in the original.
using CreateMutexAViaEcxFn = void* (__fastcall*)(void* ecxPreserved, void* edxPreserved,
                                                 void* lpMutexAttributes, int bInitialOwner,
                                                 const char* lpName);

// __thiscall ctor: stores the vtable, then creates a mutex with the given name
// and keeps its handle in field_0004.
static uint32_t* __fastcall C_0085eb30_InitNamedMutex(C_0085eb30* self, void* edx, const char* lpName)
{
    self->vtbl = CSF_VTBL(0x0085EB30);

    CreateMutexAViaEcxFn createMutexA = reinterpret_cast<CreateMutexAViaEcxFn>(g_pCreateMutexA_Import);
    self->field_0004 = reinterpret_cast<uint32_t>(
        createMutexA(static_cast<void*>(self), edx, nullptr, 0, lpName));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0069E3E0, C_0085eb30_InitNamedMutex, "auto");
