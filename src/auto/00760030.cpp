#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int, __cdecl, MpRules_CopySixteenByteRecords, 0x0055AB00, (int destination, int count, int source, int context));
CSF_FUNC(unsigned int, __fastcall, DataDefs_InsertFieldRecords, 0x0055ABF0, (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));
CSF_FUNC(unsigned int, __fastcall, DataDefs_RegisterField, 0x0055AF00, (S_0055af00* self, void* , uint32_t* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, AppConfig_GetLanguageName, 0x0069C3C0, (int a1));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void AppConfig_SetFieldRecord(
    uint32_t* record, const char* name, void* field, uint32_t type)
{
    record[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(name));
    record[1] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(field));
    record[2] = 0;
    record[3] = type;
    record[4] = 0;
}

static void AppConfig_AppendField(
    S_0055af00* self, uint32_t* record,
    const char* name, void* field, uint32_t type)
{
    const uint32_t begin = self->field_0004;
    AppConfig_SetFieldRecord(record, name, field, type);
    const uint32_t size = begin
        ? static_cast<uint32_t>(static_cast<int32_t>(self->field_0008 - begin) >> 4)
        : 0;
    if (begin &&
        size < static_cast<uint32_t>(
            static_cast<int32_t>(self->field_000c - begin) >> 4))
    {
        const uint32_t end = self->field_0008;
        MpRules_CopySixteenByteRecords(
            static_cast<int>(end), 1,
            static_cast<int>(reinterpret_cast<uintptr_t>(record)),
            static_cast<int>(reinterpret_cast<uintptr_t>(self)));
        self->field_0008 = end + 16;
    }
    else
    {
        DataDefs_InsertFieldRecords(
            self, nullptr, reinterpret_cast<char*>(self->field_0008), 1, record);
    }
}

