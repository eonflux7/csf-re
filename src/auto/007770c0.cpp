#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (void* self, void* edx, void* name));
CSF_FUNC(unsigned int, __fastcall, DataDefs_RegisterField, 0x0055AF00, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_psz_DEFAULT, 0x008A9BF4);

// MSVC 7.1 std::string layout (allocator at +0, SSO buffer at +4, size +0x14, capacity +0x18)
struct LocalString
{
    uint32_t allocator;
    union
    {
        char  buffer[16];
        char* heapPtr;
    };
    uint32_t size;
    uint32_t capacity;
};

struct FieldDescriptor
{
    const char* name;
    void*       target;
    uint32_t    reserved;
    uint32_t    typeFlags;
};

static void InitEmptyString(void* str)
{
    CSF_AT(uint32_t, str, 0x18) = 15;
    CSF_AT(uint32_t, str, 0x14) = 0;
    CSF_AT(uint8_t, str, 0x4) = 0;
}

static void AppendEnumName(void* list, const char* text, unsigned int length)
{
    LocalString name;
    name.capacity = 15;
    name.size = 0;
    name.buffer[0] = 0;
    AssignStringRange_429B60(&name, nullptr, const_cast<char*>(text), length);
    MenuManager_AppendPageName(list, nullptr, &name);
    if (name.capacity >= 0x10 && name.heapPtr)
        Mem_FreeDebug(name.heapPtr);
}

static void RegisterField(S_0055af00* self, const char* name, void* target, uint32_t typeFlags)
{
    FieldDescriptor desc;
    desc.name = name;
    desc.target = target;
    desc.typeFlags = typeFlags;
    desc.reserved = 0;
    DataDefs_RegisterField(self, nullptr, reinterpret_cast<uint32_t*>(&desc));
}

