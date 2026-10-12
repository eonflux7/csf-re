#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Vehicle_SerializeBaseValue, 0x0040D5E0, (int* a1, int a2));
CSF_FUNC(int, __fastcall, Vehicle_SerializeEmbeddedState, 0x0040F990, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, StateNode_UpdateBase, 0x0041E140, (C_0082e520* self, void* , int a2));

static bool Vehicle_BeginStateField(void* context)
{
    const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    return ready;
}

static void Vehicle_ClearStateFieldFlags(void* context)
{
    CSF_AT(uint8_t, context, 0x15) = 0;
    CSF_AT(uint8_t, context, 0x16) = 0;
}

static void Vehicle_SerializeStateByte(void* context, uint8_t* value)
{
    if (Vehicle_BeginStateField(context))
    {
        void* stream = CSF_AT(void*, context, 0x04);
        Vehicle_ClearStateFieldFlags(context);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, value, context);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }
}

static void Vehicle_SerializeStateInteger(void* context, uint32_t* value)
{
    if (Vehicle_BeginStateField(context))
    {
        void* stream = CSF_AT(void*, context, 0x04);
        Vehicle_ClearStateFieldFlags(context);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, stream, 0)[0x20 / 4])(
                    stream, nullptr, value, context);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }
}

template<class T>
static void Vehicle_SerializeStateScalar(void* context, T* value)
{
    if (Vehicle_BeginStateField(context))
    {
        void* stream = CSF_AT(void*, context, 0x04);
        Vehicle_ClearStateFieldFlags(context);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, T*, void*)>(
                CSF_AT(void**, stream, 0)[0x54 / 4])(
                    stream, nullptr, value, context);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }
}

static void Vehicle_SerializeStateReference(void* context, uint32_t* value)
{
    if (Vehicle_BeginStateField(context))
    {
        void* stream = CSF_AT(void*, context, 0x04);
        Vehicle_ClearStateFieldFlags(context);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, stream, 0)[0x38 / 4])(
                    stream, nullptr, value, context);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }
}

static int __fastcall C_0082dd78_SerializeState(
    C_0082dd78* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        failed =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, stream, 0)[0x80 / 4])(
                    stream, nullptr, 0, contextAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    StateNode_UpdateBase(self, nullptr, contextAddress);

    if (Vehicle_BeginStateField(context))
    {
        Vehicle_ClearStateFieldFlags(context);
        Vehicle_SerializeEmbeddedState(
            reinterpret_cast<uint8_t*>(self) + 0x20, nullptr, contextAddress);
    }

    if (Vehicle_BeginStateField(context))
    {
        Vehicle_ClearStateFieldFlags(context);
        Vehicle_SerializeBaseValue(
            reinterpret_cast<int*>(&self->field_0010), contextAddress);
    }

    Vehicle_SerializeStateByte(context, &self->field_0038);
    Vehicle_SerializeStateByte(context, &self->field_0039);
    Vehicle_SerializeStateInteger(context, &self->field_0034);
    Vehicle_SerializeStateScalar(context, &self->field_003c);
    Vehicle_SerializeStateScalar(context, &self->field_0040);
    Vehicle_SerializeStateScalar(context, &self->field_0044);
    Vehicle_SerializeStateScalar(context, &self->field_0048);
    Vehicle_SerializeStateScalar(context, &self->field_004c);
    Vehicle_SerializeStateScalar(context, &self->field_0050);
    Vehicle_SerializeStateScalar(context, &self->field_0054);
    Vehicle_SerializeStateScalar(context, &self->field_0058);
    Vehicle_SerializeStateReference(context, &self->field_0064);
    Vehicle_SerializeStateInteger(context, &self->field_0070);
    Vehicle_SerializeStateScalar(context, &self->field_005c);
    Vehicle_SerializeStateByte(context, &CSF_AT(uint8_t, self, 0x60));

    const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (ready)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, stream, 0)[0x7C / 4])(
                    stream, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x0040E960, C_0082dd78_SerializeState, "auto");
