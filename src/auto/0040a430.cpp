#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

CSF_FUNC(uint32_t*, __fastcall, C_0082d58c_Construct, 0x00405950, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082d648_Construct, 0x00406130, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082d77c_Construct, 0x00406FA0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082d5e0_Construct, 0x004086B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082d8a0_Construct, 0x00409EE0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082d908_Construct, 0x00409FC0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082d970_Construct, 0x0040A0A0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082d9d8_Construct, 0x0040A180, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082da40_Construct, 0x0040A260, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082daa8_Construct, 0x0040A350, (void* self, void* edx));

static uint32_t* __cdecl AllocateSubobjectHandle(int type)
{
    switch (type)
    {
    case 0:
    {
        void* object = Mem_Alloc(44);
        return object ? C_0082d77c_Construct(object, nullptr) : nullptr;
    }
    case 1:
    {
        void* object = Mem_Alloc(60);
        return object ? C_0082d5e0_Construct(object, nullptr) : nullptr;
    }
    case 2:
    {
        void* object = Mem_Alloc(60);
        return object ? C_0082da40_Construct(object, nullptr) : nullptr;
    }
    case 3:
    {
        void* object = Mem_Alloc(60);
        return object ? C_0082daa8_Construct(object, nullptr) : nullptr;
    }
    case 5:
    {
        void* object = Mem_Alloc(60);
        return object ? C_0082d648_Construct(object, nullptr) : nullptr;
    }
    case 6:
    {
        void* object = Mem_Alloc(60);
        return object ? C_0082d908_Construct(object, nullptr) : nullptr;
    }
    case 7:
    {
        void* object = Mem_Alloc(12);
        if (!object)
            return nullptr;

        CSF_AT(void**, object, 0x04) = CSF_VTBL(0x0082D428);
        CSF_AT(void**, object, 0x00) = CSF_VTBL(0x0082D6AC);
        CSF_AT(void**, object, 0x04) = CSF_VTBL(0x0082D690);
        CSF_AT(uint32_t, object, 0x08) = 0;
        return static_cast<uint32_t*>(object);
    }
    case 8:
    {
        void* object = Mem_Alloc(104);
        if (!object)
            return nullptr;

        CSF_AT(void**, object, 0x04) = CSF_VTBL(0x0082D428);
        CSF_AT(void**, object, 0x00) = CSF_VTBL(0x0082D844);
        CSF_AT(void**, object, 0x04) = CSF_VTBL(0x0082D828);
        CSF_AT(uint32_t, object, 0x28) = 0;
        CSF_AT(uint32_t, object, 0x2C) = 0;
        CSF_AT(uint32_t, object, 0x30) = 0;
        CSF_AT(uint32_t, object, 0x34) = 0;
        CSF_AT(uint32_t, object, 0x38) = 0;
        CSF_AT(uint32_t, object, 0x3C) = 0;
        CSF_AT(uint32_t, object, 0x40) = 0;
        CSF_AT(uint32_t, object, 0x44) = 0;
        CSF_AT(uint32_t, object, 0x48) = 0;
        CSF_AT(uint32_t, object, 0x4C) = 0;
        CSF_AT(uint32_t, object, 0x50) = 0;
        CSF_AT(uint32_t, object, 0x54) = 0;
        CSF_AT(uint32_t, object, 0x58) = 0;
        CSF_AT(uint32_t, object, 0x5C) = 0;
        CSF_AT(uint32_t, object, 0x60) = 0;
        CSF_AT(uint32_t, object, 0x08) = 0;
        CSF_AT(uint32_t, object, 0x0C) = 0;
        CSF_AT(uint32_t, object, 0x10) = 0;
        CSF_AT(uint32_t, object, 0x14) = 0;
        CSF_AT(float, object, 0x18) = 80.0f;
        CSF_AT(uint8_t, object, 0x1C) = 0;
        CSF_AT(uint8_t, object, 0x24) = 1;
        CSF_AT(float, object, 0x20) = 1.0f;
        CSF_AT(uint32_t, object, 0x64) = 0;
        return static_cast<uint32_t*>(object);
    }
    case 9:
    {
        void* object = Mem_Alloc(60);
        return object ? C_0082d970_Construct(object, nullptr) : nullptr;
    }
    case 10:
    {
        auto* object = static_cast<C_0082d58c*>(Mem_Alloc(68));
        if (!object)
            return nullptr;

        C_0082d58c_Construct(object, nullptr);
        CSF_AT(uint8_t, object, 0x39) = 0;
        CSF_AT(uint32_t, object, 0x40) = 0;
        object->vtbl =
            reinterpret_cast<decltype(object->vtbl)>(CSF_VTBL(0x0082D70C));
        object->field_0004 =
            reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D6F0));
        CSF_AT(uint8_t, object, 0x38) = 1;
        CSF_AT(uint32_t, object, 0x3C) = 0x40000000;
        return reinterpret_cast<uint32_t*>(object);
    }
    case 11:
    {
        auto* object = static_cast<C_0082d58c*>(Mem_Alloc(56));
        if (!object)
            return nullptr;

        C_0082d58c_Construct(object, nullptr);
        object->vtbl =
            reinterpret_cast<decltype(object->vtbl)>(CSF_VTBL(0x0082D510));
        object->field_0004 =
            reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D4F4));
        return reinterpret_cast<uint32_t*>(object);
    }
    case 12:
    {
        void* object = Mem_Alloc(12);
        if (!object)
            return nullptr;

        CSF_AT(void**, object, 0x04) = CSF_VTBL(0x0082D428);
        CSF_AT(void**, object, 0x00) = CSF_VTBL(0x0082D7E4);
        CSF_AT(void**, object, 0x04) = CSF_VTBL(0x0082D7C8);
        CSF_AT(uint32_t, object, 0x08) = 0;
        return static_cast<uint32_t*>(object);
    }
    case 13:
    {
        void* object = Mem_Alloc(60);
        return object ? C_0082d9d8_Construct(object, nullptr) : nullptr;
    }
    case 14:
    {
        void* object = Mem_Alloc(60);
        return object ? C_0082d8a0_Construct(object, nullptr) : nullptr;
    }
    default:
        return nullptr;
    }
}

HOOK(0x0040A430, AllocateSubobjectHandle, "auto");
