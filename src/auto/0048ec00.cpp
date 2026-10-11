#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void, __fastcall, MpSpawnPoints_DestroyEmbeddedObject, 0x0073F710, (void* self, void* edx));

static int __fastcall C_00833908_Destroy(C_00833908* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00833908);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833888));
    CSF_AT(void**, self, 0x008) = CSF_VTBL(0x00833884);
    self->field_000c = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833878));
    CSF_AT(void**, self, 0x010) = CSF_VTBL(0x00833870);
    self->field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083386C));

    GetGlobalState_50D190();

    if (void* object = reinterpret_cast<void*>(self->field_0044))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0044 = 0;
    }
    if (void* object = reinterpret_cast<void*>(self->field_003c))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_003c = 0;
    }
    if (void* object = reinterpret_cast<void*>(self->field_0040))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0040 = 0;
    }
    if (void* object = reinterpret_cast<void*>(self->field_0030))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0030 = 0;
    }
    if (auto* object = reinterpret_cast<C_008672e4*>(self->field_0034))
    {
        object->vSlot0(1);
        self->field_0034 = 0;
    }
    if (void* object = reinterpret_cast<void*>(self->field_0038))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0038 = 0;
    }
    if (void* object = reinterpret_cast<void*>(self->field_0048))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0048 = 0;
    }
    if (auto* object = reinterpret_cast<C_0082dd78*>(self->field_004c))
    {
        object->vSlot0(1);
        self->field_004c = 0;
    }

    if ((self->field_0170 & 0x100) != 0)
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(global, nullptr));
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, controller, 0)[0x18 / 4])(
                controller, nullptr, self->field_016c);
    }

    self->field_0148 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_013c = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_012c = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_00fc = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));

    if (self->field_00d0)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00d0));
        self->field_00d0 = 0;
    }
    if (self->field_00c0)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00c0));
        self->field_00c0 = 0;
    }
    if (self->field_00ac)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00ac));
        self->field_00ac = 0;
    }
    if (self->field_009c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_009c));
        self->field_009c = 0;
    }

    self->field_001c = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    MpSpawnPoints_DestroyEmbeddedObject(
        reinterpret_cast<uint8_t*>(self) + 0x14, nullptr);
    self->field_000c = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833860));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->vtbl = CSF_VTBL(0x0082D410);

    // The original leaves EAX unspecified after the final void call.
    return 0;
}

HOOK(0x0048EC00, C_00833908_Destroy, "auto");
