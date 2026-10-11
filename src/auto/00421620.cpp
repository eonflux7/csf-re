#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TaskBase_Destroy, 0x0041E210, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Member_Release, 0x004DB3C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, OwnedBuffer_Release, 0x0075A190, (void* self, void* edx));

static void ReleaseAt(uint8_t* base, uint32_t off)
{
    Member_Release(base + off, nullptr);
}

static int __fastcall C_0082f9f8_Destroy(C_0082f9f8* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    self->vtbl = CSF_VTBL(0x0082F9F8);
    CSF_AT(uint32_t, self, 0x4) = (uint32_t)CSF_ADDR_VT(0x0082F9DC);

    if (CSF_AT(void*, self, 0x2A0)) {
        Mem_Free(CSF_AT(void*, self, 0x2A0));
        CSF_AT(void*, self, 0x2A0) = nullptr;
    }

    OwnedBuffer_Release(base + 0x25C, nullptr);

    ReleaseAt(base, 0x1A8 + 0x3C);
    ReleaseAt(base, 0x1A8 + 0x28);
    ReleaseAt(base, 0x1A8 + 0x14);
    ReleaseAt(base, 0x1A8);
    ReleaseAt(base, 0x194);
    ReleaseAt(base, 0x180);

    ReleaseAt(base, 0x100 + 0x68);
    ReleaseAt(base, 0x100 + 0x54);
    ReleaseAt(base, 0x100 + 0x3C);
    ReleaseAt(base, 0x100 + 0x28);
    ReleaseAt(base, 0x100 + 0x14);
    ReleaseAt(base, 0x100);

    ReleaseAt(base, 0xB0 + 0x3C);
    ReleaseAt(base, 0xB0 + 0x28);
    ReleaseAt(base, 0xB0 + 0x14);
    ReleaseAt(base, 0xB0);

    ReleaseAt(base, 0x74 + 0x28);
    ReleaseAt(base, 0x74 + 0x14);
    ReleaseAt(base, 0x74);

    ReleaseAt(base, 0x60);
    ReleaseAt(base, 0x48);
    ReleaseAt(base, 0x34);
    ReleaseAt(base, 0x20);

    return TaskBase_Destroy(self, nullptr);
}

HOOK(0x00421620, C_0082f9f8_Destroy, "auto");
