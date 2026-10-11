#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char __fastcall C_00833908_GetReturnedVector(
    C_00833908* self, void* /*edx*/, uint32_t* output, int request)
{
    auto* dispatcher = reinterpret_cast<C_0082dd78*>(self->field_004c);
    char result = 0;
    if (!dispatcher)
        return result;

    auto* object = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(dispatcher, nullptr));
    if (object &&
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x34 / 4])(object, nullptr))
    {
        uint32_t response[5];
        response[0] = 0;
        response[1] = 0;
        response[2] = 0;
        response[3] = 10;
        CSF_AT(uint8_t, response, 0x10) = 0;

        result = reinterpret_cast<char(__fastcall*)(void*, void*, int, uint32_t*, int)>(
            CSF_AT(void**, object, 0)[0x40 / 4])(
                object, nullptr, request, response, 0);

        auto* vector = reinterpret_cast<uint32_t*>(response[0]);
        if (result)
        {
            output[0] = vector[0];
            output[1] = vector[1];
            output[2] = vector[2];
        }
        if (vector)
            Mem_Free(vector);
    }
    return result;
}

HOOK(0x004908A0, C_00833908_GetReturnedVector, "auto");
