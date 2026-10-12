#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_Serialize_491950, 0x00491950, (C_00833908* self, void* , int contextAddress));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall C_00834050_Serialize(
    C_00834050* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);

    bool initialFailure = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* backend = CSF_AT(void*, context, 0x04);
        initialFailure =
            !((bool(__fastcall*)(void*, void*, int, int))
                  CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, contextAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = initialFailure;
    C_00833908_Serialize_491950(self, nullptr, contextAddress);

    bool canSerialize = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(&self->field_0188),
            reinterpret_cast<S_00691860*>(context));
    }

    canSerialize = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                 CSF_AT(void**, backend, 0)[0x58 / 4])(
                backend, nullptr, &self->field_0194, contextAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                 CSF_AT(void**, backend, 0)[0x44 / 4])(
                backend, nullptr, &self->field_0198, contextAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                 CSF_AT(void**, backend, 0)[0x44 / 4])(
                backend, nullptr, &self->field_0184, contextAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                 CSF_AT(void**, backend, 0)[0x44 / 4])(
                backend, nullptr, &self->field_0199, contextAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                 CSF_AT(void**, backend, 0)[0x38 / 4])(
                backend, nullptr, &self->field_019c, contextAddress) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(&self->field_01a8),
            reinterpret_cast<S_00691860*>(context));
    }

    canSerialize = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* backend = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                 CSF_AT(void**, backend, 0)[0x58 / 4])(
                backend, nullptr, &CSF_AT(uint32_t, self, 0x1B4),
                contextAddress) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* backend = CSF_AT(void*, context, 0x04);
        // The original tests AL but forwards the full EAX on success.
        const int result =
            ((int(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, backend, 0)[0x7C / 4])(
                backend, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x0049DFA0, C_00834050_Serialize, "auto");
