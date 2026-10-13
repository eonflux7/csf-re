#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_Serialize_491950, 0x00491950, (C_00833908* self, void* , int contextAddress));
CSF_FUNC(unsigned int, __stdcall, Archive_SerializeRecordArray, 0x0049F710, (uint32_t* a1, char* a2));

static int __fastcall C_008342e8_Serialize(
    C_008342e8* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    bool error = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = reinterpret_cast<void*>(
            CSF_AT(uint32_t, context, 0x04));
        error = !((bool(__fastcall*)(void*, void*, int, int))
                      CSF_AT(void**, archive, 0)[0x80 / 4])(
            archive, nullptr, 0, contextAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = error;
    C_00833908_Serialize_491950(self, nullptr, contextAddress);

    bool ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* archive = reinterpret_cast<void*>(
            CSF_AT(uint32_t, context, 0x04));
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                 CSF_AT(void**, archive, 0)[0x54 / 4])(
                archive, nullptr, &CSF_AT(uint8_t, self, 0x184),
                contextAddress) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* archive = reinterpret_cast<void*>(
            CSF_AT(uint32_t, context, 0x04));
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                 CSF_AT(void**, archive, 0)[0x54 / 4])(
                archive, nullptr, &CSF_AT(uint8_t, self, 0x188),
                contextAddress) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* archive = reinterpret_cast<void*>(
            CSF_AT(uint32_t, context, 0x04));
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                 CSF_AT(void**, archive, 0)[0x54 / 4])(
                archive, nullptr, &CSF_AT(uint8_t, self, 0x18C),
                contextAddress) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* archive = reinterpret_cast<void*>(
            CSF_AT(uint32_t, context, 0x04));
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                 CSF_AT(void**, archive, 0)[0x54 / 4])(
                archive, nullptr, &CSF_AT(uint8_t, self, 0x190),
                contextAddress) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* archive = reinterpret_cast<void*>(
            CSF_AT(uint32_t, context, 0x04));
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                 CSF_AT(void**, archive, 0)[0x54 / 4])(
                archive, nullptr, &CSF_AT(uint8_t, self, 0x194),
                contextAddress) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Archive_SerializeRecordArray(
            &self->field_0198, reinterpret_cast<char*>(context));
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* archive = reinterpret_cast<void*>(
            CSF_AT(uint32_t, context, 0x04));
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                 CSF_AT(void**, archive, 0)[0x1C / 4])(
                archive, nullptr, &CSF_AT(uint32_t, self, 0x1A8),
                contextAddress) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = reinterpret_cast<void*>(
            CSF_AT(uint32_t, context, 0x04));
        // The original tests AL but forwards the complete EAX on success.
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

HOOK(0x0049F8E0, C_008342e8_Serialize, "auto");
