#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall CharacterModel_Serialize(void* self, void* /*edx*/,
                                               S_00691860* context)
{
    auto* model = static_cast<uint8_t*>(self);

    bool failed = false;
    if (!context->pad_0000[0])
    {
        void* backend = reinterpret_cast<void*>(context->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
                       CSF_AT(void**, backend, 0)[0x80 / 4])(
                           backend, nullptr, 0, context);
    }

    const uint32_t savedState = context[1].field_0004;
    context->pad_0000[0] = failed;
    context[1].field_0004 = 0;

    if (!failed)
    {
        void* backend = reinterpret_cast<void*>(context->field_0004);
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        context->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                 CSF_AT(void**, backend, 0)[0x48 / 4])(
                     backend, nullptr, self, context) == -1;
    }

    bool ready = context->pad_0000[0] == 0;
    context[1].field_0004 = 0;
    if (ready)
    {
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        Archive_SerializeString(reinterpret_cast<int>(model + 4), context);
    }

    ready = context->pad_0000[0] == 0;
    context[1].field_0004 = 0;
    if (ready)
    {
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        Archive_SerializeString(reinterpret_cast<int>(model + 16), context);
    }

    ready = context->pad_0000[0] == 0;
    context[1].field_0004 = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(context->field_0004);
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        context->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                 CSF_AT(void**, backend, 0)[0x54 / 4])(
                     backend, nullptr, model + 28, context) == -1;
    }

    ready = context->pad_0000[0] == 0;
    context[1].field_0004 = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(context->field_0004);
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        context->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                 CSF_AT(void**, backend, 0)[0x38 / 4])(
                     backend, nullptr, model + 32, context) == -1;
    }

    ready = context->pad_0000[0] == 0;
    context[1].field_0004 = 0;
    if (ready)
    {
        void* backend = reinterpret_cast<void*>(context->field_0004);
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        context->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                 CSF_AT(void**, backend, 0)[0x54 / 4])(
                     backend, nullptr, model + 44, context) == -1;
    }

    context[1].field_0004 = savedState;
    if (!context->pad_0000[0])
    {
        void* backend = reinterpret_cast<void*>(context->field_0004);
        const int result =
            ((int(__fastcall*)(void*, void*, S_00691860*))
                 CSF_AT(void**, backend, 0)[0x7C / 4])(
                     backend, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            context->pad_0000[0] = 0;
            return result;
        }
    }

    context->pad_0000[0] = 1;
    return 1;
}

HOOK(0x004F8320, CharacterModel_Serialize, "auto");
