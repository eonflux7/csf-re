#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DataDefs_RegisterField, 0x0055AF00, (void* self, void* edx, uint32_t* a2));

static void MpRules_RegisterNetworkParameter(
    S_0055af00* self, uint8_t* descriptor,
    const char* name, uint32_t* value, int type)
{
    CSF_AT(const char*, descriptor, 0x00) = name;
    CSF_AT(uint32_t*, descriptor, 0x04) = value;
    CSF_AT(int, descriptor, 0x08) = 0;
    CSF_AT(int, descriptor, 0x0C) = type;
    DataDefs_RegisterField(self, nullptr, reinterpret_cast<uint32_t*>(descriptor));
}

static uint32_t* __fastcall S_0055af00_InitializeNetworkParameters(
    S_0055af00* self, void* /*edx*/)
{
    // The packet also labels the direct-call address as a vtable reference.
    (void)CSF_VTBL(0x0055AF00);

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_00a4 = 200;
    self->field_0010 = 200;
    self->field_00a8 = 500;
    self->field_0014 = 500;
    self->field_00ac = 0;
    self->field_0018 = 0;
    self->field_00b0 = 0;
    self->field_001c = 0;
    self->field_00b4 = 0;
    self->field_0020 = 0;
    self->field_00b8 = 0;
    self->field_0024 = 0;
    self->field_00bc = 0;
    self->field_0028 = 0;
    self->field_00c0 = 1024;
    self->field_002c = 1024;
    self->field_00c4 = 1024;
    self->field_0030 = 1024;
    self->field_00c8 = 3;
    self->field_0034 = 3;
    self->field_00cc = 3;
    self->field_0038 = 3;
    self->field_00d0 = 0;
    self->field_003c = 0;
    self->field_00d4 = 0;
    self->field_0040 = 0;
    self->field_00d8 = 0x40000000;
    self->field_0044 = 0x40000000;
    self->field_00dc = 1008981770;
    self->field_0048 = 1008981770;
    self->field_00e0 = 0x40000000;
    self->field_004c = 0x40000000;
    self->field_00e4 = 1161527296;
    self->field_0050 = 1161527296;
    self->field_00e8 = 1008981770;
    self->field_0054 = 1008981770;
    self->field_00ec = 0x40000000;
    self->field_0058 = 0x40000000;
    self->field_00f0 = 1161527296;
    self->field_005c = 1161527296;
    self->field_00f4 = 0;
    self->field_0060 = 0;
    self->field_00f8 = 0;
    self->field_0064 = 0;
    self->field_00fc = 0x40000000;
    self->field_0068 = 0x40000000;
    self->field_0100 = 1008981770;
    self->field_006c = 1008981770;
    self->field_0104 = 0x40000000;
    self->field_0070 = 0x40000000;
    self->field_0108 = 1161527296;
    self->field_0074 = 1161527296;
    self->field_010c = 1008981770;
    self->field_0078 = 1008981770;
    self->field_0110 = 0x40000000;
    self->field_007c = 0x40000000;
    self->field_0114 = 1161527296;
    self->field_0080 = 1161527296;
    self->field_0118 = 0;
    self->field_0084 = 0;
    self->field_011c = 0;
    self->field_0088 = 0;
    self->field_0120 = 200;
    self->field_008c = 200;
    self->field_0124 = 100;
    self->field_0090 = 100;
    self->field_0128 = 1000;
    self->field_0094 = 1000;
    self->field_012c = 1;
    self->field_0098 = 1;
    self->field_0130 = 0;
    self->field_009c = 0;
    self->field_0134 = 0;
    self->field_00a0 = 0;

    alignas(8) uint8_t descriptorBytes[0x10];

#define REGISTER_NETWORK_PARAMETER(member, name, type) \
    MpRules_RegisterNetworkParameter(self, descriptorBytes, name, &self->member, type)

    REGISTER_NETWORK_PARAMETER(field_0010, CSF_STR(0x0084534C, ".uParRed_LAN_TiempoPaquete"), 16);
    REGISTER_NETWORK_PARAMETER(field_0014, CSF_STR(0x00845330, ".uParRed_LAN_TamanyoPaquete"), 16);
    REGISTER_NETWORK_PARAMETER(field_0018, CSF_STR(0x00845310, ".uParRed_LAN_EmpaquetadoActivo"), 16);
    REGISTER_NETWORK_PARAMETER(field_001c, CSF_STR(0x008452F0, ".uParRed_LAN_EntradaLatencia"), 16);
    REGISTER_NETWORK_PARAMETER(field_0020, CSF_STR(0x008452D4, ".uParRed_LAN_EntradaJitter"), 16);
    REGISTER_NETWORK_PARAMETER(field_0024, CSF_STR(0x008452B8, ".uParRed_LAN_SalidaLatencia"), 16);
    REGISTER_NETWORK_PARAMETER(field_0028, CSF_STR(0x0084529C, ".uParRed_LAN_SalidaJitter"), 16);
    REGISTER_NETWORK_PARAMETER(field_002c, CSF_STR(0x0084527C, ".uParRed_LAN_BandWidthEntrada"), 16);
    REGISTER_NETWORK_PARAMETER(field_0030, CSF_STR(0x0084525C, ".uParRed_LAN_BandWidthSalida"), 16);
    REGISTER_NETWORK_PARAMETER(field_0034, CSF_STR(0x00845230, ".uParRed_LAN_BandWidthEntradaSeleccionado"), 16);
    REGISTER_NETWORK_PARAMETER(field_0038, CSF_STR(0x00845204, ".uParRed_LAN_BandWidthSalidaSeleccionado"), 16);
    REGISTER_NETWORK_PARAMETER(field_003c, CSF_STR(0x008451E4, ".uParRed_LAN_SimulacionActiva"), 16);
    REGISTER_NETWORK_PARAMETER(field_0040, CSF_STR(0x008451CC, ".uParRed_LAN_OverHead"), 16);
    REGISTER_NETWORK_PARAMETER(field_0044, CSF_STR(0x008451AC, ".uParRed_LAN_ConstanteErrorPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_0048, CSF_STR(0x0084518C, ".uParRed_LAN_LinealMinimoPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_004c, CSF_STR(0x0084516C, ".uParRed_LAN_LinealMaximoPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_0050, CSF_STR(0x0084514C, ".uParRed_LAN_LinealDistanciaPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_0054, CSF_STR(0x00845128, ".uParRed_LAN_CuadraticoMinimoPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_0058, CSF_STR(0x00845104, ".uParRed_LAN_CuadraticoMaximoPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_005c, CSF_STR(0x008450E0, ".uParRed_LAN_CuadraticoDistanciaPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_0060, CSF_STR(0x008450BC, ".uParRed_LAN_FrecuenciadoActivoPos"), 16);
    REGISTER_NETWORK_PARAMETER(field_0064, CSF_STR(0x00845090, ".uParRed_LAN_FrecuenciadoSeleccionadoPos"), 16);
    REGISTER_NETWORK_PARAMETER(field_0068, CSF_STR(0x00845070, ".uParRed_LAN_ConstanteErrorDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_006c, CSF_STR(0x00845050, ".uParRed_LAN_LinealMinimoDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_0070, CSF_STR(0x00845030, ".uParRed_LAN_LinealMaximoDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_0074, CSF_STR(0x00845010, ".uParRed_LAN_LinealDistanciaDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_0078, CSF_STR(0x00844FEC, ".uParRed_LAN_CuadraticoMinimoDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_007c, CSF_STR(0x00844FC8, ".uParRed_LAN_CuadraticoMaximoDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_0080, CSF_STR(0x00844FA4, ".uParRed_LAN_CuadraticoDistanciaDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_0084, CSF_STR(0x00844F80, ".uParRed_LAN_FrecuenciadoActivoDir"), 16);
    REGISTER_NETWORK_PARAMETER(field_0088, CSF_STR(0x00844F54, ".uParRed_LAN_FrecuenciadoSeleccionadoDir"), 16);
    REGISTER_NETWORK_PARAMETER(field_008c, CSF_STR(0x00844F34, ".uParRed_LAN_ExtrapolationDelay"), 16);
    REGISTER_NETWORK_PARAMETER(field_0090, CSF_STR(0x00844F10, ".uParRed_LAN_MinConvergenceDelay"), 16);
    REGISTER_NETWORK_PARAMETER(field_0094, CSF_STR(0x00844EEC, ".uParRed_LAN_MaxConvergenceDelay"), 16);
    REGISTER_NETWORK_PARAMETER(field_0098, CSF_STR(0x00844EC8, ".uParRed_LAN_AjustarRelojALatencia"), 16);
    REGISTER_NETWORK_PARAMETER(field_009c, CSF_STR(0x00844EA8, ".uParRed_LAN_LatenciaEntrada"), 16);
    REGISTER_NETWORK_PARAMETER(field_00a0, CSF_STR(0x00844E8C, ".uParRed_LAN_LatenciaSalida"), 16);

    REGISTER_NETWORK_PARAMETER(field_00a4, CSF_STR(0x00844E70, ".uParRed_WAN_TiempoPaquete"), 16);
    REGISTER_NETWORK_PARAMETER(field_00a8, CSF_STR(0x00844E54, ".uParRed_WAN_TamanyoPaquete"), 16);
    REGISTER_NETWORK_PARAMETER(field_00ac, CSF_STR(0x00844E34, ".uParRed_WAN_EmpaquetadoActivo"), 16);
    REGISTER_NETWORK_PARAMETER(field_00b0, CSF_STR(0x00844E14, ".uParRed_WAN_EntradaLatencia"), 16);
    REGISTER_NETWORK_PARAMETER(field_00b4, CSF_STR(0x00844DF8, ".uParRed_WAN_EntradaJitter"), 16);
    REGISTER_NETWORK_PARAMETER(field_00b8, CSF_STR(0x00844DDC, ".uParRed_WAN_SalidaLatencia"), 16);
    REGISTER_NETWORK_PARAMETER(field_00bc, CSF_STR(0x00844DC0, ".uParRed_WAN_SalidaJitter"), 16);
    REGISTER_NETWORK_PARAMETER(field_00c0, CSF_STR(0x00844DA0, ".uParRed_WAN_BandWidthEntrada"), 16);
    REGISTER_NETWORK_PARAMETER(field_00c4, CSF_STR(0x00844D80, ".uParRed_WAN_BandWidthSalida"), 16);
    REGISTER_NETWORK_PARAMETER(field_00c8, CSF_STR(0x00844D54, ".uParRed_WAN_BandWidthEntradaSeleccionado"), 16);
    REGISTER_NETWORK_PARAMETER(field_00cc, CSF_STR(0x00844D28, ".uParRed_WAN_BandWidthSalidaSeleccionado"), 16);
    REGISTER_NETWORK_PARAMETER(field_00d0, CSF_STR(0x00844D08, ".uParRed_WAN_SimulacionActiva"), 16);
    REGISTER_NETWORK_PARAMETER(field_00d4, CSF_STR(0x00844CF0, ".uParRed_WAN_OverHead"), 16);
    REGISTER_NETWORK_PARAMETER(field_00d8, CSF_STR(0x00844CD0, ".uParRed_WAN_ConstanteErrorPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_00dc, CSF_STR(0x00844CB0, ".uParRed_WAN_LinealMinimoPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_00e0, CSF_STR(0x00844C90, ".uParRed_WAN_LinealMaximoPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_00e4, CSF_STR(0x00844C70, ".uParRed_WAN_LinealDistanciaPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_00e8, CSF_STR(0x00844C4C, ".uParRed_WAN_CuadraticoMinimoPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_00ec, CSF_STR(0x00844C28, ".uParRed_WAN_CuadraticoMaximoPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_00f0, CSF_STR(0x00844C04, ".uParRed_WAN_CuadraticoDistanciaPos"), 32);
    REGISTER_NETWORK_PARAMETER(field_00f4, CSF_STR(0x00844BE0, ".uParRed_WAN_FrecuenciadoActivoPos"), 16);
    REGISTER_NETWORK_PARAMETER(field_00f8, CSF_STR(0x00844BB4, ".uParRed_WAN_FrecuenciadoSeleccionadoPos"), 16);
    REGISTER_NETWORK_PARAMETER(field_00fc, CSF_STR(0x00844B94, ".uParRed_WAN_ConstanteErrorDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_0100, CSF_STR(0x00844B74, ".uParRed_WAN_LinealMinimoDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_0104, CSF_STR(0x00844B54, ".uParRed_WAN_LinealMaximoDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_0108, CSF_STR(0x00844B34, ".uParRed_WAN_LinealDistanciaDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_010c, CSF_STR(0x00844B10, ".uParRed_WAN_CuadraticoMinimoDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_0110, CSF_STR(0x00844AEC, ".uParRed_WAN_CuadraticoMaximoDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_0114, CSF_STR(0x00844AC8, ".uParRed_WAN_CuadraticoDistanciaDir"), 32);
    REGISTER_NETWORK_PARAMETER(field_0118, CSF_STR(0x00844AA4, ".uParRed_WAN_FrecuenciadoActivoDir"), 16);
    REGISTER_NETWORK_PARAMETER(field_011c, CSF_STR(0x00844A78, ".uParRed_WAN_FrecuenciadoSeleccionadoDir"), 16);
    REGISTER_NETWORK_PARAMETER(field_0120, CSF_STR(0x00844A58, ".uParRed_WAN_ExtrapolationDelay"), 16);
    REGISTER_NETWORK_PARAMETER(field_0124, CSF_STR(0x00844A34, ".uParRed_WAN_MinConvergenceDelay"), 16);
    REGISTER_NETWORK_PARAMETER(field_0128, CSF_STR(0x00844A10, ".uParRed_WAN_MaxConvergenceDelay"), 16);
    REGISTER_NETWORK_PARAMETER(field_012c, CSF_STR(0x008449EC, ".uParRed_WAN_AjustarRelojALatencia"), 16);
    REGISTER_NETWORK_PARAMETER(field_0130, CSF_STR(0x008449CC, ".uParRed_WAN_LatenciaEntrada"), 16);
    REGISTER_NETWORK_PARAMETER(field_0134, CSF_STR(0x008449B0, ".uParRed_WAN_LatenciaSalida"), 16);

#undef REGISTER_NETWORK_PARAMETER

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0055AF70, S_0055af00_InitializeNetworkParameters, "auto");
