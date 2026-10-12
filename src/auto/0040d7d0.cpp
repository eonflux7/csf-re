#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Vehicle_SerializeBaseValue, 0x0040D5E0, (int* a1, int a2));
CSF_FUNC(int, __fastcall, Vehicle_SerializeEmbeddedState, 0x0040F990, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, StateNode_UpdateBase, 0x0041E140, (C_0082e520* self, void* , int a2));

static bool StateSerialization_BeginField(void* context)
{
    const bool ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
    }
    return ready;
}

static int __fastcall C_0082dc88_SerializeState(
    C_0082dc88* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    bool failed = false;

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, contextAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failed;
    StateNode_UpdateBase(self, nullptr, contextAddress);

    if (StateSerialization_BeginField(context))
    {
        Vehicle_SerializeEmbeddedState(
            reinterpret_cast<uint8_t*>(self) + 0x20,
            nullptr, contextAddress);
    }

    if (StateSerialization_BeginField(context))
    {
        Vehicle_SerializeBaseValue(
            reinterpret_cast<int*>(&self->field_0010), contextAddress);
    }

    if (StateSerialization_BeginField(context))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr, &self->field_0038, contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    if (StateSerialization_BeginField(context))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr, &self->field_0039, contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    if (StateSerialization_BeginField(context))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x20 / 4])(
                    serializer, nullptr, &self->field_0034, contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    if (StateSerialization_BeginField(context))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_003c, contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    if (StateSerialization_BeginField(context))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_0040, contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    if (StateSerialization_BeginField(context))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_0044, contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    if (StateSerialization_BeginField(context))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_0048, contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    if (StateSerialization_BeginField(context))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_004c, contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    const bool readyToFinish = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedState;

    if (readyToFinish)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const bool result =
            reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, contextAddress);
        if (result)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x0040D7D0, C_0082dc88_SerializeState, "auto");
