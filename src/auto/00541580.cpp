#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, SceneLoader_CopyMapFilename, 0x006948E0, (char* a1, char* a2, unsigned int a3));
CSF_FUNC(int, __cdecl, GameBddLoader_ShouldPreloadResources, 0x0069D5C0, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __cdecl MenuMpStats_ResolveVideoPath(C_0082d410* self)
{
    alignas(8) uint8_t filenameBytes[0x200];
    char* filename = reinterpret_cast<char*>(filenameBytes);

    if (CompareStringSubstring(
            self, nullptr, 0, CSF_AT(unsigned int, self, 0x14),
            reinterpret_cast<const char*>(&g_byte_82E65C), 0))
    {
        if (static_cast<uint8_t>(GameBddLoader_ShouldPreloadResources(0)))
        {
            void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
            void* settings = LookupObjectById(globalObject, nullptr);

            if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, settings, 0)[0x58 / 4])(settings, nullptr))
            {
                char* source;
                if (CSF_AT(uint32_t, self, 0x18) < 0x10)
                    source = reinterpret_cast<char*>(&self->field_0004);
                else
                    source = reinterpret_cast<char*>(self->field_0004);

                SceneLoader_CopyMapFilename(source, filename, 512);

                int ntscResult = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, settings, 0)[0x5C / 4])(settings, nullptr);

                const char* prefix = CSF_STR(0x008431B8, "Videos/NTSC/");
                if (!static_cast<uint8_t>(ntscResult))
                    prefix = CSF_STR(0x008431B0, "Videos/");

                AssignStringRange_429B60(
                    self, nullptr, const_cast<char*>(prefix),
                    static_cast<unsigned int>(std::strlen(prefix)));

                unsigned int filenameLength = 0;
                while (filenameLength < 512 && filename[filenameLength] != '\0')
                    ++filenameLength;

                Profiles_AppendStringRange(self, 0, filename, filenameLength);
            }
        }
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x00541580, MenuMpStats_ResolveVideoPath, "auto");
