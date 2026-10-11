#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

CSF_GLOBAL(const char*, g_psz_PFX_ESTELA_PISTOLA, 0x008A8BF0);
CSF_GLOBAL(const char*, g_psz_PFX_ESTELA_RIFLE, 0x008A8BF4);
CSF_GLOBAL(const char*, g_psz_PFX_ESTELA_RIFLE_PRECISION, 0x008A8BF8);
CSF_GLOBAL(const char*, g_psz_PFX_ESTELA_ESCOPETA, 0x008A8BFC);
CSF_GLOBAL(const char*, g_psz_PFX_ESTELA_SMG, 0x008A8C00);
CSF_GLOBAL(const char*, g_psz_PFX_ESTELA_MG, 0x008A8C04);
CSF_GLOBAL(const char*, g_psz_PFX_ESTELA_PISTOLA_SILENCIADOR, 0x008A8C08);
CSF_GLOBAL(const char*, g_psz_PFX_ESTELA_TANQUE_GUN, 0x008A8C0C);
CSF_GLOBAL(const char*, g_psz_PFX_ESTELA_ARMA_SDK, 0x008A8C10);
CSF_GLOBAL(const char*, g_psz_PFX_ESTELA_AMETRALLADORA_ESTATICA, 0x008A8C14);

typedef int(__fastcall* LookupByNameFn)(void*, void*, const char*);

static int __fastcall InitializeParticleManagerNode(C_00840f00* self, void* /*edx*/)
{
    int mgr;
    void* obj;

    mgr = GetValue8B0554();
    obj = CSF_AT(void*, (void*)mgr, 0x1FCC);
    self->field_0008 = (uint32_t)((LookupByNameFn)CSF_AT(void**, obj, 0)[3])(obj, nullptr, g_psz_PFX_ESTELA_PISTOLA);

    mgr = GetValue8B0554();
    obj = CSF_AT(void*, (void*)mgr, 0x1FCC);
    self->field_000c = (uint32_t)((LookupByNameFn)CSF_AT(void**, obj, 0)[3])(obj, nullptr, g_psz_PFX_ESTELA_RIFLE);

    mgr = GetValue8B0554();
    obj = CSF_AT(void*, (void*)mgr, 0x1FCC);
    self->field_0010 = (uint32_t)((LookupByNameFn)CSF_AT(void**, obj, 0)[3])(obj, nullptr, g_psz_PFX_ESTELA_RIFLE_PRECISION);

    mgr = GetValue8B0554();
    obj = CSF_AT(void*, (void*)mgr, 0x1FCC);
    self->field_0014 = (uint32_t)((LookupByNameFn)CSF_AT(void**, obj, 0)[3])(obj, nullptr, g_psz_PFX_ESTELA_ESCOPETA);

    mgr = GetValue8B0554();
    obj = CSF_AT(void*, (void*)mgr, 0x1FCC);
    self->field_0018 = (uint32_t)((LookupByNameFn)CSF_AT(void**, obj, 0)[3])(obj, nullptr, g_psz_PFX_ESTELA_SMG);

    mgr = GetValue8B0554();
    obj = CSF_AT(void*, (void*)mgr, 0x1FCC);
    self->field_001c = (uint32_t)((LookupByNameFn)CSF_AT(void**, obj, 0)[3])(obj, nullptr, g_psz_PFX_ESTELA_MG);

    mgr = GetValue8B0554();
    obj = CSF_AT(void*, (void*)mgr, 0x1FCC);
    self->field_0020 = (uint32_t)((LookupByNameFn)CSF_AT(void**, obj, 0)[3])(obj, nullptr, g_psz_PFX_ESTELA_PISTOLA_SILENCIADOR);

    mgr = GetValue8B0554();
    obj = CSF_AT(void*, (void*)mgr, 0x1FCC);
    self->field_0024 = (uint32_t)((LookupByNameFn)CSF_AT(void**, obj, 0)[3])(obj, nullptr, g_psz_PFX_ESTELA_TANQUE_GUN);

    mgr = GetValue8B0554();
    obj = CSF_AT(void*, (void*)mgr, 0x1FCC);
    self->field_0028 = (uint32_t)((LookupByNameFn)CSF_AT(void**, obj, 0)[3])(obj, nullptr, g_psz_PFX_ESTELA_ARMA_SDK);

    mgr = GetValue8B0554();
    obj = CSF_AT(void*, (void*)mgr, 0x1FCC);
    int result = ((LookupByNameFn)CSF_AT(void**, obj, 0)[3])(obj, nullptr, g_psz_PFX_ESTELA_AMETRALLADORA_ESTATICA);
    self->field_002c = (uint32_t)result;
    return result;
}

HOOK(0x0051DA10, InitializeParticleManagerNode, "auto");
