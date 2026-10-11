#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int, __cdecl, DataDefs_CopyFieldRecords, 0x0055AB00, (int a1, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, DataDefs_InsertFieldRecords, 0x0055ABF0, (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));
CSF_FUNC(unsigned int, __fastcall, DataDefs_RegisterField, 0x0055AF00, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char*, __cdecl, GameConfig_GetDifficultyName, 0x007614A0, (int a1));

// One entry of the field table kept at +4/+8/+C of the config object (16 bytes each).
struct GameConfigFieldDesc
{
    const char* name;
    void*       target;
    uint32_t    reserved;
    uint32_t    type;
};

// Builds a temporary MSVC 7.1 std::string (28 bytes: buffer +4, size +0x14, capacity +0x18)
// and appends it to an option-name list.
static void GameConfig_AppendOptionName(void* optionList, const char* text, unsigned int length)
{
    uint32_t tempString[7] = {};
    CSF_AT(uint32_t, tempString, 0x18) = 15;
    CSF_AT(uint32_t, tempString, 0x14) = 0;
    CSF_AT(uint8_t, tempString, 0x04) = 0;

    AssignStringRange_429B60(tempString, nullptr, const_cast<char*>(text), length);
    MenuManager_AppendPageName(reinterpret_cast<S_00470ab0*>(optionList), nullptr, tempString);

    if (CSF_AT(uint32_t, tempString, 0x18) >= 0x10 && CSF_AT(uint32_t, tempString, 0x04))
        Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, tempString, 0x04))));
}

// The first registrations have the vector push_back inlined in the original.
static void GameConfig_PushField(S_0055af00* self, const char* name, void* target, uint32_t type)
{
    GameConfigFieldDesc desc;
    desc.name = name;
    desc.target = target;
    desc.reserved = 0;
    desc.type = type;

    const uint32_t begin = self->field_0004;
    unsigned int count;
    if (begin)
        count = static_cast<unsigned int>(static_cast<int>(self->field_0008 - begin) >> 4);
    else
        count = 0;

    if (begin && count < static_cast<unsigned int>(static_cast<int>(self->field_000c - begin) >> 4))
    {
        const uint32_t end = self->field_0008;
        DataDefs_CopyFieldRecords(static_cast<int>(end), 1, static_cast<int>(reinterpret_cast<uintptr_t>(&desc)));
        self->field_0008 = end + 16;
    }
    else
    {
        DataDefs_InsertFieldRecords(self, nullptr,
            reinterpret_cast<char*>(static_cast<uintptr_t>(self->field_0008)), 1,
            reinterpret_cast<uint32_t*>(&desc));
    }
}

static void GameConfig_AddField(S_0055af00* self, const char* name, void* target, uint32_t type)
{
    GameConfigFieldDesc desc;
    desc.name = name;
    desc.target = target;
    desc.reserved = 0;
    desc.type = type;
    DataDefs_RegisterField(self, nullptr, reinterpret_cast<uint32_t*>(&desc));
}

