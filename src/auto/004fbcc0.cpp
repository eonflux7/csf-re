#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

static uint32_t* __fastcall C_0083f364_Destroy(
    C_0083f364* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x0083F364);

    int managerId = GetGlobalInt();
    void* manager = reinterpret_cast<void*>(ResolveRwManager(managerId, nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*, C_00833908*)>(
        CSF_AT(void**, manager, 0)[0x1C / 4])(
            manager, nullptr,
            reinterpret_cast<C_00833908*>(self->field_0008));

    self->vtbl = CSF_VTBL(0x0082D410);
    if ((flags & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004FBCC0, C_0083f364_Destroy, "auto");