static unsigned int __fastcall S_0055af00_InitializeAppConfig(
    S_0055af00* self, void* /*edx*/)
{
    // This reported vtable address is actually the direct RegisterField target.
    (void)CSF_VTBL(0x0055AF00);

    reinterpret_cast<uint8_t*>(&self->field_0010)[0] = 1;
    AssignStringRange_429B60(
        &self->field_0014, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);

    // The temporary string spans the split locals in the original frame.
    alignas(8) uint8_t languageBytes[0x20] = {};
    C_0082d410& language = *reinterpret_cast<C_0082d410*>(languageBytes);
    for (unsigned int index = 0; index < 0xF; ++index)
    {
        const char* name = reinterpret_cast<const char*>(AppConfig_GetLanguageName(index));
        CSF_AT(uint32_t, languageBytes, 0x18) = 15;
        CSF_AT(uint32_t, languageBytes, 0x14) = 0;
        reinterpret_cast<uint8_t*>(&language.field_0004)[0] = 0;
        AssignStringRange_429B60(
            &language, nullptr, const_cast<char*>(name),
            static_cast<unsigned int>(std::strlen(name)));
        MenuManager_AppendPageName(
            reinterpret_cast<S_00470ab0*>(&self->field_0034), nullptr, &language);
        if (CSF_AT(uint32_t, languageBytes, 0x18) >= 0x10 &&
            language.field_0004)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(language.field_0004));
        }
        CSF_AT(uint32_t, languageBytes, 0x18) = 15;
        CSF_AT(uint32_t, languageBytes, 0x14) = 0;
        reinterpret_cast<uint8_t*>(&language.field_0004)[0] = 0;
    }

    self->field_0030 = 11;
    reinterpret_cast<uint8_t*>(&self->field_0044)[0] = 0;
    self->field_0048 = 0;
    reinterpret_cast<uint8_t*>(&self->field_004c)[0] = 1;
    AssignStringRange_429B60(
        &self->field_0050, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    AssignStringRange_429B60(
        &self->field_006c, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
    self->field_0088 = 0;
    AssignStringRange_429B60(
        &self->field_008c, nullptr,
        const_cast<char*>(CSF_STR(0x0086C560, "BESLES-00000")), 12);
    self->field_00a8 = 1145569280;
    self->field_00ac = 1142292480;
    self->field_00b0 = 1142947840;
    self->field_00b4 = 1139802112;
    reinterpret_cast<uint8_t*>(&self->field_00b8)[0] = 1;
    reinterpret_cast<uint8_t*>(&self->field_00b8)[1] = 1;
    self->field_00bc = 0;
    self->field_00c0 = 0;
    reinterpret_cast<uint8_t*>(&self->field_00c4)[0] = 1;
    reinterpret_cast<uint8_t*>(&self->field_00c4)[1] = 1;
    reinterpret_cast<uint8_t*>(&self->field_00c4)[2] = 0;
    reinterpret_cast<uint8_t*>(&self->field_00c4)[3] = 0;
    reinterpret_cast<uint8_t*>(&self->field_00c8)[0] = 1;
    reinterpret_cast<uint8_t*>(&self->field_00c8)[1] = 1;
    reinterpret_cast<uint8_t*>(&self->field_00c8)[2] = 0;
    reinterpret_cast<uint8_t*>(&self->field_00c8)[3] = 0;
    AssignStringRange_429B60(
        &self->field_00cc, nullptr,
        const_cast<char*>(CSF_STR(0x0086C52C, "Software\\Pyro Studios\\Commandos Strike Force\\Game\\")), 50);
    reinterpret_cast<uint8_t*>(&self->field_00e8)[0] = 1;
    reinterpret_cast<uint8_t*>(&self->field_00e8)[1] = 1;
    AssignStringRange_429B60(
        &self->field_00ec, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);

    void* oldRecords = reinterpret_cast<void*>(self->field_0004);
    reinterpret_cast<uint8_t*>(&self->field_0108)[0] = 0;
    reinterpret_cast<uint8_t*>(&self->field_0108)[1] = 0;
    reinterpret_cast<uint8_t*>(&self->field_0108)[2] = 0;
    reinterpret_cast<uint8_t*>(&self->field_0108)[3] = 0;
    if (oldRecords)
        Mem_FreeDebug(oldRecords);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;

    alignas(8) uint8_t recordBytes[0x14];
    uint32_t* record = reinterpret_cast<uint32_t*>(recordBytes);
    AppConfig_SetFieldRecord(
        record, CSF_STR(0x0086C51C, ".sBootVersion"), &self->field_00ec, 64);
    DataDefs_InsertFieldRecords(self, nullptr, nullptr, 1, record);

    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C508, ".bMenusExtrasActivo"),
        reinterpret_cast<uint8_t*>(&self->field_00c8) + 1, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C4F4, ".bAttractModeIngame"),
        reinterpret_cast<uint8_t*>(&self->field_00c4) + 1, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C4DC, ".bPlayIntroBeforeLoad"),
        reinterpret_cast<uint8_t*>(&self->field_00c4) + 2, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C4C8, ".ServerDNASEnDebug"), &self->field_0010, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C4B4, ".EventoDemoVersion"),
        reinterpret_cast<uint8_t*>(&self->field_00c4) + 3, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C49C, ".ShowVentanaSalidaDemo"), &self->field_00c8, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C480, ".IncluyeVersionSingleplayer"), &self->field_00b8, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C464, ".IncluyeVersionMultiplayer"),
        reinterpret_cast<uint8_t*>(&self->field_00b8) + 1, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C458, ".langList"), &self->field_0014, 64);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C448, ".defaultLang"), &self->field_0030, 4096);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C430, ".bRED_PermitidoModem"), &self->field_0044, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C41C, ".bLoadSaveEnable"), &self->field_00c4, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C40C, ".RENDEZVOUS_URL"), &self->field_0050, 64);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C3F4, ".RENDEZVOUS_PASSWORD"), &self->field_006c, 64);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C3E0, ".RENDEZVOUS_PORT"), &self->field_0088, 16);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C3D0, ".uMaxPlayersMP"), &self->field_0048, 16);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C3C4, ".PS2Code"), &self->field_008c, 536870976);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C3B8, ".resolution"), &self->field_00a8, 128);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C3A8, ".resolutionNTSC"), &self->field_00b0, 128);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C3A0, ".Region"), &self->field_004c, 4);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C38C, ".uPulseDelayTime"), &self->field_00bc, 16);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C370, ".fTiempoPulseDelayActivo"), &self->field_00c0, 32);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C358, ".bMostrarEscenariosAll"),
        reinterpret_cast<uint8_t*>(&self->field_00c8) + 2, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C338, ".bMostrarPantallaSalidaAlInicio"),
        reinterpret_cast<uint8_t*>(&self->field_00c8) + 3, 2);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C328, ".WINDOWSREGPATH"), &self->field_00cc, 64);
    AppConfig_AppendField(self, record,
        CSF_STR(0x0086C31C, ".bCreditos"), &self->field_00e8, 2);

    AppConfig_SetFieldRecord(record,
        CSF_STR(0x0086C30C, ".bNextMission"),
        reinterpret_cast<uint8_t*>(&self->field_00e8) + 1, 2);
    DataDefs_RegisterField(self, nullptr, record);
    AppConfig_SetFieldRecord(record,
        CSF_STR(0x0086C2F4, ".bMarketingExitoMision"), &self->field_0108, 2);
    DataDefs_RegisterField(self, nullptr, record);
    AppConfig_SetFieldRecord(record,
        CSF_STR(0x0086C2E4, ".bUsaFLIDemo"),
        reinterpret_cast<uint8_t*>(&self->field_0108) + 1, 2);
    DataDefs_RegisterField(self, nullptr, record);
    AppConfig_SetFieldRecord(record,
        CSF_STR(0x0086C2D0, ".bMarketingWhenExit"),
        reinterpret_cast<uint8_t*>(&self->field_0108) + 2, 2);
    DataDefs_RegisterField(self, nullptr, record);
    AppConfig_SetFieldRecord(record,
        CSF_STR(0x0086C2C4, ".bRating"),
        reinterpret_cast<uint8_t*>(&self->field_0108) + 3, 2);
    return DataDefs_RegisterField(self, nullptr, record);
}

HOOK(0x00760030, S_0055af00_InitializeAppConfig, "auto");
