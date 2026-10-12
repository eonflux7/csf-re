#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, SerializeMemberFields, 0x00449C60,
         (uint32_t* a1, void* a2));
CSF_FUNC(int, __fastcall, SerializeBaseFields, 0x0044A160,
         (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __stdcall, SerializeEmbeddedFields, 0x0044A9D0,
         (void* a1, int a2));

static int __fastcall C_0083114c_Serialize(
    C_0083114c* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, contextAddress);
    }

    uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SerializeBaseFields(&self->field_0004, nullptr, contextAddress);
    }

    bool continueProcessing = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SerializeEmbeddedFields(
            reinterpret_cast<uint8_t*>(self) + 0x14, contextAddress);
    }

    continueProcessing = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr, &self->field_004c, contextAddress) == -1;
    }

    continueProcessing = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_0058, contextAddress) == -1;
    }

    continueProcessing = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_005c, contextAddress) == -1;
    }

    continueProcessing = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, float*, int)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_0060, contextAddress) == -1;
    }

    continueProcessing = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SerializeMemberFields(&CSF_AT(uint32_t, self, 0x3C), context);
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        int result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x0044ABA0, C_0083114c_Serialize, "auto");
