#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
         (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, sub_46C260, 0x0046C260,
         (void* self, void* edx, char* String1));
CSF_FUNC(void*, __fastcall, sub_46E380, 0x0046E380,
         (void* self, void* edx, void* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, sub_761F80, 0x00761F80, (char* String1));

static int __fastcall S_0046c260_ResolveScriptIdentifier(
    S_0046c260* self, void* /*edx*/, char* name)
{
    // Retain the packet's required binding; the instruction is a direct call.
    (void)CSF_VTBL(0x0046C260);

    const int predefinedId = sub_761F80(name);
    if (predefinedId != -1)
        return predefinedId;

    int localId = sub_46C260(self, nullptr, name);
    if (localId == -1)
    {
        localId = 0;

        alignas(4) std::uint8_t stringStorage[0x1C];
        auto* temporaryString =
            reinterpret_cast<C_0082d410*>(stringStorage);

        CSF_AT(std::uint32_t, stringStorage, 0x18) = 15;
        CSF_AT(std::uint32_t, stringStorage, 0x14) = 0;
        *reinterpret_cast<std::uint8_t*>(&temporaryString->field_0004) = 0;

        AssignStringRange_429B60(
            temporaryString, nullptr, name,
            static_cast<unsigned int>(std::strlen(name)));

        const std::uint32_t begin = self->field_0018;
        if (begin != 0)
        {
            localId = static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x1C) - begin) / 28;
        }

        sub_46E380(
            reinterpret_cast<std::uint8_t*>(self) + 0x14,
            nullptr, temporaryString);

        if (CSF_AT(std::uint32_t, stringStorage, 0x18) >= 0x10)
        {
            if (temporaryString->field_0004 != 0)
            {
                Mem_FreeDebug(reinterpret_cast<void*>(
                    temporaryString->field_0004));
            }
        }
    }

    return localId | 0x8000000;
}

HOOK(0x0046E4E0, S_0046c260_ResolveScriptIdentifier, "auto");
