#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// ---------------------------------------------------------------------------
// Callees (bound by address)
// ---------------------------------------------------------------------------
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(uint32_t*, __fastcall, WaterParams_Construct, 0x006FDEE0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, WaterList_Reserve, 0x007761E0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, WaterList_PushBack, 0x00777FC0, (void* self, void* edx, void* a3));

// ---------------------------------------------------------------------------
// Keyword globals
// ---------------------------------------------------------------------------
CSF_GLOBAL(char*, g_pszKey_AGUAS, 0x008A9A88);
CSF_GLOBAL(char*, g_pszKey_NOMBRE_game_data_defs, 0x008A9A94);
CSF_GLOBAL(char*, g_pszKey_AGUA_DIFFUSEADD, 0x008A9B74);
CSF_GLOBAL(char*, g_pszKey_AGUA_DIFFUSEMOD, 0x008A9B78);
CSF_GLOBAL(char*, g_pszKey_AGUA_SPECULAR, 0x008A9B7C);
CSF_GLOBAL(char*, g_pszKey_AGUA_SPECULARLEVEL, 0x008A9B80);
CSF_GLOBAL(char*, g_pszKey_AGUA_SPECULARGLOSS, 0x008A9B84);
CSF_GLOBAL(char*, g_pszKey_AGUA_SPECULARENV, 0x008A9B88);
CSF_GLOBAL(char*, g_pszKey_AGUA_UVOSC, 0x008A9B8C);
CSF_GLOBAL(char*, g_pszKey_AGUA_TRANSPARENCY, 0x008A9B90);
CSF_GLOBAL(char*, g_pszKey_AGUA_BLUR, 0x008A9B94);
CSF_GLOBAL(char*, g_pszKey_AGUA_DISPLACEMENTVEL, 0x008A9B98);
CSF_GLOBAL(char*, g_pszKey_AGUA_UVSCALE, 0x008A9B9C);
CSF_GLOBAL(char*, g_pszKey_AGUA_ENVOSC, 0x008A9BA0);
CSF_GLOBAL(char*, g_pszKey_AGUA_TEXNM, 0x008A9BA4);
CSF_GLOBAL(char*, g_pszKey_AGUA_NORMALPERT, 0x008A9BA8);
CSF_GLOBAL(char*, g_pszKey_AGUA_HEIGHTOSCSIZE, 0x008A9BAC);
CSF_GLOBAL(char*, g_pszKey_AGUA_HEIGHTOSCVEL, 0x008A9BB0);
CSF_GLOBAL(char*, g_pszKey_AGUA_NORMALMAPZ, 0x008A9BB4);

// ---------------------------------------------------------------------------
// Local layouts (stack temporaries of the original)
// ---------------------------------------------------------------------------
namespace {

// MSVC 7.1 std::string (0x1C bytes)
struct LegacyStdString
{
    std::uint32_t allocatorProxy;      // +0x00
    union
    {
        char  inlineBuffer[16];        // +0x04
        char* heapBuffer;
    };
    std::uint32_t length;              // +0x14
    std::uint32_t capacity;            // +0x18
};

struct WaterParams
{
    int   diffuseAdd;                  // +0x00
    int   diffuseMod;                  // +0x04
    int   specular;                    // +0x08
    float specularLevel;               // +0x0C
    float specularGloss;               // +0x10
    float specularEnv;                 // +0x14
    float uvOsc;                       // +0x18
    float transparency;                // +0x1C
    float blur;                        // +0x20
    float displacementVel;             // +0x24
    float uvScale;                     // +0x28
    float envOsc;                      // +0x2C
    LegacyStdString normalMapTexture;  // +0x30
    float normalPert;                  // +0x4C
    float heightOscSize;               // +0x50
    float heightOscVel;                // +0x54
    float normalMapZ;                  // +0x58
    std::uint8_t reserved[0x30];       // +0x5C (rest of the object's stack footprint)
};

struct WaterEntry
{
    LegacyStdString name;              // +0x00
    WaterParams     params;            // +0x1C
};

} // namespace

