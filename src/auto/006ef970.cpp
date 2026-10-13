#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_006d4450_FindOrAddTexture, 0x006D5560, (S_006d4450* self, void* , char* name, int));

static unsigned int __fastcall C_008621c8_SetNamedParameters(
    C_008621c8* self, void* /*edx*/, char* name,
    int parameterCount, const uint32_t* parameters, char flag)
{
    unsigned int nameLength = 0;
    while (name[nameLength] != '\0')
        ++nameLength;

    AssignStringRange_429B60(
        reinterpret_cast<uint8_t*>(self) + 4, nullptr, name, nameLength);

    self->field_0020 = parameterCount;
    self->field_003d = flag;

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(global, nullptr));
    auto* provider = static_cast<S_006d4450*>(
        GetCachedObjectProvider(accessor, nullptr));
    S_006d4450_FindOrAddTexture(provider, nullptr, name, 1);

    unsigned int copied = 0;
    if (parameterCount >= 4)
    {
        unsigned int groups =
            (static_cast<unsigned int>(parameterCount - 4) >> 2) + 1;
        copied = 4 * groups;

        auto* destination = reinterpret_cast<uint8_t*>(self) + 0x30;
        auto* source = reinterpret_cast<const uint8_t*>(parameters);
        do
        {
            CSF_AT(uint32_t, destination, 0) = CSF_AT(uint32_t, source, 0);
            CSF_AT(uint32_t, destination, 4) = CSF_AT(uint32_t, source, 4);
            CSF_AT(uint32_t, destination, 8) = CSF_AT(uint32_t, source, 8);
            CSF_AT(uint32_t, destination, 12) = CSF_AT(uint32_t, source, 12);
            destination += 0x10;
            source += 0x10;
        }
        while (--groups);
    }

    if (copied < static_cast<unsigned int>(parameterCount))
    {
        auto* destination =
            reinterpret_cast<uint8_t*>(self) + 0x30 + 4 * copied;
        do
        {
            CSF_AT(uint32_t, destination, 0) = parameters[copied];
            ++copied;
            destination += 4;
        }
        while (copied < static_cast<unsigned int>(parameterCount));
    }

    return copied;
}

HOOK(0x006EF970, C_008621c8_SetNamedParameters, "auto");
