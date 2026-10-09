#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(void, __fastcall, Hud_ReadFloatPair, 0x00693A20, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseWeaponType, 0x00761530, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAutomaticMode, 0x007615D0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseLoadingMode, 0x00761620, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseRangeType, 0x00761670, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseHand, 0x007616C0, (char* String1));
CSF_FUNC(int, __cdecl, WeaponDefs_ParseAnimationType, 0x007617B0, (char* String1));
CSF_FUNC(int, __cdecl, ParsePlayerAnimTypeName, 0x007618A0, (char* String1));
CSF_FUNC(void, __fastcall, AnimRefList_Add, 0x00771FB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __stdcall, WeaponDefs_FinalizeEntry, 0x00783430, (int a1));
CSF_FUNC(int, __fastcall, WeaponDefs_InitializeDefaults, 0x00783EA0, (int a1, int a2));

// Explicitly expose the 552-byte by-value record as 138 stack dwords.
// Together with count, the callee pops 556 bytes.
CSF_FUNC(int, __fastcall, WeaponDefs_Resize, 0x00784900, (
    void* self, void* edx, int count,
    uint32_t word0, uint32_t word1, uint32_t word2, uint32_t word3,
    uint32_t word4, uint32_t word5, uint32_t word6, uint32_t word7,
    uint32_t word8, uint32_t word9, uint32_t word10, uint32_t word11,
    uint32_t word12, uint32_t word13, uint32_t word14, uint32_t word15,
    uint32_t word16, uint32_t word17, uint32_t word18, uint32_t word19,
    uint32_t word20, uint32_t word21, uint32_t word22, uint32_t word23,
    uint32_t word24, uint32_t word25, uint32_t word26, uint32_t word27,
    uint32_t word28, uint32_t word29, uint32_t word30, uint32_t word31,
    uint32_t word32, uint32_t word33, uint32_t word34, uint32_t word35,
    uint32_t word36, uint32_t word37, uint32_t word38, uint32_t word39,
    uint32_t word40, uint32_t word41, uint32_t word42, uint32_t word43,
    uint32_t word44, uint32_t word45, uint32_t word46, uint32_t word47,
    uint32_t word48, uint32_t word49, uint32_t word50, uint32_t word51,
    uint32_t word52, uint32_t word53, uint32_t word54, uint32_t word55,
    uint32_t word56, uint32_t word57, uint32_t word58, uint32_t word59,
    uint32_t word60, uint32_t word61, uint32_t word62, uint32_t word63,
    uint32_t word64, uint32_t word65, uint32_t word66, uint32_t word67,
    uint32_t word68, uint32_t word69, uint32_t word70, uint32_t word71,
    uint32_t word72, uint32_t word73, uint32_t word74, uint32_t word75,
    uint32_t word76, uint32_t word77, uint32_t word78, uint32_t word79,
    uint32_t word80, uint32_t word81, uint32_t word82, uint32_t word83,
    uint32_t word84, uint32_t word85, uint32_t word86, uint32_t word87,
    uint32_t word88, uint32_t word89, uint32_t word90, uint32_t word91,
    uint32_t word92, uint32_t word93, uint32_t word94, uint32_t word95,
    uint32_t word96, uint32_t word97, uint32_t word98, uint32_t word99,
    uint32_t word100, uint32_t word101, uint32_t word102, uint32_t word103,
    uint32_t word104, uint32_t word105, uint32_t word106, uint32_t word107,
    uint32_t word108, uint32_t word109, uint32_t word110, uint32_t word111,
    uint32_t word112, uint32_t word113, uint32_t word114, uint32_t word115,
    uint32_t word116, uint32_t word117, uint32_t word118, uint32_t word119,
    uint32_t word120, uint32_t word121, uint32_t word122, uint32_t word123,
    uint32_t word124, uint32_t word125, uint32_t word126, uint32_t word127,
    uint32_t word128, uint32_t word129, uint32_t word130, uint32_t word131,
    uint32_t word132, uint32_t word133, uint32_t word134, uint32_t word135,
    uint32_t word136, uint32_t word137));

CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9C74, 0x008A9C74);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9C78, 0x008A9C78);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A9C7C, 0x008A9C7C);
CSF_GLOBAL(const char*, g_pszKey_FILE_8A9C84, 0x008A9C84);
CSF_GLOBAL(const char*, g_pszKey_FILE2, 0x008A9C88);
CSF_GLOBAL(const char*, g_pszKey_TIPO_8A9C8C, 0x008A9C8C);
CSF_GLOBAL(const char*, g_pszKey_AUTOMATISMO, 0x008A9C94);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_DFF, 0x008A9CA0);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_TEXTURE, 0x008A9CA4);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_COLOR, 0x008A9CA8);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_DURACION, 0x008A9CAC);
CSF_GLOBAL(const char*, g_pszKey_FOGONAZO_RADIO, 0x008A9CB0);
CSF_GLOBAL(const char*, g_pszKey_CARGA, 0x008A9CBC);
CSF_GLOBAL(const char*, g_pszKey_ALCANCE, 0x008A9CC0);
CSF_GLOBAL(const char*, g_pszKey_ALCANCE_RANGO_MIN, 0x008A9CD0);
CSF_GLOBAL(const char*, g_pszKey_ALCANCE_RANGO_MAX, 0x008A9CD4);
CSF_GLOBAL(const char*, g_pszKey_DESVIACION_H, 0x008A9CE0);
CSF_GLOBAL(const char*, g_pszKey_DESVIACION_V, 0x008A9CE4);
CSF_GLOBAL(const char*, g_pszKey_DESVIACION_FA, 0x008A9CE8);
CSF_GLOBAL(const char*, g_pszKey_BALAS, 0x008A9CEC);
CSF_GLOBAL(const char*, g_pszKey_CARGADORES, 0x008A9CF0);
CSF_GLOBAL(const char*, g_pszKey_MAX_CARGADORES, 0x008A9CF4);
CSF_GLOBAL(const char*, g_pszKey_DAMAGE_8A9D00, 0x008A9D00);
CSF_GLOBAL(const char*, g_pszKey_DISPERSION, 0x008A9D08);
CSF_GLOBAL(const char*, g_pszKey_RADIO_8A9D0C, 0x008A9D0C);
CSF_GLOBAL(const char*, g_pszKey_CADENCIA, 0x008A9D10);
CSF_GLOBAL(const char*, g_pszKey_DECALUV1, 0x008A9D2C);
CSF_GLOBAL(const char*, g_pszKey_DECALUV2, 0x008A9D30);
CSF_GLOBAL(const char*, g_pszKey_MANO, 0x008A9D34);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_8A9D38, 0x008A9D38);
CSF_GLOBAL(const char*, g_pszKey_ANIMACIONES_PLAYER, 0x008A9D3C);
CSF_GLOBAL(const char*, g_pszKey_TIEMPO_DETONACION, 0x008A9D4C);
CSF_GLOBAL(const char*, g_pszKey_VELOCIDAD, 0x008A9D50);
CSF_GLOBAL(const char*, g_pszKey_ANGULO_LANZAMIENTO, 0x008A9D54);
CSF_GLOBAL(const char*, g_pszKey_ATENUACION_BOTE, 0x008A9D60);

// The packet also labels the child-count function address as a vtable.
[[maybe_unused]] static auto const g_MenuNodeVtable = CSF_VTBL(0x00692B80);

static std::uint8_t* WeaponDefs_GetEntry(void* self, std::uint32_t byteOffset)
{
    void* entries = CSF_AT(void*, self, 0x08);
    return CSF_AT(std::uint8_t*, entries, 0x04) + byteOffset;
}

static void WeaponDefs_ReadStringField(
    void* self, std::uint32_t byteOffset, std::uint8_t* node,
    const char* key, char* buffer, std::size_t fieldOffset)
{
    MenuExtras_ReadText(node, nullptr, const_cast<char*>(key), buffer, 256);
    std::uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);
    unsigned int length = static_cast<unsigned int>(std::strlen(buffer));
    AssignStringRange_429B60(entry + fieldOffset, nullptr, buffer, length);
}

static void WeaponDefs_ReadIntegerField(
    void* self, std::uint32_t byteOffset, std::uint8_t* node,
    const char* key, std::size_t fieldOffset)
{
    std::uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);
    int value = ReadIntegerAttribute(node, nullptr, const_cast<char*>(key));
    CSF_AT(int, entry, fieldOffset) = value;
}

static void WeaponDefs_ReadFloatField(
    void* self, std::uint32_t byteOffset, std::uint8_t* node,
    const char* key, std::size_t fieldOffset)
{
    std::uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);
    double value = ReadFloatAttribute(node, nullptr, const_cast<char*>(key));
    CSF_AT(float, entry, fieldOffset) = static_cast<float>(value);
}

