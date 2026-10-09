#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(int, __cdecl, WeaponDefs_ReleaseRecords, 0x00783670, (int a1, int a2, int a3, int a4));
CSF_FUNC(char, __fastcall, WeaponDefs_ParseV1To3, 0x007849E0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, WeaponDefs_ParseV4, 0x00785090, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, WeaponDefs_ParseV5, 0x007857C0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, WeaponDefs_ParseV6, 0x00785F80, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, WeaponDefs_ParseV7, 0x00786760, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, WeaponDefs_ParseV8, 0x00786F60, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, WeaponDefs_ParseV9, 0x00787880, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, WeaponDefs_ParseV10, 0x007881C0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, WeaponDefs_ParseV11, 0x00788C20, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, WeaponDefs_ParseV12, 0x00789850, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, WeaponDefs_ParseV13, 0x0078A4F0, (void* self, void* edx, int a2, void* a3, char* String1));

CSF_GLOBAL(char*, g_pszKey_VERSION_8A9C70, 0x008A9C70);

static char __fastcall CWeaponDefTable_LoadFile(CWeaponDefTable* self, void* /*edx*/, int* table, int fileArg, char* String1)
{
    char result = 0;

    if (table[1]) {
        WeaponDefs_ReleaseRecords(table[1], table[2], (int)table, (int)table);
        if (table[1])
            Mem_FreeDebug((void*)table[1]);
    }
    table[1] = 0;
    table[2] = 0;
    table[3] = 0;

    std::uint8_t file[0x3C];
    MenuExtras_InitializeFile(file, nullptr);

    if (MenuExtras_LoadFile(file, nullptr, (const char*)fileArg)) {
        std::uint8_t* node = CSF_AT(std::uint8_t*, file, 0);
        int version = ReadIntegerAttribute(node, nullptr, g_pszKey_VERSION_8A9C70);
        if (version <= 13) {
            switch (version) {
            case 1:
            case 2:
            case 3:
                result = WeaponDefs_ParseV1To3(self, nullptr, node);
                break;
            case 4:
                result = WeaponDefs_ParseV4(self, nullptr, node);
                break;
            case 5:
                result = WeaponDefs_ParseV5(self, nullptr, node);
                break;
            case 6:
                result = WeaponDefs_ParseV6(self, nullptr, node);
                break;
            case 7:
                result = WeaponDefs_ParseV7(self, nullptr, node);
                break;
            case 8:
                result = WeaponDefs_ParseV8(self, nullptr, node);
                break;
            case 9:
                result = WeaponDefs_ParseV9(self, nullptr, node);
                break;
            case 10:
                result = WeaponDefs_ParseV10(self, nullptr, node);
                break;
            case 11:
                result = WeaponDefs_ParseV11(self, nullptr, node);
                break;
            case 12:
                result = WeaponDefs_ParseV12(self, nullptr, node);
                break;
            case 13:
                result = WeaponDefs_ParseV13(self, nullptr, (int)table, node, String1);
                break;
            default:
                break;
            }
            MenuExtras_CloseFile(file, nullptr);
            return result;
        }
    }
    MenuExtras_CloseFile(file, nullptr);
    return 0;
}

HOOK(0x0078B370, CWeaponDefTable_LoadFile, "auto");
