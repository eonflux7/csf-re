#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));

static std::uint8_t __fastcall C_00844928_LoadMiscellaneousRules(
    C_00844928* self, void* /*edx*/, S_00692b80* properties)
{
    auto* misc = FindProperty(
        reinterpret_cast<std::uint8_t*>(properties), nullptr,
        const_cast<char*>(CSF_STR(0x008448AC, ".VALORES_MISCELANEOS")));
    if (!misc)
        return 0;

    self->field_0198 = static_cast<float>(ReadFloatProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x0084488C, ".TIEMPO_SECS_INTERROGAR_ESPIA"))));
    self->field_019c = static_cast<float>(ReadFloatProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x00844868, ".TIEMPO_SECS_INTERROGAR_NO_ESPIA"))));
    self->field_01a0 = static_cast<float>(ReadFloatProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x00844850, ".TIEMPO_EN_MORIBUNDO"))));
    self->field_01a4 = ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x00844838, ".CLAVES_QUE_DA_ESPIA")));
    self->field_01a8 = ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x00844820, ".CLAVES_QUE_DA_NO_ESPIA")));
    self->field_01ac = ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x0084480C, ".CLAVES_PARA_GANAR")));
    self->field_01b0 = static_cast<float>(ReadFloatProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x008447F4, ".TIEMPO_ACTIVAR_BOMBA"))));
    self->field_01b4 = static_cast<float>(ReadFloatProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x008447D8, ".TIEMPO_DESACTIVAR_BOMBA"))));
    self->field_01b8 = ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x008447C0, ".CLASSID_BOMBA_ALIADA")));
    self->field_01bc = ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x008447A8, ".CLASSID_BOMBA_ALEMANA")));
    self->field_01c0 = ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x00844784, ".TIEMPO_EXPLOSION_BOMBA_SABOTAGE_MS")));
    self->field_01c4 = static_cast<float>(ReadFloatProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x00844764, ".RADIO_EXPLOSION_BOMBA_SABOTAGE"))));
    self->field_01c8 = ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x00844744, ".DANIO_EXPLOSION_BOMBA_SABOTAGE")));
    self->field_01cc = static_cast<std::uint8_t>(ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x00844738, ".ALIADOS_R"))));
    self->field_01cd = static_cast<std::uint8_t>(ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x0084472C, ".ALIADOS_G"))));
    self->field_01ce = static_cast<std::uint8_t>(ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x00844720, ".ALIADOS_B"))));
    self->field_01cf = static_cast<std::uint8_t>(ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x00844714, ".ALEMANES_R"))));
    self->field_01d0 = static_cast<std::uint8_t>(ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x00844708, ".ALEMANES_G"))));
    self->field_01d1 = static_cast<std::uint8_t>(ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x008446FC, ".ALEMANES_B"))));
    self->field_01d2 = static_cast<std::uint8_t>(ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x008446F0, ".NEUTRO_R"))));
    self->field_01d3 = static_cast<std::uint8_t>(ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x008446E4, ".NEUTRO_G"))));
    self->field_01d4 = static_cast<std::uint8_t>(ReadIntegerProperty(
        misc, nullptr, const_cast<char*>(CSF_STR(0x008446D8, ".NEUTRO_B"))));
    return 1;
}

HOOK(0x00558180, C_00844928_LoadMiscellaneousRules, "auto");