// ---------------------------------------------------------------------------
// CLevelData::LoadWaters  (reads the AGUAS block: one water definition per child)
// ---------------------------------------------------------------------------
static std::uint32_t __fastcall CLevelData_LoadWaters(void* self, void* /*edx*/, std::uint8_t* rootNode)
{
    std::uint8_t* watersNode = FindProperty(rootNode, nullptr, g_pszKey_AGUAS);
    if (!watersNode)
        return 0;

    std::uint32_t waterCount = MenuExtras_GetChildCount(watersNode, nullptr);
    void* waterList = (std::uint8_t*)self + 0x44;
    WaterList_Reserve(waterList, nullptr, waterCount);

    for (std::uint32_t index = 0; index < waterCount; ++index)
    {
        std::uint8_t* waterNode = reinterpret_cast<std::uint8_t*>(GetComponent(watersNode, nullptr, index));

        char textBuffer[256];
        WaterEntry entry;

        entry.name.capacity = 15;
        entry.name.length = 0;
        entry.name.inlineBuffer[0] = 0;
        WaterParams_Construct(&entry.params, nullptr);

        MenuExtras_ReadText(waterNode, nullptr, g_pszKey_NOMBRE_game_data_defs, textBuffer, 256);
        AssignStringRange_429B60(&entry.name, nullptr, textBuffer, (unsigned int)std::strlen(textBuffer));

        entry.params.diffuseAdd      = ReadIntegerAttribute(waterNode, nullptr, g_pszKey_AGUA_DIFFUSEADD);
        entry.params.diffuseMod      = ReadIntegerAttribute(waterNode, nullptr, g_pszKey_AGUA_DIFFUSEMOD);
        entry.params.specular        = ReadIntegerAttribute(waterNode, nullptr, g_pszKey_AGUA_SPECULAR);
        entry.params.blur            = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_BLUR);
        entry.params.displacementVel = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_DISPLACEMENTVEL);
        entry.params.envOsc          = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_ENVOSC);
        entry.params.heightOscSize   = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_HEIGHTOSCSIZE);
        entry.params.heightOscVel    = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_HEIGHTOSCVEL);
        entry.params.normalPert      = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_NORMALPERT);
        entry.params.specularEnv     = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_SPECULARENV);
        entry.params.specularGloss   = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_SPECULARGLOSS);
        entry.params.specularLevel   = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_SPECULARLEVEL);
        entry.params.transparency    = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_TRANSPARENCY);
        entry.params.uvOsc           = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_UVOSC);
        entry.params.uvScale         = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_UVSCALE);

        if (FindProperty(waterNode, nullptr, g_pszKey_AGUA_NORMALMAPZ))
            entry.params.normalMapZ = (float)ReadFloatAttribute(waterNode, nullptr, g_pszKey_AGUA_NORMALMAPZ);

        MenuExtras_ReadText(waterNode, nullptr, g_pszKey_AGUA_TEXNM, textBuffer, 256);
        AssignStringRange_429B60(&entry.params.normalMapTexture, nullptr, textBuffer, (unsigned int)std::strlen(textBuffer));

        WaterList_PushBack(waterList, nullptr, &entry);

        // ~WaterEntry: release both strings
        if (entry.params.normalMapTexture.capacity >= 0x10 && entry.params.normalMapTexture.heapBuffer)
            Mem_FreeDebug(entry.params.normalMapTexture.heapBuffer);
        entry.params.normalMapTexture.capacity = 15;
        entry.params.normalMapTexture.length = 0;
        entry.params.normalMapTexture.inlineBuffer[0] = 0;

        if (entry.name.capacity >= 0x10)
        {
            if (entry.name.heapBuffer)
                Mem_FreeDebug(entry.name.heapBuffer);
        }
        entry.name.capacity = 15;
        entry.name.length = 0;
        entry.name.inlineBuffer[0] = 0;
    }

    return waterCount;
}

HOOK(0x007781E0, CLevelData_LoadWaters, "auto");