static S_0055af00* __fastcall GameDataDefs_ConstructSceneDefs(S_0055af00* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    InitEmptyString(base + 0x18);            // sky file
    InitEmptyString(base + 0x434);
    CSF_AT(uint8_t, self, 0x450) = 0;
    InitEmptyString(base + 0x464);           // simplified model
    InitEmptyString(base + 0x880);
    CSF_AT(uint8_t, self, 0x89c) = 0;

    CSF_AT(uint32_t, self, 0x8c8) = 0;       // fog type names
    CSF_AT(uint32_t, self, 0x8cc) = 0;
    CSF_AT(uint32_t, self, 0x8d0) = 0;
    CSF_AT(int32_t, self, 0x8c0) = -1;
    CSF_AT(uint32_t, self, 0x8ec) = 0;       // water fog type names
    CSF_AT(uint32_t, self, 0x8f0) = 0;
    CSF_AT(uint32_t, self, 0x8f4) = 0;
    CSF_AT(int32_t, self, 0x8e4) = -1;
    CSF_AT(uint32_t, self, 0x910) = 0;       // atmospheric fx names
    CSF_AT(uint32_t, self, 0x914) = 0;
    CSF_AT(uint32_t, self, 0x918) = 0;
    CSF_AT(int32_t, self, 0x908) = -1;

    InitEmptyString(base + 0x924);           // RTS texture
    InitEmptyString(base + 0xd40);
    CSF_AT(uint8_t, self, 0xd5c) = 0;
    CSF_AT(uint32_t, self, 0xd60) = 0;
    CSF_AT(uint32_t, self, 0xd64) = 0;
    CSF_AT(uint32_t, self, 0xd68) = 0;
    CSF_AT(uint32_t, self, 0xd6c) = 0;
    CSF_AT(uint32_t, self, 0xd70) = 0;
    CSF_AT(uint32_t, self, 0xd74) = 0;
    CSF_AT(uint32_t, self, 0xd7c) = 0;
    CSF_AT(uint32_t, self, 0xd80) = 0;
    CSF_AT(uint32_t, self, 0xd84) = 0;
    CSF_AT(uint32_t, self, 0xd88) = 0;
    CSF_AT(uint32_t, self, 0xd8c) = 0;
    CSF_AT(uint32_t, self, 0xd90) = 0;
    InitEmptyString(base + 0xd94);           // tactical map texture
    InitEmptyString(base + 0x11b0);
    CSF_AT(uint8_t, self, 0x11cc) = 0;
    InitEmptyString(base + 0x11d0);          // scenario flis
    InitEmptyString(base + 0x15ec);
    CSF_AT(uint8_t, self, 0x1608) = 0;
    InitEmptyString(base + 0x160c);          // sector map
    InitEmptyString(base + 0x1a28);
    CSF_AT(uint8_t, self, 0x1a44) = 0;

    InitEmptyString(base + 0x1a48);          // save game description
    AssignStringRange_429B60(base + 0x1a48, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    InitEmptyString(base + 0x1a64);          // save game scene
    AssignStringRange_429B60(base + 0x1a64, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);

    InitEmptyString(base + 0x1a80);          // opening video
    InitEmptyString(base + 0x1e9c);
    CSF_AT(uint8_t, self, 0x1eb8) = 0;
    InitEmptyString(base + 0x1ebc);          // aim id
    CSF_AT(uint8_t, self, 0x1eec) = 0;
    CSF_AT(uint32_t, self, 0x1ef0) = 20;
    CSF_AT(uint32_t, self, 0x1ef4) = 60;
    CSF_AT(uint32_t, self, 0x1ef8) = 38;
    CSF_AT(uint8_t, self, 0x1efc) = 0;
    CSF_AT(uint8_t, self, 0x1efd) = 0;
    CSF_AT(uint8_t, self, 0x1efe) = 0;

    self->field_0010 = 0xFFFFFFFF;
    AssignStringRange_429B60(base + 0x18, nullptr, const_cast<char*>(CSF_STR(0x008723F0, "*.dff")), 5);
    CSF_AT(uint32_t, self, 0x454) = 0;
    CSF_AT(uint32_t, self, 0x458) = 0;
    CSF_AT(uint32_t, self, 0x45c) = 0;
    CSF_AT(uint8_t, self, 0x460) = 1;
    AssignStringRange_429B60(base + 0x464, nullptr, const_cast<char*>(CSF_STR(0x0086BC2C, "*.rws")), 5);
    AssignStringRange_429B60(base + 0xd94, nullptr, const_cast<char*>(CSF_STR(0x0086BC1C, "*.png")), 5);
    AssignStringRange_429B60(base + 0x924, nullptr, const_cast<char*>(CSF_STR(0x0086BC1C, "*.png")), 5);
    AssignStringRange_429B60(base + 0x11d0, nullptr, const_cast<char*>(CSF_STR(0x008723E8, "*.fli")), 5);
    AssignStringRange_429B60(base + 0x1a80, nullptr, const_cast<char*>(CSF_STR(0x0086BC14, "*.bik")), 5);
    {
        const char* defaultName = g_psz_DEFAULT;
        AssignStringRange_429B60(base + 0x1ebc, nullptr, const_cast<char*>(defaultName),
                                 static_cast<unsigned int>(strlen(defaultName)));
    }

    self->field_0014 = 0xFFFFFFFF;
    CSF_AT(uint8_t, self, 0x8a0) = 0;
    CSF_AT(uint32_t, self, 0x8a4) = 0xFF000000;
    CSF_AT(uint32_t, self, 0x8a8) = 0xFF3C5064;
    CSF_AT(uint32_t, self, 0x8ac) = 0;
    CSF_AT(float, self, 0x8b0) = 10.0f;
    CSF_AT(float, self, 0x8b4) = 10000.0f;
    CSF_AT(float, self, 0x8b8) = -1.0f;
    CSF_AT(uint8_t, self, 0x8bc) = 1;
    CSF_AT(uint32_t, self, 0x8d4) = 0xFF353433;
    CSF_AT(uint32_t, self, 0x8d8) = 255;
    CSF_AT(float, self, 0x8dc) = 0.005f;
    CSF_AT(uint32_t, self, 0x8e0) = 0;

    AppendEnumName(base + 0x8c4, CSF_STR(0x008723E0, "LINEAR"), 6);
    AppendEnumName(base + 0x8c4, CSF_STR(0x008723D4, "EXPONENTIAL"), 11);
    AppendEnumName(base + 0x8c4, CSF_STR(0x008723C4, "EXPONENTIAL2"), 12);
    CSF_AT(int32_t, self, 0x8c0) = 0;

    CSF_AT(uint32_t, self, 0x8f8) = 0xFFFFFFFF;
    CSF_AT(uint32_t, self, 0x8fc) = 255;
    CSF_AT(float, self, 0x900) = 1.0f;
    CSF_AT(float, self, 0x904) = -10000.0f;
    AppendEnumName(base + 0x8e8, CSF_STR(0x008723E0, "LINEAR"), 6);
    AppendEnumName(base + 0x8e8, CSF_STR(0x008723D4, "EXPONENTIAL"), 11);
    AppendEnumName(base + 0x8e8, CSF_STR(0x008723C4, "EXPONENTIAL2"), 12);
    CSF_AT(int32_t, self, 0x8e4) = 0;

    AppendEnumName(base + 0x90c, CSF_STR(0x008723BC, "NONE"), 4);
    AppendEnumName(base + 0x90c, CSF_STR(0x008723B4, "RAIN"), 4);
    AppendEnumName(base + 0x90c, CSF_STR(0x008723AC, "SNOW"), 4);

    CSF_AT(float, self, 0xd60) = -490.0f;          // RTS origin
    CSF_AT(uint32_t, self, 0xd64) = 0;
    CSF_AT(float, self, 0xd68) = 3509.0f;
    CSF_AT(float, self, 0xd6c) = 550.0f;           // RTS destination
    CSF_AT(uint32_t, self, 0xd70) = 0;
    CSF_AT(float, self, 0xd74) = 4363.0f;
    CSF_AT(uint32_t, self, 0xd7c) = 0x44AFA080;    // tactical map origin
    CSF_AT(uint32_t, self, 0xd80) = 0xC61C5A75;
    CSF_AT(uint32_t, self, 0xd84) = 0x44CEBC98;
    CSF_AT(uint32_t, self, 0xd88) = 0x46A72A08;    // tactical map destination
    CSF_AT(uint32_t, self, 0xd8c) = 0x461C258B;
    CSF_AT(int32_t, self, 0x908) = 0;
    CSF_AT(uint8_t, self, 0x91c) = 0;
    CSF_AT(uint8_t, self, 0x91e) = 0;
    CSF_AT(float, self, 0x920) = 10000.0f;
    CSF_AT(float, self, 0xd78) = 10000.0f;
    CSF_AT(uint32_t, self, 0xd90) = 0x46A91CCA;

    AssignStringRange_429B60(base + 0x1a48, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    AssignStringRange_429B60(base + 0x1a64, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);

    CSF_AT(uint8_t, self, 0x1ed8) = 0;
    CSF_AT(uint8_t, self, 0x1ed9) = 0;
    CSF_AT(uint8_t, self, 0x1eda) = 0;
    CSF_AT(int32_t, self, 0x1edc) = -1;
    CSF_AT(uint8_t, self, 0x1ee0) = 0;
    CSF_AT(uint8_t, self, 0x1ee1) = 0;
    CSF_AT(uint32_t, self, 0x1ee4) = 0;
    CSF_AT(uint32_t, self, 0x1ee8) = 0;
    CSF_AT(uint8_t, self, 0x1efc) = 1;
    CSF_AT(uint8_t, self, 0x1efd) = 0;
    CSF_AT(uint8_t, self, 0x1efe) = 0;
    CSF_AT(uint8_t, self, 0x91d) = 0;

    RegisterField(self, CSF_STR(0x008723A0, ".PlayerID"), &self->field_0010, 0x4000);
    RegisterField(self, CSF_STR(0x00872394, ".SkyFile"), base + 0x18, 512);
    RegisterField(self, CSF_STR(0x00872388, ".SkySpeed1"), base + 0x454, 32);
    RegisterField(self, CSF_STR(0x0087237C, ".SkySpeed2"), base + 0x458, 32);
    RegisterField(self, CSF_STR(0x00872370, ".SkySpeed3"), base + 0x45c, 32);
    RegisterField(self, CSF_STR(0x00872360, ".SkyModulate2X"), base + 0x460, 2);
    RegisterField(self, CSF_STR(0x00872350, ".AmbientSound"), &self->field_0014, 0x8000);
    RegisterField(self, CSF_STR(0x0087233C, ".InvertirNorteSur"), base + 0x1ee1, 2);
    RegisterField(self, CSF_STR(0x00872330, ".Modulate2X"), base + 0x8a0, 2);
    RegisterField(self, CSF_STR(0x00872324, ".AmbientAdd"), base + 0x8ac, 32);
    RegisterField(self, CSF_STR(0x00872314, ".ColorAmbEscena"), base + 0x8a4, 0x20000);
    RegisterField(self, CSF_STR(0x00872304, ".ColorAmbiente"), base + 0x8a8, 0x20000);
    RegisterField(self, CSF_STR(0x008722F8, ".CameraNear"), base + 0x8b0, 32);
    RegisterField(self, CSF_STR(0x008722EC, ".CameraFar"), base + 0x8b4, 32);
    RegisterField(self, CSF_STR(0x008722E0, ".CameraFOV"), base + 0x8b8, 32);
    RegisterField(self, CSF_STR(0x008722D4, ".FogEnabled"), base + 0x8bc, 2);
    RegisterField(self, CSF_STR(0x008722C8, ".FogType"), base + 0x8c0, 4096);
    RegisterField(self, CSF_STR(0x008722BC, ".FogColor"), base + 0x8d4, 0x20000);
    RegisterField(self, CSF_STR(0x008722B0, ".FogAlpha"), base + 0x8d8, 8);
    RegisterField(self, CSF_STR(0x008722A4, ".FogDensity"), base + 0x8dc, 32);
    RegisterField(self, CSF_STR(0x00872294, ".FogDistance"), base + 0x8e0, 32);
    RegisterField(self, CSF_STR(0x00872284, ".FogWaterType"), base + 0x8e4, 4096);
    RegisterField(self, CSF_STR(0x00872274, ".FogWaterColor"), base + 0x8f8, 0x20000);
    RegisterField(self, CSF_STR(0x00872264, ".FogWaterAlpha"), base + 0x8fc, 8);
    RegisterField(self, CSF_STR(0x00872250, ".FogWaterDensity"), base + 0x900, 32);
    RegisterField(self, CSF_STR(0x0087223C, ".FogWaterDistance"), base + 0x904, 32);
    RegisterField(self, CSF_STR(0x0087222C, ".AtmosfeticFX"), base + 0x908, 4096);
    RegisterField(self, CSF_STR(0x0087221C, ".VahoEnabled"), base + 0x91c, 2);
    RegisterField(self, CSF_STR(0x00872208, ".EliminarMuertos"), base + 0x91d, 2);
    RegisterField(self, CSF_STR(0x008721F8, ".Simplificado"), base + 0x464, 512);
    RegisterField(self, CSF_STR(0x008721E8, ".MiniMapaRTS"), base + 0x91e, 2);
    RegisterField(self, CSF_STR(0x008721DC, ".RadioRTS"), base + 0x920, 32);
    RegisterField(self, CSF_STR(0x008721D0, ".TexturaRTS"), base + 0x924, 512);
    RegisterField(self, CSF_STR(0x008721C4, ".RTSOrigen"), base + 0xd60, 256);
    RegisterField(self, CSF_STR(0x008721B8, ".RTSDestino"), base + 0xd6c, 256);
    RegisterField(self, CSF_STR(0x008721A4, ".RadioMiniMapaMP"), base + 0xd78, 32);
    RegisterField(self, CSF_STR(0x00872190, ".MapaTacticoOrigen"), base + 0xd7c, 256);
    RegisterField(self, CSF_STR(0x0087217C, ".MapaTacticoDestino"), base + 0xd88, 256);
    RegisterField(self, CSF_STR(0x00872168, ".MapaTacticoTextura"), base + 0xd94, 512);
    RegisterField(self, CSF_STR(0x00872158, ".FlisEscenario"), base + 0x11d0, 512);
    RegisterField(self, CSF_STR(0x00872148, ".MapaSectores"), base + 0x160c, 512);
    RegisterField(self, CSF_STR(0x00872138, ".SaveGameDescr"), base + 0x1a48, 0x200000);
    RegisterField(self, CSF_STR(0x00872128, ".SaveGameScene"), base + 0x1a64, 0x200000);
    RegisterField(self, CSF_STR(0x00872118, ".VideoOpening"), base + 0x1a80, 512);
    RegisterField(self, CSF_STR(0x0087210C, ".PunteriaID"), base + 0x1ebc, 64);
    RegisterField(self, CSF_STR(0x008720FC, ".MPDeathMatch"), base + 0x1ed8, 2);
    RegisterField(self, CSF_STR(0x008720EC, ".MPTDeathMatch"), base + 0x1ed9, 2);
    RegisterField(self, CSF_STR(0x008720E0, ".MPPostmen"), base + 0x1eda, 2);
    RegisterField(self, CSF_STR(0x008720C8, ".MPPostmen_IDPostman"), base + 0x1edc, 0x40000);
    RegisterField(self, CSF_STR(0x008720BC, ".MPFrancos"), base + 0x1ee0, 2);
    RegisterField(self, CSF_STR(0x008720A8, ".PuntuacionMaxima"), base + 0x1ee4, 16);
    RegisterField(self, CSF_STR(0x00872094, ".PuntuacionMinima"), base + 0x1ee8, 16);
    RegisterField(self, CSF_STR(0x0087207C, ".PlayerInicioCommando"), base + 0x1efc, 2);
    RegisterField(self, CSF_STR(0x00872064, ".PlayerInicioSnipper"), base + 0x1efd, 2);
    RegisterField(self, CSF_STR(0x00872050, ".PlayerInicioSpy"), base + 0x1efe, 2);

    return self;
}

HOOK(0x007770C0, GameDataDefs_ConstructSceneDefs, "auto");