static S_0055af00* __fastcall S_0055af00_InitGameConfig(S_0055af00* self, void* /*edx*/)
{
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_0090 = -1;
    self->field_0124 = 0;
    self->field_0128 = 0;
    self->field_012c = 0;
    self->field_011c = -1;
    self->field_01bc = 0;
    self->field_01c0 = 0;
    self->field_01c4 = 0;
    self->field_01c8 = 0;
    self->field_0218 = 0;
    self->field_021c = 0;
    self->field_0220 = 0;
    self->field_0224 = 0;

    // Float defaults; the fields are declared uint32_t, so the raw IEEE bit patterns are stored.
    self->field_0018 = 1135869952;   // 360.0f
    self->field_001c = 1135869952;
    self->field_0020 = 1110704128;   // 45.0f
    self->field_0024 = 1110704128;
    self->field_0028 = 1119092736;   // 90.0f
    self->field_002c = 1119092736;
    self->field_0010 = 1056964608;   // 0.5f
    self->field_0014 = 0;
    self->field_0030 = 0x3F666666;   // 0.9f
    self->field_0034 = 0x3F666666;
    self->field_0038 = 1127481344;   // 180.0f
    self->field_003c = 1127481344;
    self->field_0040 = 1065353216;   // 1.0f
    self->field_0044 = 1065353216;
    self->field_0048 = 1065353216;
    self->field_004c = 1065353216;
    self->field_0058 = 1110704128;
    self->field_005c = 1110704128;
    self->field_0060 = 1110704128;
    self->field_0064 = 1110704128;
    self->field_0068 = 1110704128;
    self->field_006c = 1110704128;
    self->field_0070 = 1065353216;
    self->field_0074 = 1065353216;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 1065353216;
    self->field_0050 = 1065353216;
    self->field_0054 = 0;
    self->field_0084 = 1065353216;
    self->field_0088 = 1065353216;
    self->field_008c = 1065353216;

    for (int difficulty = 0; difficulty < 4; ++difficulty)
    {
        const char* difficultyName = GameConfig_GetDifficultyName(difficulty);
        GameConfig_AppendOptionName(&self->field_0094, difficultyName,
                                    static_cast<unsigned int>(std::strlen(difficultyName)));
    }

    self->field_00e4 = 1065353216;
    self->field_00e8 = 1065353216;
    self->field_0090 = 0;
    self->field_00a4 = 1069547520;   // 1.5f
    self->field_00a8 = 1056964608;
    self->field_00ac = 1056964608;
    self->field_00b0 = 1056964608;
    self->field_00b4 = 1069547520;
    self->field_00b8 = 1069547520;
    self->field_00bc = 1056964608;
    self->field_00c0 = 1069547520;
    self->field_00c4 = 1069547520;
    self->field_00c8 = 1069547520;
    self->field_00cc = 1056964608;
    self->field_00d0 = 1056964608;
    self->field_00d4 = 1056964608;
    self->field_00d8 = 1069547520;
    self->field_00dc = 1069547520;
    self->field_00e0 = 0x40000000;   // 2.0f
    self->field_00ec = 0x40000000;
    self->field_00f0 = 1056964608;
    self->field_00f4 = 1056964608;
    self->field_00f8 = 1056964608;
    self->field_00fc = 0x40000000;
    self->field_0100 = 0x40000000;
    self->field_0104 = 1135869952;
    self->field_0108 = 1114636288;   // 60.0f
    self->field_010c = 1106247680;   // 30.0f
    self->field_0110 = 1092616192;   // 10.0f
    CSF_AT(uint8_t, self, 0x114) = 0;
    CSF_AT(uint8_t, self, 0x115) = 0;
    self->field_0118 = 0;

    GameConfig_AppendOptionName(&self->field_0120, CSF_STR(0x00869C38, "BILINEAL SIN MIPS"), 17);
    GameConfig_AppendOptionName(&self->field_0120, CSF_STR(0x00869C24, "BILINEAL CON MIPS"), 17);
    GameConfig_AppendOptionName(&self->field_0120, CSF_STR(0x00869C18, "TRILINEAL"), 9);

    self->field_011c = 1;
    CSF_AT(uint8_t, self, 0x131) = 0;
    CSF_AT(uint8_t, self, 0x130) = 1;
    self->field_0134 = 1115815936;   // 65.0f
    self->field_0154 = 1133903872;   // 300.0f
    self->field_0158 = 1133903872;
    self->field_015c = 1133903872;
    self->field_0160 = 1048576000;   // 0.25f
    self->field_0164 = 1048576000;
    self->field_0168 = 1048576000;
    self->field_0170 = 1056964608;
    self->field_0174 = 1056964608;
    self->field_0178 = 1056964608;
    self->field_017c = 1140457472;   // 500.0f
    self->field_0180 = 1133903872;
    self->field_0184 = 1133903872;
    self->field_0188 = 1133903872;
    self->field_018c = 1048576000;
    self->field_0190 = 1048576000;
    self->field_0194 = 1048576000;
    self->field_019c = 1056964608;
    self->field_01a0 = 1056964608;
    self->field_01a4 = 1056964608;
    self->field_01a8 = 1140457472;
    self->field_01ac = 1065353216;
    self->field_01b0 = 1065353216;
    self->field_01b4 = 8;
    self->field_0138 = 1128792064;   // 200.0f
    self->field_013c = 1128792064;
    self->field_0140 = 1045220557;   // 0.2f
    self->field_0144 = 1053609165;   // 0.4f
    self->field_0148 = 957533431;
    self->field_014c = 1097859072;   // 15.0f
    self->field_0150 = 1097859072;
    self->field_01cc = 1041026253;
    self->field_01d0 = 1035712573;
    self->field_01d4 = 640;
    self->field_01d8 = 640;
    self->field_01dc = 640;
    self->field_016c = 0x40000000;
    self->field_0198 = 0x40000000;
    self->field_01bc = 1031798784;
    self->field_01c0 = 1034590513;
    self->field_01c4 = 1040187392;
    self->field_01c8 = 1033504467;
    self->field_01e0 = 0;
    self->field_01e1 = 0;
    self->field_01e4 = 500;
    self->field_01e8 = 10;
    self->field_01ec = 200;
    self->field_01f0 = 10;
    self->field_01f4 = 5;
    self->field_01f8 = 1065353216;
    self->field_01fc = 32;
    self->field_0200 = 0;
    self->field_0204 = 256;
    self->field_0208 = 0;
    self->field_020c = 1045220557;
    self->field_0210 = 1048576000;
    self->field_0214 = 0;
    self->field_0218 = 1036831949;   // 0.1f
    self->field_021c = 0x40000000;
    self->field_0220 = 1036831949;
    self->field_0224 = 0x40000000;
    self->field_0228 = 1065353216;
    self->field_01b8 = 1056964608;

    // Aiming ("apuntado") tuning.
    GameConfig_PushField(self, CSF_STR(0x00869C08, ".bAmbientWhite"), &self->field_01e1, 2);
    GameConfig_PushField(self, CSF_STR(0x00869BEC, ".fApuntadoFactorPegajoso"), &self->field_0010, 32);
    GameConfig_PushField(self, CSF_STR(0x00869BD0, ".fApuntadoAnguloPegajoso"), &self->field_0014, 32);
    GameConfig_PushField(self, CSF_STR(0x00869BB4, ".vApuntadoVelocidadMaxima"), &self->field_0018, 128);
    GameConfig_PushField(self, CSF_STR(0x00869B98, ".vApuntadoVelocidadLenta"), &self->field_0020, 128);
    GameConfig_PushField(self, CSF_STR(0x00869B7C, ".vApuntadoVelocidadRapida"), &self->field_0028, 128);
    GameConfig_PushField(self, CSF_STR(0x00869B60, ".vApuntadoCambioVelocidad"), &self->field_0030, 128);
    GameConfig_PushField(self, CSF_STR(0x00869B48, ".vApuntadoAceleracion"), &self->field_0038, 128);
    GameConfig_PushField(self, CSF_STR(0x00869B2C, ".vApuntadoExpVelocidadLenta"), &self->field_0040, 128);
    GameConfig_PushField(self, CSF_STR(0x00869B0C, ".vApuntadoExpVelocidadRapida"), &self->field_0048, 128);
    GameConfig_PushField(self, CSF_STR(0x00869AEC, ".fApuntadoZoomFactorPegajoso"), &self->field_0050, 32);
    GameConfig_PushField(self, CSF_STR(0x00869ACC, ".fApuntadoZoomAnguloPegajoso"), &self->field_0054, 32);
    GameConfig_PushField(self, CSF_STR(0x00869AAC, ".vApuntadoZoomVelocidadMaxima"), &self->field_0058, 128);
    GameConfig_PushField(self, CSF_STR(0x00869A8C, ".vApuntadoZoomVelocidadLenta"), &self->field_0060, 128);
    GameConfig_PushField(self, CSF_STR(0x00869A6C, ".vApuntadoZoomVelocidadRapida"), &self->field_0068, 128);
    GameConfig_PushField(self, CSF_STR(0x00869A4C, ".vApuntadoZoomCambioVelocidad"), &self->field_0070, 128);
    GameConfig_PushField(self, CSF_STR(0x00869A30, ".vApuntadoZoomAceleracion"), &self->field_0078, 128);
    GameConfig_PushField(self, CSF_STR(0x00869A10, ".vApuntadoZoomExpVelocidadLenta"), &self->field_0080, 128);
    GameConfig_PushField(self, CSF_STR(0x008699EC, ".vApuntadoZoomExpVelocidadRapida"), &self->field_0088, 128);

    // Difficulty tuning.
    GameConfig_AddField(self, CSF_STR(0x008699D8, ".initialDificulty"), &self->field_0090, 4096);
    GameConfig_AddField(self, CSF_STR(0x008699B8, ".fDificultadVidaJugadorFacil"), &self->field_00a4, 32);
    GameConfig_AddField(self, CSF_STR(0x00869998, ".fDificultadVidaJugadorDificil"), &self->field_00a8, 32);
    GameConfig_AddField(self, CSF_STR(0x00869978, ".fDificultadVidaJugadorComando"), &self->field_00ac, 32);
    GameConfig_AddField(self, CSF_STR(0x00869958, ".fDificultadVidaEnemigoFacil"), &self->field_00b0, 32);
    GameConfig_AddField(self, CSF_STR(0x00869938, ".fDificultadVidaEnemigoDificil"), &self->field_00b4, 32);
    GameConfig_AddField(self, CSF_STR(0x00869918, ".fDificultadVidaEnemigoComando"), &self->field_00b8, 32);
    GameConfig_AddField(self, CSF_STR(0x008698F8, ".fDificultadArmaEnemigoFacil"), &self->field_00bc, 32);
    GameConfig_AddField(self, CSF_STR(0x008698D8, ".fDificultadArmaEnemigoDificil"), &self->field_00c0, 32);
    GameConfig_AddField(self, CSF_STR(0x008698B8, ".fDificultadArmaEnemigoComando"), &self->field_00c4, 32);
    GameConfig_AddField(self, CSF_STR(0x00869894, ".fDificultadReaccionEnemigoFacil"), &self->field_00c8, 32);
    GameConfig_AddField(self, CSF_STR(0x00869870, ".fDificultadReaccionEnemigoDificil"), &self->field_00cc, 32);
    GameConfig_AddField(self, CSF_STR(0x0086984C, ".fDificultadReaccionEnemigoComando"), &self->field_00d0, 32);
    GameConfig_AddField(self, CSF_STR(0x00869824, ".fDificultadPuntuacionConseguidaFacil"), &self->field_00d4, 32);
    GameConfig_AddField(self, CSF_STR(0x008697FC, ".fDificultadPuntuacionConseguidaDificil"), &self->field_00d8, 32);
    GameConfig_AddField(self, CSF_STR(0x008697D4, ".fDificultadPuntuacionConseguidaComando"), &self->field_00dc, 32);
    GameConfig_AddField(self, CSF_STR(0x008697BC, ".fDificultadBalasFacil"), &self->field_00e0, 32);
    GameConfig_AddField(self, CSF_STR(0x008697A0, ".fDificultadBalasDificil"), &self->field_00e4, 32);
    GameConfig_AddField(self, CSF_STR(0x00869784, ".fDificultadBalasComando"), &self->field_00e8, 32);
    GameConfig_AddField(self, CSF_STR(0x0086975C, ".fDificultadRecuperacionEnergiaFacil"), &self->field_00ec, 32);
    GameConfig_AddField(self, CSF_STR(0x00869734, ".fDificultadRecuperacionEnergiaDificil"), &self->field_00f0, 32);
    GameConfig_AddField(self, CSF_STR(0x0086970C, ".fDificultadRecuperacionEnergiaComando"), &self->field_00f4, 32);
    GameConfig_AddField(self, CSF_STR(0x008696E4, ".fDificultadDispersionMinimaArmasFacil"), &self->field_00f8, 32);
    GameConfig_AddField(self, CSF_STR(0x008696B8, ".fDificultadDispersionMinimaArmasDificil"), &self->field_00fc, 32);
    GameConfig_AddField(self, CSF_STR(0x0086968C, ".fDificultadDispersionMinimaArmasComando"), &self->field_0100, 32);
    GameConfig_AddField(self, CSF_STR(0x00869670, ".fTiempoLanzarGranadasFacil"), &self->field_0104, 32);
    GameConfig_AddField(self, CSF_STR(0x00869650, ".fTiempoLanzarGranadasNormal"), &self->field_0108, 32);
    GameConfig_AddField(self, CSF_STR(0x00869630, ".fTiempoLanzarGranadasDificil"), &self->field_010c, 32);
    GameConfig_AddField(self, CSF_STR(0x00869610, ".fTiempoLanzarGranadasComando"), &self->field_0110, 32);

    // Input, effects, physics, language menu.
    GameConfig_AddField(self, CSF_STR(0x008695FC, ".vTurnSpeedLimits"), &self->field_0218, 128);
    GameConfig_AddField(self, CSF_STR(0x008695E8, ".vPitchSpeedLimits"), &self->field_0220, 128);
    GameConfig_AddField(self, CSF_STR(0x008695D4, ".DecalsMaxNumber"), &self->field_01e4, 16);
    GameConfig_AddField(self, CSF_STR(0x008695C4, ".VahoEffectMax"), &self->field_01e8, 16);
    GameConfig_AddField(self, CSF_STR(0x008695B0, ".fImpactForceMult"), &self->field_0138, 32);
    GameConfig_AddField(self, CSF_STR(0x00869598, ".fExplosionForceMult"), &self->field_013c, 32);
    GameConfig_AddField(self, CSF_STR(0x00869588, ".uMaxPhysicObjs"), &self->field_01ec, 16);
    GameConfig_AddField(self, CSF_STR(0x00869570, ".uMaxPhysicObjActive"), &self->field_01f0, 16);
    GameConfig_AddField(self, CSF_STR(0x00869560, ".uMaxRagdolls"), &self->field_01f4, 16);
    GameConfig_AddField(self, CSF_STR(0x00869550, ".fWorldBounce"), &self->field_0140, 32);
    GameConfig_AddField(self, CSF_STR(0x00869540, ".fWorldSlide"), &self->field_0144, 32);
    GameConfig_AddField(self, CSF_STR(0x00869530, ".fWaterDensity"), &self->field_0148, 32);
    GameConfig_AddField(self, CSF_STR(0x00869520, ".fWaterFLinear"), &self->field_014c, 32);
    GameConfig_AddField(self, CSF_STR(0x00869510, ".fWaterFAngular"), &self->field_0150, 32);
    GameConfig_AddField(self, CSF_STR(0x00869500, ".langInitialPos"), &self->field_01bc, 128);
    GameConfig_AddField(self, CSF_STR(0x008694F4, ".langSize"), &self->field_01c4, 128);
    GameConfig_AddField(self, CSF_STR(0x008694E4, ".langSpaceWidth"), &self->field_01cc, 32);
    GameConfig_AddField(self, CSF_STR(0x008694D0, ".langSpaceHeight"), &self->field_01d0, 32);
    GameConfig_AddField(self, CSF_STR(0x008694B8, ".fMarcaDanyoReescalado"), &self->field_0228, 32);
    GameConfig_AddField(self, CSF_STR(0x008694A8, ".fAxisSensMenus"), &self->field_01f8, 32);
    GameConfig_AddField(self, CSF_STR(0x00869494, ".MultiplayerIsLAN"), &self->field_01e0, 2);

    // Video.
    GameConfig_AddField(self, CSF_STR(0x00869484, ".bFullScreen"), &self->field_0114, 2);
    GameConfig_AddField(self, CSF_STR(0x00869474, ".bAntialiasing"), &CSF_AT(uint8_t, self, 0x115), 2);
    GameConfig_AddField(self, CSF_STR(0x00869464, ".iAnisotropic"), &self->field_0118, 8);
    GameConfig_AddField(self, CSF_STR(0x00869458, ".Filtrado"), &self->field_011c, 4096);
    GameConfig_AddField(self, CSF_STR(0x00869444, ".bMaxRefreshRate"), &self->field_0130, 2);
    GameConfig_AddField(self, CSF_STR(0x00869438, ".bWaitVsync"), &CSF_AT(uint8_t, self, 0x131), 2);
    GameConfig_AddField(self, CSF_STR(0x00869430, ".fFOV"), &self->field_0134, 32);
    GameConfig_AddField(self, CSF_STR(0x00869420, ".screenDepth"), &self->field_01fc, 8);

    // Single-player movement.
    GameConfig_AddField(self, CSF_STR(0x00869408, ".fPlayerForwardSpeed"), &self->field_0154, 32);
    GameConfig_AddField(self, CSF_STR(0x008693F4, ".fPlayerBackSpeed"), &self->field_0158, 32);
    GameConfig_AddField(self, CSF_STR(0x008693E0, ".fPlayerStrafeSpeed"), &self->field_015c, 32);
    GameConfig_AddField(self, CSF_STR(0x008693C8, ".fPlayerForwardInertia"), &self->field_0160, 32);
    GameConfig_AddField(self, CSF_STR(0x008693B4, ".fPlayerBackInertia"), &self->field_0164, 32);
    GameConfig_AddField(self, CSF_STR(0x0086939C, ".fPlayerStrafeInertia"), &self->field_0168, 32);
    GameConfig_AddField(self, CSF_STR(0x00869388, ".fPlayerRunFactor"), &self->field_016c, 32);
    GameConfig_AddField(self, CSF_STR(0x00869374, ".fPlayerCrawlFactor"), &self->field_0170, 32);
    GameConfig_AddField(self, CSF_STR(0x00869360, ".fPlayerSwimFactor"), &self->field_0174, 32);
    GameConfig_AddField(self, CSF_STR(0x0086934C, ".fPlayerAimFactor"), &self->field_0178, 32);
    GameConfig_AddField(self, CSF_STR(0x00869338, ".PlayerJumpForce"), &self->field_017c, 32);
    GameConfig_AddField(self, CSF_STR(0x00869318, ".fPlayerVistaApuntarPrecision"), &self->field_01ac, 32);

    // Multiplayer movement.
    GameConfig_AddField(self, CSF_STR(0x00869300, ".fMPPlayerForwardSpeed"), &self->field_0180, 32);
    GameConfig_AddField(self, CSF_STR(0x008692EC, ".fMPPlayerBackSpeed"), &self->field_0184, 32);
    GameConfig_AddField(self, CSF_STR(0x008692D4, ".fMPPlayerStrafeSpeed"), &self->field_0188, 32);
    GameConfig_AddField(self, CSF_STR(0x008692B8, ".fMPPlayerForwardInertia"), &self->field_018c, 32);
    GameConfig_AddField(self, CSF_STR(0x008692A0, ".fMPPlayerBackInertia"), &self->field_0190, 32);
    GameConfig_AddField(self, CSF_STR(0x00869288, ".fMPPlayerStrafeInertia"), &self->field_0194, 32);
    GameConfig_AddField(self, CSF_STR(0x00869274, ".fMPPlayerRunFactor"), &self->field_0198, 32);
    GameConfig_AddField(self, CSF_STR(0x0086925C, ".fMPPlayerCrawlFactor"), &self->field_019c, 32);
    GameConfig_AddField(self, CSF_STR(0x00869244, ".fMPPlayerSwimFactor"), &self->field_01a0, 32);
    GameConfig_AddField(self, CSF_STR(0x00869230, ".fMPPlayerAimFactor"), &self->field_01a4, 32);
    GameConfig_AddField(self, CSF_STR(0x0086921C, ".MPPlayerJumpForce"), &self->field_01a8, 32);
    GameConfig_AddField(self, CSF_STR(0x008691FC, ".fMPPlayerVistaApuntarPrecision"), &self->field_01b0, 32);

    // Platform / misc.
    GameConfig_AddField(self, CSF_STR(0x008691EC, ".PS2GameSize"), &self->field_01d4, 536870920);
    GameConfig_AddField(self, CSF_STR(0x008691DC, ".PS2OptionsSize"), &self->field_01d8, 536870920);
    GameConfig_AddField(self, CSF_STR(0x008691CC, ".PS2ProfileSize"), &self->field_01dc, 536870920);
    GameConfig_AddField(self, CSF_STR(0x008691BC, ".iShadowsMax"), &self->field_0200, 8);
    GameConfig_AddField(self, CSF_STR(0x008691A8, ".iShadowsTexSize"), &self->field_0204, 8);
    GameConfig_AddField(self, CSF_STR(0x00869194, ".iNoiseIntensity"), &self->field_0208, 8);
    GameConfig_AddField(self, CSF_STR(0x00869184, ".MemoriaSonido"), &self->field_01b4, 8);
    GameConfig_AddField(self, CSF_STR(0x00869170, ".fAlturaBandaCutScn"), &self->field_020c, 32);
    GameConfig_AddField(self, CSF_STR(0x0086915C, ".fAlturaSubtitulos"), &self->field_0210, 32);
    GameConfig_AddField(self, CSF_STR(0x0086914C, ".bTrazarNetZ"), &self->field_0214, 2);
    GameConfig_AddField(self, CSF_STR(0x00869134, ".fBarraProgresoLatencia"), &self->field_01b8, 32);

    return self;
}

HOOK(0x00753B40, S_0055af00_InitGameConfig, "auto");
