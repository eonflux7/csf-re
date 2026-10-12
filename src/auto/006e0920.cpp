#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RpPrtAdvEmitterClassCreate, 0x005784E0, (int a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RpPrtStdPropTabDestroy, 0x0057C770, (uint32_t* a1));
CSF_FUNC(uint32_t*, __cdecl, RpPrtStdPropTabCreate, 0x0057C9A0, (int a1, uint8_t* a2, char* a3));
CSF_FUNC(int, __cdecl, RpPrtStdEmitterClassSetCallBack, 0x0057CE10, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RpPrtStdEmitterClassSetPropTab, 0x0057CE90, (int a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RpPrtStdEmitterClassGetPropTab, 0x0057CED0, (int a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RpPrtStdEClassStdCreate, 0x0057FA90, (int a1));

CSF_FUNC(int, __stdcall, GfxParticleEmitters_ConfigurePrimaryCallbacks,
         0x006E04D0, (int a1, int a2));
CSF_FUNC(int, __stdcall, GfxParticleEmitters_ConfigureSecondaryCallbacks,
         0x006E0670, (int a1, int a2));

static void GfxParticleEmitters_InitializeAdvancedCallbackSlots(
    uint8_t* callbacks)
{
    std::memset(callbacks, 0, 0x84);
    CSF_AT(uint32_t, callbacks, 0x00) = CSF_ADDR_FN(0x0057DBA0);
    CSF_AT(uint32_t, callbacks, 0x04) = CSF_ADDR_FN(0x0057EF90);
    CSF_AT(uint32_t, callbacks, 0x08) = CSF_ADDR_FN(0x0057F050);
    CSF_AT(uint32_t, callbacks, 0x14) = CSF_ADDR_FN(0x0057ED00);
    CSF_AT(uint32_t, callbacks, 0x18) = CSF_ADDR_FN(0x0057EDA0);
    CSF_AT(uint32_t, callbacks, 0x1C) = CSF_ADDR_FN(0x004B2A30);
    CSF_AT(uint32_t, callbacks, 0x20) = CSF_ADDR_FN(0x004B2A30);
    CSF_AT(uint32_t, callbacks, 0x24) = CSF_ADDR_FN(0x004B2A30);
    CSF_AT(uint32_t, callbacks, 0x28) = CSF_ADDR_FN(0x004B2A30);
    CSF_AT(uint32_t, callbacks, 0x40) = CSF_ADDR_FN(0x007038B0);
    CSF_AT(uint32_t, callbacks, 0x2C) = CSF_ADDR_FN(0x007043D0);
    CSF_AT(uint32_t, callbacks, 0x30) = CSF_ADDR_FN(0x006C2AB0);
    CSF_AT(uint32_t, callbacks, 0x34) = CSF_ADDR_FN(0x00704C30);
}

static uint32_t __fastcall S_0041e5f0_InitializeParticleEmitterClasses(
    S_0041e5f0* self, void* /*edx*/)
{
    alignas(8) uint8_t firstArrayBytes[0x400];
    alignas(8) uint8_t secondArrayBytes[0x404];
    alignas(8) uint8_t callbackBytes[0x84];

    auto* firstArray = reinterpret_cast<uint32_t*>(firstArrayBytes);
    auto* secondArray = reinterpret_cast<uint32_t*>(secondArrayBytes);
    uint32_t propertyTableAddress;

    if (self->field_0004 < 4)
        S_0041e5f0_GrowArrayCapacity(
            self, nullptr, static_cast<int>(4 - self->field_0004));

    for (uint32_t index = self->field_0008; index < 4; ++index)
        reinterpret_cast<uint32_t*>(self->field_0000)[index] = 0;

    self->field_0008 = 4;

    int emitterClass = RpPrtAdvEmitterClassCreate(131131, nullptr);
    GfxParticleEmitters_ConfigurePrimaryCallbacks(emitterClass, 0);
    GfxParticleEmitters_ConfigureSecondaryCallbacks(emitterClass, 0);
    reinterpret_cast<uint32_t*>(self->field_0000)[0] = emitterClass;

    emitterClass = RpPrtAdvEmitterClassCreate(131187, nullptr);
    propertyTableAddress = 0;
    RpPrtStdEmitterClassGetPropTab(emitterClass, &propertyTableAddress);

    void* propertyTable = reinterpret_cast<void*>(propertyTableAddress);
    int propertyCount = CSF_AT(int, propertyTable, 0x10);
    for (int index = 0; index < propertyCount; ++index)
    {
        secondArray[index] =
            CSF_AT(uint32_t*, propertyTable, 0x18)[index];
        firstArray[index] =
            CSF_AT(uint32_t*, propertyTable, 0x20)[index];
    }

    secondArray[propertyCount] = 10;
    firstArray[propertyCount] = 8;
    uint32_t* extendedTable = RpPrtStdPropTabCreate(
        propertyCount + 1, secondArrayBytes,
        reinterpret_cast<char*>(firstArrayBytes));
    RpPrtStdEmitterClassSetPropTab(emitterClass, extendedTable);
    RpPrtStdPropTabDestroy(extendedTable);

    GfxParticleEmitters_InitializeAdvancedCallbackSlots(callbackBytes);
    CSF_AT(uint32_t, callbackBytes, 0x0C) = CSF_ADDR_FN(0x00703D20);
    CSF_AT(uint32_t, callbackBytes, 0x38) = CSF_ADDR_FN(0x0057F0B0);

    RpPrtStdEmitterClassSetCallBack(
        emitterClass, 2, reinterpret_cast<int>(callbackBytes));
    reinterpret_cast<uint32_t*>(self->field_0000)[2] = emitterClass;

    emitterClass = RpPrtAdvEmitterClassCreate(131187, nullptr);
    propertyTableAddress = 0;
    RpPrtStdEmitterClassGetPropTab(emitterClass, &propertyTableAddress);

    propertyCount = CSF_AT(
        int, reinterpret_cast<void*>(propertyTableAddress), 0x10);
    for (int index = 0; index < propertyCount; ++index)
    {
        firstArray[index] = CSF_AT(
            uint32_t*, reinterpret_cast<void*>(propertyTableAddress),
            0x18)[index];
        secondArray[index] = CSF_AT(
            uint32_t*, reinterpret_cast<void*>(propertyTableAddress),
            0x20)[index];
    }

    firstArray[propertyCount] = 10;
    secondArray[propertyCount] = 8;
    firstArray[propertyCount + 1] = 11;
    secondArray[propertyCount + 1] = 4;

    CSF_AT(uint32_t*, secondArrayBytes, 0x400) =
        RpPrtStdPropTabCreate(
            propertyCount + 2, firstArrayBytes,
            reinterpret_cast<char*>(secondArrayBytes));
    RpPrtStdEmitterClassSetPropTab(
        emitterClass, CSF_AT(uint32_t*, secondArrayBytes, 0x400));
    RpPrtStdPropTabDestroy(
        CSF_AT(uint32_t*, secondArrayBytes, 0x400));

    GfxParticleEmitters_InitializeAdvancedCallbackSlots(callbackBytes);
    CSF_AT(uint32_t, callbackBytes, 0x6C) =
        CSF_ADDR_FN(0x007038E0);
    CSF_AT(uint32_t, callbackBytes, 0x58) = CSF_ADDR_FN(0x006C2AB0);
    CSF_AT(uint32_t, callbackBytes, 0x5C) = CSF_ADDR_FN(0x006C2AB0);
    CSF_AT(uint32_t, callbackBytes, 0x60) =
        CSF_ADDR_FN(0x00704CC0);
    CSF_AT(uint32_t, callbackBytes, 0x0C) =
        CSF_ADDR_FN(0x00704000);

    RpPrtStdEmitterClassSetCallBack(
        emitterClass, 3, reinterpret_cast<int>(callbackBytes));
    reinterpret_cast<uint32_t*>(self->field_0000)[3] = emitterClass;

    uint32_t* standardClass = RpPrtStdEClassStdCreate(33);
    propertyTableAddress = 0;
    RpPrtStdEmitterClassGetPropTab(
        reinterpret_cast<int>(standardClass), &propertyTableAddress);

    propertyCount = CSF_AT(
        int, reinterpret_cast<void*>(propertyTableAddress), 0x10);
    for (int index = 0; index < propertyCount; ++index)
    {
        secondArray[index] = CSF_AT(
            uint32_t*, reinterpret_cast<void*>(propertyTableAddress),
            0x18)[index];
        firstArray[index] = CSF_AT(
            uint32_t*, reinterpret_cast<void*>(propertyTableAddress),
            0x20)[index];
    }

    secondArray[propertyCount] = 9;
    firstArray[propertyCount] = 68;

    CSF_AT(uint32_t*, secondArrayBytes, 0x400) =
        RpPrtStdPropTabCreate(
            propertyCount + 1, secondArrayBytes,
            reinterpret_cast<char*>(firstArrayBytes));
    RpPrtStdEmitterClassSetPropTab(
        reinterpret_cast<int>(standardClass),
        CSF_AT(uint32_t*, secondArrayBytes, 0x400));
    RpPrtStdPropTabDestroy(
        CSF_AT(uint32_t*, secondArrayBytes, 0x400));

    std::memset(callbackBytes, 0, sizeof(callbackBytes));
    CSF_AT(uint32_t, callbackBytes, 0x08) = CSF_ADDR_FN(0x0057F050);
    CSF_AT(uint32_t, callbackBytes, 0x18) = CSF_ADDR_FN(0x0057EDA0);
    CSF_AT(uint32_t, callbackBytes, 0x0C) = CSF_ADDR_FN(0x0057F0B0);
    CSF_AT(uint32_t, callbackBytes, 0x1C) = CSF_ADDR_FN(0x004B2A30);
    CSF_AT(uint32_t, callbackBytes, 0x20) = CSF_ADDR_FN(0x004B2A30);
    CSF_AT(uint32_t, callbackBytes, 0x24) = CSF_ADDR_FN(0x004B2A30);
    CSF_AT(uint32_t, callbackBytes, 0x28) = CSF_ADDR_FN(0x004B2A30);
    CSF_AT(uint32_t, callbackBytes, 0x14) =
        CSF_ADDR_FN(0x00703910);
    CSF_AT(uint32_t, callbackBytes, 0x00) = CSF_ADDR_FN(0x006C2AB0);
    CSF_AT(uint32_t, callbackBytes, 0x04) =
        CSF_ADDR_FN(0x00704200);

    RpPrtStdEmitterClassSetCallBack(
        reinterpret_cast<int>(standardClass), 1,
        reinterpret_cast<int>(callbackBytes));

    uint32_t result = self->field_0000;
    reinterpret_cast<uint32_t*>(self->field_0000)[1] =
        reinterpret_cast<uint32_t>(standardClass);
    return result;
}

HOOK(0x006E0920, S_0041e5f0_InitializeParticleEmitterClasses, "auto");