static char __fastcall WeaponDefs_LoadDataTable(void* self, void* /*edx*/, S_00692b80* source)
{
    char buffer[0x10C] = {};
    std::uint8_t* list = LookupNamedElement(
        reinterpret_cast<std::uint8_t*>(source), nullptr, g_pszKey_LISTADATOS_8A9C74);
    std::uint32_t count = MenuExtras_GetChildCount(list, nullptr);

    void* entries = CSF_AT(void*, self, 0x08);
    std::uint32_t defaults[138] = {};
    WeaponDefs_InitializeDefaults(reinterpret_cast<int>(defaults), 0);
    WeaponDefs_Resize(
        entries, nullptr, static_cast<int>(count),
        defaults[0], defaults[1], defaults[2], defaults[3],
        defaults[4], defaults[5], defaults[6], defaults[7],
        defaults[8], defaults[9], defaults[10], defaults[11],
        defaults[12], defaults[13], defaults[14], defaults[15],
        defaults[16], defaults[17], defaults[18], defaults[19],
        defaults[20], defaults[21], defaults[22], defaults[23],
        defaults[24], defaults[25], defaults[26], defaults[27],
        defaults[28], defaults[29], defaults[30], defaults[31],
        defaults[32], defaults[33], defaults[34], defaults[35],
        defaults[36], defaults[37], defaults[38], defaults[39],
        defaults[40], defaults[41], defaults[42], defaults[43],
        defaults[44], defaults[45], defaults[46], defaults[47],
        defaults[48], defaults[49], defaults[50], defaults[51],
        defaults[52], defaults[53], defaults[54], defaults[55],
        defaults[56], defaults[57], defaults[58], defaults[59],
        defaults[60], defaults[61], defaults[62], defaults[63],
        defaults[64], defaults[65], defaults[66], defaults[67],
        defaults[68], defaults[69], defaults[70], defaults[71],
        defaults[72], defaults[73], defaults[74], defaults[75],
        defaults[76], defaults[77], defaults[78], defaults[79],
        defaults[80], defaults[81], defaults[82], defaults[83],
        defaults[84], defaults[85], defaults[86], defaults[87],
        defaults[88], defaults[89], defaults[90], defaults[91],
        defaults[92], defaults[93], defaults[94], defaults[95],
        defaults[96], defaults[97], defaults[98], defaults[99],
        defaults[100], defaults[101], defaults[102], defaults[103],
        defaults[104], defaults[105], defaults[106], defaults[107],
        defaults[108], defaults[109], defaults[110], defaults[111],
        defaults[112], defaults[113], defaults[114], defaults[115],
        defaults[116], defaults[117], defaults[118], defaults[119],
        defaults[120], defaults[121], defaults[122], defaults[123],
        defaults[124], defaults[125], defaults[126], defaults[127],
        defaults[128], defaults[129], defaults[130], defaults[131],
        defaults[132], defaults[133], defaults[134], defaults[135],
        defaults[136], defaults[137]);

    std::uint32_t byteOffset = 0;
    for (std::uint32_t index = 0; index < count; ++index, byteOffset += 552) {
        std::uint8_t* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(list, nullptr, index));

        WeaponDefs_ReadIntegerField(self, byteOffset, component, g_pszKey_ID_8A9C78, 0x000);
        WeaponDefs_ReadStringField(self, byteOffset, component, g_pszKey_NOMBRE_8A9C7C, buffer, 4);
        WeaponDefs_ReadStringField(self, byteOffset, component, g_pszKey_FILE_8A9C84, buffer, 60);
        WeaponDefs_ReadStringField(self, byteOffset, component, g_pszKey_FILE2, buffer, 88);

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
        std::uint8_t* entry = WeaponDefs_GetEntry(self, byteOffset);
        int parsed = WeaponDefs_ParseWeaponType(buffer);
        CSF_AT(int, entry, 0x074) = parsed;

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_AUTOMATISMO), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        parsed = WeaponDefs_ParseAutomaticMode(buffer);
        CSF_AT(int, entry, 0x07C) = parsed;

        WeaponDefs_ReadStringField(self, byteOffset, component, g_pszKey_FOGONAZO_DFF, buffer, 136);
        WeaponDefs_ReadStringField(self, byteOffset, component, g_pszKey_FOGONAZO_TEXTURE, buffer, 164);
        WeaponDefs_ReadIntegerField(self, byteOffset, component, g_pszKey_FOGONAZO_COLOR, 0x0C0);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_FOGONAZO_DURACION, 0x0C4);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_FOGONAZO_RADIO, 0x0C8);

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_CARGA), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        parsed = WeaponDefs_ParseLoadingMode(buffer);
        CSF_AT(int, entry, 0x0D4) = parsed;

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_ALCANCE), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        parsed = WeaponDefs_ParseRangeType(buffer);
        CSF_AT(int, entry, 0x0D8) = parsed;

        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_ALCANCE_RANGO_MIN, 0x0E8);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_ALCANCE_RANGO_MAX, 0x0EC);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_DESVIACION_H, 0x0F8);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_DESVIACION_V, 0x0FC);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_DESVIACION_FA, 0x100);
        WeaponDefs_ReadIntegerField(self, byteOffset, component, g_pszKey_BALAS, 0x104);
        WeaponDefs_ReadIntegerField(self, byteOffset, component, g_pszKey_CARGADORES, 0x108);
        WeaponDefs_ReadIntegerField(self, byteOffset, component, g_pszKey_MAX_CARGADORES, 0x10C);
        WeaponDefs_ReadIntegerField(self, byteOffset, component, g_pszKey_DAMAGE_8A9D00, 0x118);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_DISPERSION, 0x120);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_RADIO_8A9D0C, 0x124);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_CADENCIA, 0x128);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_TIEMPO_DETONACION, 0x1A4);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_VELOCIDAD, 0x1A8);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_ATENUACION_BOTE, 0x1B8);
        WeaponDefs_ReadFloatField(self, byteOffset, component, g_pszKey_ANGULO_LANZAMIENTO, 0x1AC);

        CSF_AT(int, WeaponDefs_GetEntry(self, byteOffset), 304) = -1;
        CSF_AT(int, WeaponDefs_GetEntry(self, byteOffset), 316) = -1;
        Hud_ReadFloatPair(component, nullptr, const_cast<char*>(g_pszKey_DECALUV1),
                          reinterpret_cast<int>(WeaponDefs_GetEntry(self, byteOffset) + 320));
        Hud_ReadFloatPair(component, nullptr, const_cast<char*>(g_pszKey_DECALUV2),
                          reinterpret_cast<int>(WeaponDefs_GetEntry(self, byteOffset) + 328));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_MANO), buffer, 256);
        entry = WeaponDefs_GetEntry(self, byteOffset);
        parsed = WeaponDefs_ParseHand(buffer);
        CSF_AT(int, entry, 0x150) = parsed;

        std::uint8_t* animations = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES_8A9D38));
        if (animations) {
            std::uint32_t animationCount = MenuExtras_GetChildCount(animations, nullptr);
            for (std::uint32_t animationIndex = 0; animationIndex < animationCount; ++animationIndex) {
                std::uint8_t* animation = reinterpret_cast<std::uint8_t*>(
                    GetComponent(animations, nullptr, animationIndex));
                std::int32_t reference[3] = {-1, -1, -1};
                reference[0] = ReadIntegerAttribute(
                    animation, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));
                void* animationTable = CSF_AT(void*, self, 0x04);
                reference[2] =
                    reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, animationTable, 0)[2])(
                            animationTable, nullptr, reference[0]);
                MenuExtras_ReadText(animation, nullptr,
                                    const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
                reference[1] = WeaponDefs_ParseAnimationType(buffer);
                if (reference[2] != -1 && reference[0] != -1 && reference[1] != -1) {
                    AnimRefList_Add(WeaponDefs_GetEntry(self, byteOffset) + 340, nullptr,
                                    reinterpret_cast<std::uint32_t*>(reference));
                }
            }
        }

        std::uint8_t* playerAnimations = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_ANIMACIONES_PLAYER));
        if (playerAnimations) {
            std::uint32_t animationCount = MenuExtras_GetChildCount(playerAnimations, nullptr);
            for (std::uint32_t animationIndex = 0; animationIndex < animationCount; ++animationIndex) {
                std::uint8_t* animation = reinterpret_cast<std::uint8_t*>(
                    GetComponent(playerAnimations, nullptr, animationIndex));
                std::int32_t reference[6] = {-1, -1, -1, 0, 0, 0};
                reference[0] = ReadIntegerAttribute(
                    animation, nullptr, const_cast<char*>(g_pszKey_ID_8A9C78));
                void* animationTable = CSF_AT(void*, self, 0x04);
                reference[2] =
                    reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, animationTable, 0)[2])(
                            animationTable, nullptr, reference[0]);
                MenuExtras_ReadText(animation, nullptr,
                                    const_cast<char*>(g_pszKey_TIPO_8A9C8C), buffer, 256);
                reference[1] = ParsePlayerAnimTypeName(buffer);
                if (reference[2] != -1 && reference[0] != -1 && reference[1] != -1) {
                    AnimRefList_Add(WeaponDefs_GetEntry(self, byteOffset) + 356, nullptr,
                                    reinterpret_cast<std::uint32_t*>(reference));
                }
            }
        }

        WeaponDefs_FinalizeEntry(reinterpret_cast<int>(WeaponDefs_GetEntry(self, byteOffset)));
    }
    return 1;
}

HOOK(0x00786760, WeaponDefs_LoadDataTable, "auto");
