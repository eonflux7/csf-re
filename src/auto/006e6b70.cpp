#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Invokes the virtual method in vtable slot 23 (byte offset 0x5C) on this object,
// then returns the address of the embedded sub-object stored at offset +0x28.
static char* __fastcall C_00861d48_InvokeVtbl23AndGetPtr28(C_00861d48* self, void* /*edx*/)
{
    reinterpret_cast<void(__fastcall*)(C_00861d48*, void*)>(self->vtbl[23])(self, nullptr);
    return reinterpret_cast<char*>(self) + 40;
}

HOOK(0x006E6B70, C_00861d48_InvokeVtbl23AndGetPtr28, "auto");
