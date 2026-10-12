#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall MenuScenarioSelect_GetScenarioMask(
    void* self, void* /*edx*/, char* text)
{
    alignas(8) uint8_t stringBytes[0x20];
    C_0082d410& temporaryString =
        *reinterpret_cast<C_0082d410*>(stringBytes);

    CSF_AT(uint32_t, &temporaryString, 0x18) = 15;
    CSF_AT(uint32_t, &temporaryString, 0x14) = 0;
    *reinterpret_cast<uint8_t*>(&temporaryString.field_0004) = 0;

    AssignStringRange_429B60(
        &temporaryString, nullptr, text,
        static_cast<unsigned int>(std::strlen(text)));

    int result = 0;
    if (__stricmp(text, reinterpret_cast<const char*>(&g_byte_82E65C)) != 0)
    {
        unsigned int entryOffset = 0;
        unsigned int scenarioIndex = 0;
        do
        {
            uint8_t* entry = CSF_AT(uint8_t*, self, 0x08) + entryOffset;
            const char* entryText =
                CSF_AT(uint32_t, entry, 0x1C) < 0x10
                    ? reinterpret_cast<const char*>(entry + 0x08)
                    : CSF_AT(const char*, entry, 0x08);

            if (__stricmp(entryText, text) == 0)
            {
                result = 1 << scenarioIndex;
                break;
            }

            entryOffset += 0x20;
            ++scenarioIndex;
        } while (entryOffset < 0x140);
    }

    if (CSF_AT(uint32_t, &temporaryString, 0x18) >= 0x10)
    {
        if (temporaryString.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(temporaryString.field_0004));
    }

    return result;
}

HOOK(0x0069BC50, MenuScenarioSelect_GetScenarioMask, "auto");
