#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, ProcessContextChildStages, 0x004E1380, (void* self, void* , int contextAddress));
CSF_FUNC(int, __stdcall, SerializeEmbeddedStage, 0x004E4C50, (int a1, int a2));

[[maybe_unused]] static const auto kReportedContextVtable = CSF_VTBL(0x004E1380);

static bool BeginArchiveStage(void* context)
{
    const bool ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    return ready;
}

static void ClearArchiveStageFlags(void* context)
{
    CSF_AT(uint8_t, context, 0x15) = 0;
    CSF_AT(uint8_t, context, 0x16) = 0;
}

static int __fastcall S_004e1380_Serialize(
    S_004e1380* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    auto* fields = reinterpret_cast<uint8_t*>(self);

    bool initialFailure = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 4);
        initialFailure =
            !((bool(__fastcall*)(void*, void*, int, int))
                CSF_AT(void**, archive, 0)[0x80 / 4])(
                    archive, nullptr, 0, contextAddress);
    }

    const uint32_t savedStage = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = initialFailure;
    ProcessContextChildStages(self, nullptr, contextAddress);

    if (BeginArchiveStage(context))
    {
        ClearArchiveStageFlags(context);
        SerializeEmbeddedStage(
            reinterpret_cast<int>(fields + 0x14), contextAddress);
    }

    if (BeginArchiveStage(context))
    {
        void* archive = CSF_AT(void*, context, 4);
        ClearArchiveStageFlags(context);
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, archive, 0)[0x48 / 4])(
                    archive, nullptr, fields + 0x48, contextAddress) == -1;
    }

    if (BeginArchiveStage(context))
    {
        void* archive = CSF_AT(void*, context, 4);
        ClearArchiveStageFlags(context);
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, fields + 0x24, contextAddress) == -1;
    }

    if (BeginArchiveStage(context))
    {
        void* archive = CSF_AT(void*, context, 4);
        ClearArchiveStageFlags(context);
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, archive, 0)[0x38 / 4])(
                    archive, nullptr, fields + 0x28, contextAddress) == -1;
    }

    if (BeginArchiveStage(context))
    {
        void* archive = CSF_AT(void*, context, 4);
        ClearArchiveStageFlags(context);
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr, fields + 0x38, contextAddress) == -1;
    }

    if (BeginArchiveStage(context))
    {
        void* archive = CSF_AT(void*, context, 4);
        ClearArchiveStageFlags(context);
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr, fields + 0x3C, contextAddress) == -1;
    }

    if (BeginArchiveStage(context))
    {
        void* archive = CSF_AT(void*, context, 4);
        ClearArchiveStageFlags(context);
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr, fields + 0x40, contextAddress) == -1;
    }

    if (BeginArchiveStage(context))
    {
        void* archive = CSF_AT(void*, context, 4);
        ClearArchiveStageFlags(context);
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, fields + 0x44, contextAddress) == -1;
    }

    if (BeginArchiveStage(context))
    {
        void* archive = CSF_AT(void*, context, 4);
        ClearArchiveStageFlags(context);
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr, fields + 0x4C, contextAddress) == -1;
    }

    if (BeginArchiveStage(context))
    {
        ClearArchiveStageFlags(context);
        Archive_SerializeObjectRef(
            fields + 0x50, reinterpret_cast<bool*>(context));
    }

    if (CSF_AT(uint8_t, context, 0x14) == 0 &&
        CSF_AT(uint8_t, context, 0) == 0)
    {
        self->field_0034 = 0;
    }

    const bool readyToFinish = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedStage;
    if (readyToFinish)
    {
        void* archive = CSF_AT(void*, context, 4);
        const int result =
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, archive, 0)[0x7C / 4])(
                    archive, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004E4F10, S_004e1380_Serialize, "auto");
