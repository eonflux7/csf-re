#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall HudStatusIcons_Serialize(
    void* self, void* /*edx*/, S_00691860* context)
{
    const uint32_t savedContext = context[1].field_0004;

    bool failed = false;
    if (!context->pad_0000[0])
    {
        void* backend = reinterpret_cast<void*>(context->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, context);
    }

    context->pad_0000[0] = failed;
    context[1].field_0004 = 0;
    if (!failed)
    {
        void* backend = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        context->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, self, context) == -1;
    }

    const int memberOffsets[] = {8, 12, 4, 16, 20, 24};
    for (int offset : memberOffsets)
    {
        const bool alreadyFailed = context->pad_0000[0] != 0;
        context[1].field_0004 = 0;
        if (!alreadyFailed)
        {
            void* backend = reinterpret_cast<void*>(context->field_0004);
            CSF_AT(uint8_t, context, 0x15) = 0;
            CSF_AT(uint8_t, context, 0x16) = 0;
            context->pad_0000[0] =
                ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                    CSF_AT(void**, backend, 0)[0x54 / 4])(
                        backend, nullptr,
                        reinterpret_cast<uint8_t*>(self) + offset,
                        context) == -1;
        }
    }

    const bool stringFailed = context->pad_0000[0] != 0;
    context[1].field_0004 = 0;
    if (!stringFailed)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 28),
            context);
    }

    const bool completionFailed = context->pad_0000[0] != 0;
    context[1].field_0004 = savedContext;
    if (!completionFailed)
    {
        void* backend = reinterpret_cast<void*>(context->field_0004);
        const bool result =
            ((bool(__fastcall*)(void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x7C / 4])(
                    backend, nullptr, context);
        if (result)
        {
            context->pad_0000[0] = 0;
            return result;
        }
    }

    context->pad_0000[0] = 1;
    return 1;
}

HOOK(0x00458C60, HudStatusIcons_Serialize, "auto");
