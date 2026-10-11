#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_RegisterObject, 0x00475850, (int a1));
CSF_FUNC(void*, __fastcall, C_00840ec0_Construct, 0x0051CA80, (void* self, void* edx));
CSF_FUNC(C_00840ee0*, __fastcall, C_00840ee0_Construct, 0x0051D1E0, (C_00840ee0* self, void*));
CSF_FUNC(C_00840f00*, __fastcall, C_00840f00_Construct, 0x0051DB20, (C_00840f00* self, void*));
CSF_FUNC(C_00840f28*, __fastcall, C_00840f28_Construct, 0x0051E070, (C_00840f28* self, void*));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static void C_00840fdc_AppendDefinition(C_00840fdc* self, void* definition)
{
    if (self->field_0014 >= self->field_0010)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&self->field_000c),
            nullptr, static_cast<int>(self->field_0018));
    }

    const uint32_t count = self->field_0014;
    void** destination = reinterpret_cast<void**>(self->field_000c) + count;
    self->field_0014 = count + 1;
    *destination = definition;
}

static uint32_t* __fastcall C_00840fdc_Construct(C_00840fdc* self, void* /*edx*/)
{
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x008316B4));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00840F94));
    self->vtbl = CSF_VTBL(0x00840FDC);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00840FBC));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00840FB4));

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 10;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 10;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 10;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 10;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 10;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0078 = 10;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 10;

    void* sharedObject = Mem_Alloc(4);
    if (sharedObject)
        CSF_AT(void**, sharedObject, 0) = CSF_VTBL(0x00840F9C);
    self->field_0090 = reinterpret_cast<uint32_t>(sharedObject);

    MpSpawnPoints_RegisterObject(reinterpret_cast<int>(self));

    auto* firstStorage = static_cast<C_00840ee0*>(Mem_Alloc(36));
    void* firstDefinition = firstStorage
        ? C_00840ee0_Construct(firstStorage, nullptr) : nullptr;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, firstDefinition, 0)[12 / 4])(
            firstDefinition, nullptr, self->field_0090);
    C_00840fdc_AppendDefinition(self, firstDefinition);

    void* secondStorage = Mem_Alloc(184);
    void* secondDefinition = secondStorage
        ? C_00840ec0_Construct(secondStorage, nullptr) : nullptr;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, secondDefinition, 0)[12 / 4])(
            secondDefinition, nullptr, self->field_0090);
    C_00840fdc_AppendDefinition(self, secondDefinition);

    auto* thirdStorage = static_cast<C_00840f00*>(Mem_Alloc(48));
    void* thirdDefinition = thirdStorage
        ? C_00840f00_Construct(thirdStorage, nullptr) : nullptr;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, thirdDefinition, 0)[12 / 4])(
            thirdDefinition, nullptr, self->field_0090);
    C_00840fdc_AppendDefinition(self, thirdDefinition);

    auto* fourthStorage = static_cast<C_00840f28*>(Mem_Alloc(24));
    void* fourthDefinition = fourthStorage
        ? C_00840f28_Construct(fourthStorage, nullptr) : nullptr;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, fourthDefinition, 0)[12 / 4])(
            fourthDefinition, nullptr, self->field_0090);
    C_00840fdc_AppendDefinition(self, fourthDefinition);

    void* configuration = reinterpret_cast<void*>(self->field_0090);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, configuration, 0)[4 / 4])(
            configuration, nullptr, 0, 25);

    configuration = reinterpret_cast<void*>(self->field_0090);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, configuration, 0)[4 / 4])(
            configuration, nullptr, 1, 5);

    self->field_008c = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0051FD70, C_00840fdc_Construct, "auto");
