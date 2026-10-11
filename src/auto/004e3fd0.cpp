#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, SceneLoader_SerializeRecordMember, 0x00445620, (S_00413570* records, int archiveAddress));
CSF_FUNC(int, __fastcall, ProcessContextChildStages, 0x004E1380, (void* self, void* , int contextAddress));

// The packet identifies this address as a vtable reference as well as a callee.
[[maybe_unused]] static const auto kContextStagesVtable = CSF_VTBL(0x004E1380);

static bool ContextSerialization_BeginMember(void* context)
{
    const bool active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    return active;
}

static int __fastcall S_004e1380_SerializeContext(
    S_004e1380* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    bool failed = false;

    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, archive, 0)[128 / 4])(
                archive, nullptr, 0, contextAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    ProcessContextChildStages(self, nullptr, contextAddress);

    if (ContextSerialization_BeginMember(context))
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                CSF_AT(void**, archive, 0)[88 / 4])(
                    archive, nullptr, &CSF_AT(uint8_t, self, 0x14),
                    contextAddress) == -1;
    }

    if (CSF_AT(uint8_t, context, 0x14) == 0)
        self->field_001c = 0;

    if (ContextSerialization_BeginMember(context))
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                CSF_AT(void**, archive, 0)[68 / 4])(
                    archive, nullptr, &CSF_AT(uint8_t, self, 0x29),
                    contextAddress) == -1;
    }

    if (ContextSerialization_BeginMember(context))
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                CSF_AT(void**, archive, 0)[84 / 4])(
                    archive, nullptr, &CSF_AT(uint8_t, self, 0x2C),
                    contextAddress) == -1;
    }

    if (ContextSerialization_BeginMember(context))
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SceneLoader_SerializeRecordMember(
            reinterpret_cast<S_00413570*>(
                reinterpret_cast<uint8_t*>(self) + 0x30),
            contextAddress);
    }

    if (ContextSerialization_BeginMember(context))
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                CSF_AT(void**, archive, 0)[84 / 4])(
                    archive, nullptr, &CSF_AT(uint8_t, self, 0x18),
                    contextAddress) == -1;
    }

    if (ContextSerialization_BeginMember(context))
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, archive, 0)[84 / 4])(
                    archive, nullptr, &self->field_001c,
                    contextAddress) == -1;
    }

    if (ContextSerialization_BeginMember(context))
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                CSF_AT(void**, archive, 0)[84 / 4])(
                    archive, nullptr, &CSF_AT(uint8_t, self, 0x20),
                    contextAddress) == -1;
    }

    if (ContextSerialization_BeginMember(context))
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                CSF_AT(void**, archive, 0)[84 / 4])(
                    archive, nullptr, &CSF_AT(uint8_t, self, 0x24),
                    contextAddress) == -1;
    }

    if (ContextSerialization_BeginMember(context))
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, char*, int))
                CSF_AT(void**, archive, 0)[68 / 4])(
                    archive, nullptr, &CSF_AT(char, self, 0x44),
                    contextAddress) == -1;
    }

    if (ContextSerialization_BeginMember(context))
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, char*, int))
                CSF_AT(void**, archive, 0)[88 / 4])(
                    archive, nullptr, &CSF_AT(char, self, 0x40),
                    contextAddress) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;

    if (CSF_AT(uint8_t, context, 0x00) != 0)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    void* archive = CSF_AT(void*, context, 0x04);
    const int result = ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, archive, 0)[124 / 4])(
            archive, nullptr, contextAddress);

    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0x00) = 0;
    return result;
}

HOOK(0x004E3FD0, S_004e1380_SerializeContext, "auto");
