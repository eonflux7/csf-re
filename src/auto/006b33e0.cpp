#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));
CSF_FUNC(int, __fastcall, GfxTextureLoader_ReleaseIndexedResource, 0x006D4790, (void* self, void* edx, int a1, int a2));
CSF_FUNC(void, __fastcall, ClearFlagIfSet, 0x006E42F0, (void* self, void*));

static void __fastcall C_008601a8_Destroy(C_008601a8* self, void* /*edx*/)
{
    const uint32_t renderResource = self->field_006c;
    self->vtbl = CSF_VTBL(0x00860288);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00860280));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00860260));

    if (renderResource)
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* renderHandle = reinterpret_cast<void*>(RenderHandle_FromObject(global, nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, renderHandle, 0)[0x30 / 4])(
                renderHandle, nullptr, self->field_006c);
        self->field_006c = 0;
    }

    void* embeddedState = reinterpret_cast<uint8_t*>(self) + 0x30;
    if (((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, embeddedState, 0)[0x70 / 4])(embeddedState, nullptr))
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, embeddedState, 0)[0x88 / 4])(embeddedState, nullptr);
    }

    void* object = reinterpret_cast<void*>(self->field_0028);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0x4 / 4])(object, nullptr, 1);
        self->field_0028 = 0;
    }

    object = reinterpret_cast<void*>(self->field_002c);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_002c = 0;
    }

    for (uint32_t i = 0; i < self->field_0024; ++i)
    {
        object = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(self->field_0020)[i]);
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[0x4 / 4])(object, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0020)[i] = 0;
        }
    }
    if (self->field_0020)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0020));
        self->field_0020 = 0;
    }

    for (uint32_t i = 0; i < self->field_001c; ++i)
    {
        object = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(self->field_0018)[i]);
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[0x4 / 4])(object, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0018)[i] = 0;
        }
    }
    if (self->field_0018)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0018));
        self->field_0018 = 0;
    }

    if (self->field_000c)
    {
        if (self->field_0010 != static_cast<uint32_t>(-1))
        {
            void* global = reinterpret_cast<void*>(GetGlobalInt());
            void* accessor = reinterpret_cast<void*>(
                GlobalInt_ResolveAccessor(global, nullptr));
            const int resource = static_cast<int>(self->field_000c);
            const int index = static_cast<int>(self->field_0010);
            void* provider = GetCachedObjectProvider(accessor, nullptr);
            GfxTextureLoader_ReleaseIndexedResource(provider, nullptr, index, resource);
        }
        self->field_000c = 0;
    }

    object = reinterpret_cast<void*>(self->field_0068);
    C_0085e7b8* entries =
        ((C_0085e7b8*(__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr);
    void** payload = reinterpret_cast<void**>(
        C_0085e7b8_FindEntryPayloadByKey(entries, nullptr, 5));
    if (payload)
    {
        object = *payload;
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        }
    }

    object = reinterpret_cast<void*>(self->field_0068);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0x4 / 4])(object, nullptr, 1);
        self->field_0068 = 0;
    }

    if (self->field_0060)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0060));
        self->field_0060 = 0;
    }

    void* flagStorage = reinterpret_cast<void*>(self->field_007c);
    if (flagStorage)
    {
        ClearFlagIfSet(flagStorage, nullptr);
        Mem_Free(flagStorage);
        self->field_007c = 0;
    }

    if (self->field_0084)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0084));
        self->field_0084 = 0;
    }

    GfxObject_DestroyEmbeddedState(reinterpret_cast<uint8_t*>(self) + 0x30, nullptr);
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->vtbl = CSF_VTBL(0x008601A8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0086019C));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006B33E0, C_008601a8_Destroy, "auto");
