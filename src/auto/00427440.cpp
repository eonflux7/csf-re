#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Serialization helpers for the two container members (+0x04 and +0x14).
CSF_FUNC(int, __stdcall, Archive_SerializeContainer04, 0x00427020, (int a1, int a2));
CSF_FUNC(int, __stdcall, Archive_SerializeContainer14, 0x00427220, (int* a1, int a2));

// Archive context layout (unknown class, accessed by offset):
//   +0x00 uint8_t  error flag
//   +0x04 void*    stream object (virtual interface)
//   +0x15 uint8_t  per-field flag
//   +0x16 uint8_t  per-field flag
//   +0x1C uint32_t current field name / tag (saved and restored around the object)

static int __fastcall C_0082fe88_Serialize(C_0082fe88* self, void* /*edx*/, void* ctx)
{
    // Open the object block on the stream.
    bool failed;
    if (CSF_AT(uint8_t, ctx, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, ctx, 0x04);
        bool opened = ((bool(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, stream, 0)[0x80 / 4])(
            stream, nullptr, 0, ctx);
        failed = !opened ? true : false;
    }
    else
    {
        failed = false;
    }

    uint32_t savedTag = CSF_AT(uint32_t, ctx, 0x1C);
    CSF_AT(uint8_t, ctx, 0x00) = failed ? 1 : 0;
    CSF_AT(uint32_t, ctx, 0x1C) = 0;

    if (!failed)
    {
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        Archive_SerializeContainer04(reinterpret_cast<int>(&self->field_0004), reinterpret_cast<int>(ctx));
    }

    bool ok = CSF_AT(uint8_t, ctx, 0x00) == 0;
    CSF_AT(uint32_t, ctx, 0x1C) = 0;
    if (ok)
    {
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        Archive_SerializeContainer14(reinterpret_cast<int*>(&self->field_0014), reinterpret_cast<int>(ctx));
    }

    ok = CSF_AT(uint8_t, ctx, 0x00) == 0;
    CSF_AT(uint32_t, ctx, 0x1C) = 0;
    if (ok)
    {
        void* stream = CSF_AT(void*, ctx, 0x04);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int status = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x58 / 4])(
            stream, nullptr, &self->field_0024, ctx);
        CSF_AT(uint8_t, ctx, 0x00) = (status == -1);
    }

    ok = CSF_AT(uint8_t, ctx, 0x00) == 0;
    CSF_AT(uint32_t, ctx, 0x1C) = 0;
    if (ok)
    {
        void* stream = CSF_AT(void*, ctx, 0x04);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int status = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x44 / 4])(
            stream, nullptr, reinterpret_cast<uint8_t*>(self) + 0x28, ctx);
        CSF_AT(uint8_t, ctx, 0x00) = (status == -1);
    }

    CSF_AT(uint32_t, ctx, 0x1C) = savedTag;

    // Close the object block.
    if (CSF_AT(uint8_t, ctx, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, ctx, 0x04);
        int closeResult = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, stream, 0)[0x7C / 4])(
            stream, nullptr, ctx);
        if (static_cast<uint8_t>(closeResult) != 0)
        {
            CSF_AT(uint8_t, ctx, 0x00) = 0;
            return closeResult;
        }
    }

    CSF_AT(uint8_t, ctx, 0x00) = 1;
    return 1;
}

HOOK(0x00427440, C_0082fe88_Serialize, "auto");
