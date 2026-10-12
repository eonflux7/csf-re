#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GfxSkin_ReleaseCachedSkinObject, 0x006C5740, ());
CSF_FUNC(char, __cdecl, ReleaseSharedResources, 0x006E2F50, ());

static int __fastcall C_00860130_DestroyOwnedResources(
    C_00860130* self, void* /*edx*/)
{
    void* object = reinterpret_cast<void*>(self->field_005c);
    self->vtbl = CSF_VTBL(0x00860130);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_005c = 0;
    }

    object = reinterpret_cast<void*>(self->field_0068);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_0068 = 0;
    }

    object = reinterpret_cast<void*>(self->field_006c);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_006c = 0;
    }

    object = reinterpret_cast<void*>(self->field_0054);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_0054 = 0;
    }

    object = reinterpret_cast<void*>(self->field_0064);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_0064 = 0;
    }

    object = reinterpret_cast<void*>(self->field_0060);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_0060 = 0;
    }

    object = reinterpret_cast<void*>(self->field_0058);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_0058 = 0;
    }

    GfxSkin_ReleaseCachedSkinObject();

    for (uint32_t index = 0; index < self->field_0010; ++index)
    {
        object = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(self->field_0008)[index]);
        if (object)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0008)[index] = 0;
        }
    }

    uint32_t count = self->field_0084;
    self->field_0010 = 0;
    if (count)
    {
        uint32_t index = 0;
        do
        {
            object = reinterpret_cast<void*>(
                reinterpret_cast<uint32_t*>(self->field_007c)[index]);
            if (object)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
                reinterpret_cast<uint32_t*>(self->field_007c)[index] = 0;
            }
            count = self->field_0084;
            ++index;
        } while (index < count);
    }

    object = reinterpret_cast<void*>(self->field_0074);
    self->field_0084 = 0;
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0074 = 0;
    }

    object = reinterpret_cast<void*>(self->field_008c);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_008c = 0;
    }

    ReleaseSharedResources();

    void* screen = reinterpret_cast<void*>(self->field_0070);
    if (screen)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, screen, 0)[0x30 / 4])(screen, nullptr);

        screen = reinterpret_cast<void*>(self->field_0070);
        if (screen)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, screen, 0)[1])(screen, nullptr, 1);
            self->field_0070 = 0;
        }
    }

    void* array = reinterpret_cast<void*>(self->field_007c);
    if (array)
    {
        Mem_Free(array);
        self->field_007c = 0;
    }

    int result = static_cast<int>(self->field_0008);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0008));
        self->field_0008 = 0;
    }
    return result;
}

HOOK(0x006B16E0, C_00860130_DestroyOwnedResources, "auto");
