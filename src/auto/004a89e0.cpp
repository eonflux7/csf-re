#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

static int __fastcall S_0041f0a0_SerializeWithHandlers(S_0041f0a0* self, void* /*edx*/, S_0041f0a0* a2)
{
    const int savedContext = CSF_AT(int, a2, 0x1C);

    // First handler: vtable slot 0x80 called with (handler, nullptr, 0, a2).
    uint8_t handlerFlag = 0;
    if (CSF_AT(uint8_t, a2, 0) == 0)
    {
        void* handler = CSF_AT(void*, a2, 0x04);
        const bool blocked =
            reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
                CSF_AT(void**, handler, 0)[0x80 / 4])(handler, nullptr, 0, a2);
        handlerFlag = blocked ? 0 : 1;
    }
    CSF_AT(uint8_t, a2, 0) = handlerFlag;

    Archive_SerializeBaseFields(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(a2)));

    CSF_AT(int, a2, 0x1C) = 0;
    if (CSF_AT(uint8_t, a2, 0) == 0)
    {
        void* obj = CSF_AT(void*, a2, 0x04);
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        const int status =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, obj, 0)[0x58 / 4])(
                    obj, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x1C,
                    a2);
        CSF_AT(uint8_t, a2, 0) = status == -1;
    }

    CSF_AT(int, a2, 0x1C) = 0;
    if (CSF_AT(uint8_t, a2, 0) == 0)
    {
        void* obj = CSF_AT(void*, a2, 0x04);
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        const int status =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, obj, 0)[0x38 / 4])(
                    obj, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x10,
                    a2);
        CSF_AT(uint8_t, a2, 0) = status == -1;
    }

    CSF_AT(int, a2, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, a2, 0) == 0)
    {
        void* obj = CSF_AT(void*, a2, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, obj, 0)[0x7C / 4])(obj, nullptr, a2);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, a2, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, a2, 0) = 1;
    return 1;
}

HOOK(0x004A89E0, S_0041f0a0_SerializeWithHandlers, "auto");
