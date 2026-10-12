#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, MenuExtras_GetFileData, 0x00692900, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __stdcall, MpRules_ComputeFileChecksum, 0x0069DB30, (int a1, unsigned int a2));
CSF_FUNC(char, __fastcall, C_00844928_ParseTeamProperties, 0x005583A0, (void* self, void* edx, int a2, void* a3));
CSF_FUNC(char, __fastcall, C_00844928_ParseGeneralProperties, 0x00558180, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_00844928_ParseSabotageProperties, 0x0055A1B0, (void* self, void* edx, void* a5));

static char __fastcall C_00844928_LoadRulesFile(C_00844928* self, void* /*edx*/)
{
    void* allocation = Mem_Alloc(60);
    if (!allocation)
        return 0;

    // Constructor-associated reference supplied by the packet.
    (void)CSF_VTBL(0x00692B10);
    void* file = MenuExtras_InitializeFile(allocation, nullptr);
    if (!file)
        return 0;

    alignas(8) std::uint8_t dataBytes[0x4] = {};
    alignas(8) std::uint8_t sizeAndFileBytes[0x8];
    CSF_AT(void*, sizeAndFileBytes, 0x4) = file;

    const char* path = self->field_0200 < 0x10
        ? reinterpret_cast<const char*>(&self->field_01ec)
        : reinterpret_cast<const char*>(self->field_01ec);

    char result = MenuExtras_LoadFile(file, nullptr, path);
    if (result)
    {
        CSF_AT(uint32_t, sizeAndFileBytes, 0x0) = 0;
        MenuExtras_GetFileData(
            file, nullptr,
            reinterpret_cast<uint32_t*>(dataBytes),
            reinterpret_cast<uint32_t*>(sizeAndFileBytes));

        self->field_0204 = MpRules_ComputeFileChecksum(
            static_cast<int>(CSF_AT(uint32_t, dataBytes, 0x0)),
            CSF_AT(uint32_t, sizeAndFileBytes, 0x0));

        auto* root = reinterpret_cast<std::uint8_t*>(
            reinterpret_cast<S_00693740*>(file)->field_0000);
        result = root != nullptr;
        if (root)
        {
            auto* allies = FindProperty(
                root, nullptr,
                const_cast<char*>(CSF_STR(0x00844980, ".ALIADOS")));
            if (!allies)
            {
                result = 0;
            }
            else
            {
                result = C_00844928_ParseTeamProperties(self, nullptr, 0, allies);
                if (result)
                {
                    auto* germans = FindProperty(
                        root, nullptr,
                        const_cast<char*>(CSF_STR(0x00844938, ".ALEMANES")));
                    if (!germans)
                    {
                        result = 0;
                    }
                    else
                    {
                        result = C_00844928_ParseTeamProperties(self, nullptr, 1, germans);
                        if (result)
                        {
                            result = C_00844928_ParseGeneralProperties(self, nullptr, root);
                            if (result)
                                result = C_00844928_ParseSabotageProperties(self, nullptr, root);
                        }
                    }
                }
            }
        }

        file = CSF_AT(void*, sizeAndFileBytes, 0x4);
    }

    MenuExtras_CloseFile(file, nullptr);
    Mem_Free(file);
    return result;
}

HOOK(0x0055A800, C_00844928_LoadRulesFile, "auto");
