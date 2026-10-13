#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, AiStateFactory_ProcessFallbackTask, 0x004B9320, (int firstCandidate, int ctx));

static int __stdcall TypeRegistry_SerializeBox(int box, int ctx)
{
    void* context = reinterpret_cast<void*>(ctx);
    bool failed = false;

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, const char*, int))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, CSF_STR(0x00844380, "CBox"), ctx);
    }

    CSF_AT(uint8_t, context, 0) = failed;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, int, int))
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr, box, ctx) == -1;
    }

    bool ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, int, int))
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr, box + 12, ctx) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        AiStateFactory_ProcessFallbackTask(box + 24, ctx);

        if (CSF_AT(uint8_t, context, 0) == 0)
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            if (((bool(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, serializer, 0)[0x7C / 4])(
                        serializer, nullptr, ctx))
            {
                CSF_AT(uint8_t, context, 0) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return -1;
}

HOOK(0x0054CBF0, TypeRegistry_SerializeBox, "auto");
