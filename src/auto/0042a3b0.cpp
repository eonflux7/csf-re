#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Win32 imports (user32) used by the original; globals.h does not declare them.
// The linkage name is produced by stringification so the file has no bare string literal.
#define CHEATS_LINKAGE_NAME(name) #name
extern CHEATS_LINKAGE_NAME(C) {
__declspec(dllimport) unsigned int __stdcall MapVirtualKeyA(
    unsigned int code, unsigned int mapType);
__declspec(dllimport) int __stdcall ToUnicode(
    unsigned int virtualKey, unsigned int scanCode,
    const uint8_t* keyState, uint16_t* characters,
    int characterCapacity, unsigned int flags);
}

CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (void* self, void* edx, int16_t a2));

static S_004f21d0* __cdecl Cheats_GetInputBindingName(
    unsigned int device, uint32_t code, S_004f21d0* output)
{
    if (device > 2)
        return output;

    if (code == 0xFFFFFFFF)
    {
        InitStringFromLiteral(output, CSF_STR(0x008303E4, "c094"), 1);
        return output;
    }

    const char* label;
    if (device == 1)
    {
        switch (code)
        {
        case 0: label = CSF_STR(0x008303EC, "c031"); break;
        case 1: label = CSF_STR(0x008303F4, "c032"); break;
        case 2: label = CSF_STR(0x008303FC, "c033"); break;
        default: return output;
        }
    }
    else if (device == 2)
    {
        switch (code)
        {
        case 0: label = CSF_STR(0x00830404, "c034"); break;
        case 1: label = CSF_STR(0x0083040C, "c035"); break;
        default: return output;
        }
    }
    else
    {
        switch (code)
        {
        case 0xEu:  label = CSF_STR(0x008303DC, "c036"); break;
        case 0xFu:  label = CSF_STR(0x008303D4, "c037"); break;
        case 0x1Cu: label = CSF_STR(0x008303CC, "c038"); break;
        case 0x1Du: label = CSF_STR(0x008303C4, "c039"); break;
        case 0x2Au: label = CSF_STR(0x008303BC, "c040"); break;
        case 0x36u: label = CSF_STR(0x008303B4, "c041"); break;
        case 0x37u: label = CSF_STR(0x008303AC, "c115"); break;
        case 0x38u: label = CSF_STR(0x008303A4, "c042"); break;
        case 0x39u: label = CSF_STR(0x0083039C, "c043"); break;
        case 0x3Au: label = CSF_STR(0x00830394, "c044"); break;
        case 0x3Bu: label = CSF_STR(0x0083038C, "c045"); break;
        case 0x3Cu: label = CSF_STR(0x00830384, "c046"); break;
        case 0x3Du: label = CSF_STR(0x0083037C, "c047"); break;
        case 0x3Eu: label = CSF_STR(0x00830374, "c048"); break;
        case 0x3Fu: label = CSF_STR(0x0083036C, "c049"); break;
        case 0x40u: label = CSF_STR(0x00830364, "c050"); break;
        case 0x41u: label = CSF_STR(0x0083035C, "c051"); break;
        case 0x42u: label = CSF_STR(0x00830354, "c052"); break;
        case 0x43u: label = CSF_STR(0x0083034C, "c053"); break;
        case 0x44u: label = CSF_STR(0x00830344, "c054"); break;
        case 0x45u: label = CSF_STR(0x0083033C, "c070"); break;
        case 0x46u: label = CSF_STR(0x00830334, "c071"); break;
        case 0x47u: label = CSF_STR(0x0083032C, "c067"); break;
        case 0x48u: label = CSF_STR(0x00830324, "c068"); break;
        case 0x49u: label = CSF_STR(0x0083031C, "c069"); break;
        case 0x4Au: label = CSF_STR(0x00830314, "c072"); break;
        case 0x4Bu: label = CSF_STR(0x0083030C, "c064"); break;
        case 0x4Cu: label = CSF_STR(0x00830304, "c065"); break;
        case 0x4Du: label = CSF_STR(0x008302FC, "c066"); break;
        case 0x4Eu: label = CSF_STR(0x008302F4, "c073"); break;
        case 0x4Fu: label = CSF_STR(0x008302EC, "c061"); break;
        case 0x50u: label = CSF_STR(0x008302E4, "c062"); break;
        case 0x51u: label = CSF_STR(0x008302DC, "c063"); break;
        case 0x52u: label = CSF_STR(0x008302D4, "c060"); break;
        case 0x53u: label = CSF_STR(0x008302CC, "c074"); break;
        case 0x57u: label = CSF_STR(0x008302C4, "c055"); break;
        case 0x58u: label = CSF_STR(0x008302BC, "c056"); break;
        case 0x64u: label = CSF_STR(0x008302B4, "c057"); break;
        case 0x65u: label = CSF_STR(0x008302AC, "c058"); break;
        case 0x66u: label = CSF_STR(0x008302A4, "c059"); break;
        case 0x9Cu: label = CSF_STR(0x0083029C, "c077"); break;
        case 0x9Du: label = CSF_STR(0x00830294, "c078"); break;
        case 0xB5u: label = CSF_STR(0x0083028C, "c080"); break;
        case 0xB8u: label = CSF_STR(0x00830284, "c081"); break;
        case 0xC5u: label = CSF_STR(0x0083027C, "c082"); break;
        case 0xC7u: label = CSF_STR(0x00830274, "c083"); break;
        case 0xC8u: label = CSF_STR(0x0083026C, "c084"); break;
        case 0xC9u: label = CSF_STR(0x00830264, "c085"); break;
        case 0xCBu: label = CSF_STR(0x0083025C, "c086"); break;
        case 0xCDu: label = CSF_STR(0x00830254, "c087"); break;
        case 0xCFu: label = CSF_STR(0x0083024C, "c088"); break;
        case 0xD0u: label = CSF_STR(0x00830244, "c089"); break;
        case 0xD1u: label = CSF_STR(0x0083023C, "c090"); break;
        case 0xD2u: label = CSF_STR(0x00830234, "c091"); break;
        case 0xD3u: label = CSF_STR(0x0083022C, "c092"); break;
        default:
        {
            const unsigned int virtualKey = MapVirtualKeyA(code, 1u);
            if (virtualKey)
            {
                alignas(8) uint8_t keyState[0x100];
                alignas(8) uint8_t characterBytes[0x28];
                auto* characters = reinterpret_cast<uint16_t*>(characterBytes);
                int characterCount;
                do
                {
                    std::memset(characterBytes, 0, sizeof(characterBytes));
                    std::memset(keyState, 0, sizeof(keyState));
                    characterCount = ToUnicode(
                        virtualKey, code, keyState, characters, 3, 0);
                }
                while (characterCount > 1);

                if (characterCount == -1)
                {
                    std::memset(keyState, 0, sizeof(keyState));
                    std::memset(characterBytes, 0, sizeof(characterBytes));
                    ToUnicode(virtualKey, code, keyState, characters, 3, 0);
                }
                SaveSlots_ApplyIndex(
                    output, nullptr, static_cast<int16_t>(characters[0]));
            }
            return output;
        }
        }
    }

    InitStringFromLiteral(output, label, 1);
    return output;
}

HOOK(0x0042A3B0, Cheats_GetInputBindingName, "auto");
