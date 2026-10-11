#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, Scene_DestroyMemberList, 0x00446150, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Scene_ResetSubsystemB0, 0x00446210, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int DestroyFortyByteElementArray(void* elements)
{
    auto* bytes = static_cast<uint8_t*>(elements);
    void* allocation = bytes - 4;
    const uint32_t count = CSF_AT(uint32_t, allocation, 0);

    if (static_cast<int32_t>(count - 1) >= 0)
    {
        auto* element = bytes + 8 * (5 * count);
        uint32_t remaining = count;
        do
        {
            element -= 40;
            --remaining;
            CSF_AT(void**, element, 0x8) = CSF_VTBL(0x0082D410);
        } while (remaining);
    }

    return Mem_Free(allocation);
}

static int DestroyThirtySixByteElementArray(void* elements)
{
    auto* bytes = static_cast<uint8_t*>(elements);
    void* allocation = bytes - 4;
    const uint32_t count = CSF_AT(uint32_t, allocation, 0);

    if (static_cast<int32_t>(count - 1) >= 0)
    {
        auto* element = bytes + 4 * (9 * count);
        uint32_t remaining = count;
        do
        {
            element -= 36;
            --remaining;
            CSF_AT(void**, element, 0x4) = CSF_VTBL(0x0082D410);
        } while (remaining);
    }

    return Mem_Free(allocation);
}

static void DestroyPolymorphicArray(void* elements)
{
    auto* allocation = static_cast<uint8_t*>(elements) - 4;
    if (CSF_AT(uint32_t, allocation, 0))
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, elements, 0)[0])(elements, nullptr, 3);
    }
    else
    {
        Mem_Free(allocation);
    }
}

static int __fastcall C_00831120_Destroy(C_00831120* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00831120);
    self->field_0004 =
        reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00831118));

    Scene_ResetSubsystemB0(self, nullptr);

    void* ownedObject = reinterpret_cast<void*>(self->field_0114);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_0114 = 0;
    }

    if (self->field_0100)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0100));
        self->field_0100 = 0;
    }

    void* elements = reinterpret_cast<void*>(self->field_00ec);
    if (elements)
    {
        DestroyFortyByteElementArray(elements);
        self->field_00ec = 0;
    }

    elements = reinterpret_cast<void*>(self->field_00d8);
    if (elements)
    {
        DestroyThirtySixByteElementArray(elements);
        self->field_00d8 = 0;
    }

    elements = reinterpret_cast<void*>(self->field_00c8);
    if (elements)
    {
        DestroyPolymorphicArray(elements);
        self->field_00c8 = 0;
    }

    Scene_DestroyMemberList(&self->field_0090, nullptr);
    Scene_DestroyMemberList(&self->field_0060, nullptr);

    elements = reinterpret_cast<void*>(self->field_0050);
    if (elements)
    {
        DestroyPolymorphicArray(elements);
        self->field_0050 = 0;
    }

    elements = reinterpret_cast<void*>(self->field_0040);
    if (elements)
    {
        DestroyPolymorphicArray(elements);
        self->field_0040 = 0;
    }

    if (self->field_002c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_002c));
        self->field_002c = 0;
    }

    elements = reinterpret_cast<void*>(self->field_001c);
    if (elements)
    {
        DestroyFortyByteElementArray(elements);
        self->field_001c = 0;
    }

    int result = static_cast<int32_t>(self->field_000c);
    if (result)
    {
        result = DestroyThirtySixByteElementArray(
            reinterpret_cast<void*>(self->field_000c));
        self->field_000c = 0;
    }

    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x00446740, C_00831120_Destroy, "auto");
