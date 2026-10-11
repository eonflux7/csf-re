#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_GetField016c, 0x0048E230, (C_00833908* self, void*));

CSF_GLOBAL(float, g_GravityScale, 0x0082E070);
CSF_GLOBAL(float, g_WheelTravelMultiplier, 0x0082E24C);

static char __fastcall C_0083e5e8_ScaleLinkedVectorsOnce(
    C_0083e5e8* self, void* /*edx*/)
{
    const char applied = CSF_AT(char, self, 0x30);
    if (applied)
        return applied;

    alignas(8) uint8_t vectorBytes[0xC];
    auto* owner = reinterpret_cast<C_00833908*>(self->field_000c);

    void* firstEntry =
        reinterpret_cast<void* (__fastcall*)(C_00833908*, void*)>(
            CSF_AT(void**, owner, 0)[0xA0 / 4])(owner, nullptr);

    void* firstVectorObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, firstEntry, 0)[0x70 / 4])(firstEntry, nullptr);

    if (firstVectorObject)
    {
        void* source =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, firstVectorObject, 0)[0x30 / 4])(
                    firstVectorObject, nullptr);

        CSF_AT(uint32_t, vectorBytes, 0) = CSF_AT(uint32_t, source, 0);
        CSF_AT(float, vectorBytes, 4) = CSF_AT(float, source, 4);
        CSF_AT(float, vectorBytes, 8) = CSF_AT(float, source, 8);
        CSF_AT(float, vectorBytes, 4) *= g_GravityScale;
        CSF_AT(float, vectorBytes, 8) *= g_WheelTravelMultiplier;

        reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, firstVectorObject, 0)[0x2C / 4])(
                firstVectorObject, nullptr, vectorBytes);
    }

    void* secondEntry =
        reinterpret_cast<void*>(C_00833908_GetField016c(owner, nullptr));
    void* secondVectorObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, secondEntry, 0)[0x70 / 4])(secondEntry, nullptr);

    char result = 0;
    if (secondVectorObject)
    {
        void* source =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, secondVectorObject, 0)[0x30 / 4])(
                    secondVectorObject, nullptr);

        CSF_AT(uint32_t, vectorBytes, 0) = CSF_AT(uint32_t, source, 0);
        CSF_AT(float, vectorBytes, 4) = CSF_AT(float, source, 4);
        CSF_AT(float, vectorBytes, 8) = CSF_AT(float, source, 8);
        CSF_AT(float, vectorBytes, 4) *= g_GravityScale;
        CSF_AT(float, vectorBytes, 8) *= g_WheelTravelMultiplier;

        result = static_cast<char>(
            reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, secondVectorObject, 0)[0x2C / 4])(
                    secondVectorObject, nullptr, vectorBytes));
    }

    CSF_AT(char, self, 0x30) = 1;
    return result;
}

HOOK(0x004E1D70, C_0083e5e8_ScaleLinkedVectorsOnce, "auto");
