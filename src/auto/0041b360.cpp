#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Vehicle_SerializeBaseValue, 0x0040D5E0,
         (int* a1, int a2));
CSF_FUNC(int, __fastcall, Vehicle_SerializeEmbeddedState, 0x0040F990,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_SerializeBaseState, 0x0040FA60,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, Vehicle_SerializeWheelState, 0x0041AF40,
         (uint32_t* a1, int a2));
CSF_FUNC(int, __stdcall, Vehicle_SerializeAuxiliaryState, 0x0041B160,
         (uint32_t* a1, int a2));

static bool Vehicle_BeginField(void* context)
{
    const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
    }
    return ready;
}

static void Vehicle_SerializeFloat(float* value, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    if (Vehicle_BeginField(context))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x54 / 4])(
                serializer, nullptr, value, contextAddress) == -1;
    }
}

static void Vehicle_SerializeByte(uint8_t* value, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    if (Vehicle_BeginField(context))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x44 / 4])(
                serializer, nullptr, value, contextAddress) == -1;
    }
}

static int __fastcall C_0082e1e8_Serialize(
    C_0082e1e8* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, serializer, 0x00)[0x80 / 4])(
            serializer, nullptr, 0, contextAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    Vehicle_SerializeBaseState(self, nullptr, contextAddress);

    if (Vehicle_BeginField(context))
    {
        Vehicle_SerializeEmbeddedState(
            reinterpret_cast<uint8_t*>(self) + 0x20, nullptr, contextAddress);
    }
    if (Vehicle_BeginField(context))
    {
        Vehicle_SerializeWheelState(&self->field_0034, contextAddress);
    }
    if (Vehicle_BeginField(context))
    {
        Vehicle_SerializeWheelState(&self->field_0044, contextAddress);
    }
    if (Vehicle_BeginField(context))
    {
        Vehicle_SerializeAuxiliaryState(&self->field_0054, contextAddress);
    }
    if (Vehicle_BeginField(context))
    {
        Vehicle_SerializeBaseValue(
            reinterpret_cast<int*>(&self->field_0010), contextAddress);
    }

    Vehicle_SerializeFloat(&self->field_0064, contextAddress);
    Vehicle_SerializeFloat(&self->field_0068, contextAddress);
    Vehicle_SerializeFloat(&self->field_006c, contextAddress);
    Vehicle_SerializeFloat(&self->field_0070, contextAddress);
    Vehicle_SerializeFloat(&self->field_0074, contextAddress);
    Vehicle_SerializeFloat(&self->field_0078, contextAddress);
    Vehicle_SerializeByte(&self->field_0084, contextAddress);
    Vehicle_SerializeByte(&self->field_0085, contextAddress);

    if (Vehicle_BeginField(context))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x20 / 4])(
                serializer, nullptr, &self->field_007c, contextAddress) == -1;
    }

    Vehicle_SerializeByte(&CSF_AT(uint8_t, self, 0x86), contextAddress);

    const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x7C / 4])(
                serializer, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x0041B360, C_0082e1e8_Serialize, "auto");
