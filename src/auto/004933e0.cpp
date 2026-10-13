#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_Serialize_491950, 0x00491950, (C_00833908* self, void* , int contextAddress));
CSF_FUNC(int, __fastcall, C_00833b88_SerializeEmbeddedState, 0x00493310, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static bool Archive_BeginField(void* context)
{
    const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    return ready;
}

static void Archive_SetFieldFlags(void* context, uint8_t objectReference)
{
    CSF_AT(uint8_t, context, 0x15) = objectReference;
    CSF_AT(uint8_t, context, 0x16) = 0;
}

static int __fastcall C_00833b88_Serialize(
    C_00833b88* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, contextAddress);
    }

    const uint32_t savedFieldState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    C_00833908_Serialize_491950(self, nullptr, contextAddress);

    if (Archive_BeginField(context))
    {
        Archive_SetFieldFlags(context, 1);
        Archive_SerializeObjectRef(
            &self->field_0204, reinterpret_cast<bool*>(context));
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x38 / 4])(
                    backend, nullptr, &self->field_018c, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x38 / 4])(
                    backend, nullptr, &self->field_0198, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, &CSF_AT(float, self, 0x1A4),
                    contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, &CSF_AT(float, self, 0x1A8),
                    contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x1C / 4])(
                    backend, nullptr, &self->field_01ac, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        Archive_SetFieldFlags(context, 0);
        Archive_SerializeString(
            reinterpret_cast<int>(&CSF_AT(uint8_t, self, 0x1B4)),
            reinterpret_cast<S_00691860*>(context));
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x48 / 4])(
                    backend, nullptr, &self->field_01c0, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, &self->field_01d4, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_01d8, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_01c4, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_01dc, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_01e0, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        Archive_SetFieldFlags(context, 0);
        C_00833b88_SerializeEmbeddedState(
            &self->field_01e4, nullptr, contextAddress);
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x20 / 4])(
                    backend, nullptr, &self->field_01f0, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x48 / 4])(
                    backend, nullptr, &self->field_01f4, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x48 / 4])(
                    backend, nullptr, &self->field_01f8, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, &self->field_01fc, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x54 / 4])(
                    backend, nullptr, &self->field_0200, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, backend, 0)[0x38 / 4])(
                    backend, nullptr, &self->field_01c8, contextAddress) == -1;
    }

    if (Archive_BeginField(context))
    {
        void* backend = CSF_AT(void*, context, 0x04);
        Archive_SetFieldFlags(context, 0);
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint16_t*, int)>(
                CSF_AT(void**, backend, 0)[0x50 / 4])(
                    backend, nullptr, &self->field_0208, contextAddress) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedFieldState;
    if (CSF_AT(uint8_t, context, 0x00) != 0)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    void* backend = CSF_AT(void*, context, 0x04);
    // Only the low byte controls success, but the full result is returned.
    const int finishResult =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, backend, 0)[0x7C / 4])(
                backend, nullptr, contextAddress);
    if (static_cast<uint8_t>(finishResult) == 0)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0x00) = 0;
    return finishResult;
}

HOOK(0x004933E0, C_00833b88_Serialize, "auto");
