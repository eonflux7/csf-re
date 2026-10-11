#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, ProcessContextChildStages, 0x004E1380, (void* self, void* , int contextAddress));

static int __fastcall S_004e1380_Serialize(
    S_004e1380* self, void* /*edx*/, int contextAddress)
{
    auto* context = reinterpret_cast<uint8_t*>(contextAddress);
    auto* fields = reinterpret_cast<uint8_t*>(self);

    // The packet also identifies the child-stage entry as a vtable reference.
    (void)CSF_VTBL(0x004E1380);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, contextAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failed;
    ProcessContextChildStages(self, nullptr, contextAddress);

    bool processField = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, archive, 0)[0x58 / 4])(
                    archive, nullptr, fields + 0x14, contextAddress) == -1;
    }

    processField = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, fields + 0x18, contextAddress) == -1;
    }

    processField = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(&self->field_001c),
            reinterpret_cast<S_00691860*>(context));
    }

    processField = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr, fields + 0x28, contextAddress) == -1;
    }

    processField = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, fields + 0x2C, contextAddress) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        // AL controls success, but the original returns the entire EAX.
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
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

HOOK(0x004E2560, S_004e1380_Serialize, "auto");
