#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __stdcall, GfxSkin_ReleaseBufferElements, 0x006B4DC0, (int a1, int a2));

static void GfxSkin_FreeArrayStorage(
    uint32_t& storage, uint32_t& count, uint32_t& capacity)
{
    uint32_t block = storage;
    count = 0;
    capacity = 0;
    if (block)
    {
        Mem_Free(reinterpret_cast<void*>(block));
        storage = 0;
    }
}

static int __fastcall C_008608e8_ClearBuffers(
    C_008608e8* self, void* /*edx*/)
{
    GfxSkin_FreeArrayStorage(self->field_0030, self->field_0034, self->field_0038);
    GfxSkin_FreeArrayStorage(self->field_0040, self->field_0044, self->field_0048);
    GfxSkin_FreeArrayStorage(self->field_0050, self->field_0054, self->field_0058);
    GfxSkin_FreeArrayStorage(self->field_0060, self->field_0064, self->field_0068);
    GfxSkin_FreeArrayStorage(self->field_0070, self->field_0074, self->field_0078);
    GfxSkin_FreeArrayStorage(self->field_0080, self->field_0084, self->field_0088);
    GfxSkin_FreeArrayStorage(self->field_0090, self->field_0094, self->field_0098);
    GfxSkin_FreeArrayStorage(self->field_00a0, self->field_00a4, self->field_00a8);
    GfxSkin_FreeArrayStorage(self->field_00b0, self->field_00b4, self->field_00b8);
    GfxSkin_FreeArrayStorage(self->field_00c0, self->field_00c4, self->field_00c8);
    GfxSkin_FreeArrayStorage(self->field_00d0, self->field_00d4, self->field_00d8);
    GfxSkin_FreeArrayStorage(self->field_00e0, self->field_00e4, self->field_00e8);

    int result = static_cast<int>(self->field_013c);
    if (result)
    {
        GfxSkin_ReleaseBufferElements(result, CSF_AT(int, self, 0x140));
        result = static_cast<int>(self->field_013c);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_013c));
    }

    self->field_013c = 0;
    CSF_AT(uint32_t, self, 0x140) = 0;
    CSF_AT(uint32_t, self, 0x144) = 0;
    return result;
}

HOOK(0x006C6E60, C_008608e8_ClearBuffers, "auto");
