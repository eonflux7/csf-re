#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, RpUserDataArrayGetName, 0x005870B0, (int a1));
CSF_FUNC(int, __cdecl, RpUserDataArrayGetInt, 0x0058BE50, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, GfxObjectProps_ResolveDummyName, 0x006BA370, (void* self, void* edx, void* a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_DummyPluginOffset, 0x008BB474);

static bool GfxObjectProps_MatchesPropertyName(
    const char* name, const char* expected, unsigned int byteCount)
{
    for (unsigned int i = 0; i < byteCount; ++i)
    {
        if (name[i] != expected[i])
            return false;
    }
    return true;
}

static char __cdecl GfxObjectProps_ReadDummyUserData(
    void* properties, int userDataArray, int /*a3*/, int /*a4*/)
{
    const char* name =
        reinterpret_cast<const char*>(RpUserDataArrayGetName(userDataArray));

    if (GfxObjectProps_MatchesPropertyName(
            name, CSF_STR(0x0086053C, "nombre_Dummy"), 13))
    {
        char* text =
            reinterpret_cast<char*>(RpUserDataArrayGetInt(userDataArray, 0));

        alignas(8) uint8_t stringBytes[0x1C];
        C_0082d410& temporaryString =
            *reinterpret_cast<C_0082d410*>(stringBytes);

        CSF_AT(unsigned int, &temporaryString, 0x18) = 15;
        CSF_AT(int, &temporaryString, 0x14) = 0;
        reinterpret_cast<uint8_t*>(&temporaryString.field_0004)[0] = 0;

        const char* end = text;
        while (*end != '\0')
            ++end;

        AssignStringRange_429B60(
            &temporaryString, nullptr, text,
            static_cast<unsigned int>(end - text));

        void* resolver =
            CSF_AT(void*, g_pRwEngineInstance, g_DummyPluginOffset);
        CSF_AT(void*, properties, 0) =
            GfxObjectProps_ResolveDummyName(resolver, nullptr, &temporaryString);

        if (CSF_AT(unsigned int, &temporaryString, 0x18) >= 0x10)
        {
            if (temporaryString.field_0004)
            {
                Mem_FreeDebug(
                    reinterpret_cast<void*>(temporaryString.field_0004));
            }
        }
        return 1;
    }

    if (GfxObjectProps_MatchesPropertyName(
            name, CSF_STR(0x00860530, "id_Dummy"), 9))
    {
        CSF_AT(int, properties, 0x4) =
            RpUserDataArrayGetInt(userDataArray, 0);
        return 1;
    }

    return 0;
}

HOOK(0x006BAEB0, GfxObjectProps_ReadDummyUserData, "auto");
