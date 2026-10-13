#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082d478_Serialize, 0x00474BD0,
         (void* self, void* edx, int a2));

static int __fastcall C_0082e0f4_Serialize(
    C_0082e0f4* self, void* /*edx*/, int a2)
{
    void* context = reinterpret_cast<void*>(a2);
    bool failed = false;
    if (!CSF_AT(uint8_t, context, 0x00))
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, serializer, 0)[128 / 4])(
                serializer, nullptr, 0, a2);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    C_0082d478_Serialize(self, nullptr, a2);

    bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, serializer, 0)[56 / 4])(
                    serializer, nullptr, &self->field_0034, a2) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, serializer, 0)[56 / 4])(
                    serializer, nullptr, &self->field_0040, a2) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                CSF_AT(void**, serializer, 0)[76 / 4])(
                    serializer, nullptr, &self->field_0064, a2) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, serializer, 0)[88 / 4])(
                    serializer, nullptr, &self->field_0060, a2) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, serializer, 0)[88 / 4])(
                    serializer, nullptr, &self->field_006c, a2) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, serializer, 0)[72 / 4])(
                    serializer, nullptr, &self->field_005c, a2) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                CSF_AT(void**, serializer, 0)[68 / 4])(
                    serializer, nullptr, &self->field_0070, a2) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, int))
                CSF_AT(void**, serializer, 0)[68 / 4])(
                    serializer, nullptr, &self->field_0071, a2) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, serializer, 0)[56 / 4])(
                    serializer, nullptr, &self->field_004c, a2) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, serializer, 0)[72 / 4])(
                    serializer, nullptr, &self->field_0068, a2) == -1;
    }

    if (!CSF_AT(uint8_t, context, 0x14))
        self->field_0058 = 0;

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const int result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, serializer, 0)[124 / 4])(
                serializer, nullptr, a2);
        if (static_cast<uint8_t>(result))
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00474D40, C_0082e0f4_Serialize, "auto");
