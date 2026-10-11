#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_008621c8_Initialize, 0x006EF6A0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, C_008621c8_SetNamedParameters, 0x006EF970,
         (void* self, void* edx, char* String2, int a3, int a4, char a5));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_00860bb8_SetNamedParameters(
    C_00860bb8* self, void* /*edx*/, char* name, int value,
    float firstOptional, float secondOptional, char flag)
{
    if (!name)
        return;

    const char* end = name;
    while (*end)
        ++end;
    if (end == name)
        return;

    if (!self->field_0064)
    {
        void* storage = Mem_Alloc(68);
        self->field_0064 = storage
            ? static_cast<std::uint32_t>(C_008621c8_Initialize(storage, nullptr))
            : 0;
    }

    alignas(8) std::uint8_t parameterBytes[0xC];
    CSF_AT(int, parameterBytes, 0x0) = value;
    CSF_AT(float, parameterBytes, 0x4) = 0.0f;
    CSF_AT(float, parameterBytes, 0x8) = 0.0f;

    int count = 1;
    if (firstOptional > g_fZero)
    {
        CSF_AT(float, parameterBytes, 0x4) = firstOptional;
        count = 2;
    }
    if (secondOptional > g_fZero)
    {
        ++count;
        CSF_AT(float, parameterBytes, (count - 1) * 4) = secondOptional;
    }

    C_008621c8_SetNamedParameters(
        reinterpret_cast<void*>(self->field_0064), nullptr, name, count,
        static_cast<int>(reinterpret_cast<std::intptr_t>(parameterBytes)), flag);
}

HOOK(0x006CAE30, C_00860bb8_SetNamedParameters, "auto");
