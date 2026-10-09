#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Callees of the state handlers (addresses from the packet).
CSF_FUNC(char, __fastcall, CCharActionFsm_TickStateA, 0x00484E90, (void* self, void* edx, float a3));
CSF_FUNC(void, __fastcall, sub_4846D0, 0x004846D0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_488050, 0x00488050, (void* self, void* edx, int a3));
CSF_FUNC(void, __fastcall, CCharWeaponFsm_AdvanceTimer, 0x00482670, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, sub_486FC0, 0x00486FC0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_487100, 0x00487100, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_Unknown_InvokeSubobjectVfunc2C, 0x004F51A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_4FC3B0, 0x004FC3B0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharWeaponFsm_HandleDefaultState, 0x0047FB00, (void* self, void* edx, int a2));

static int FloatBits(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

// Shared lookup used by several state bodies: GetFirstElement(GetGlobalValue_4FBDE0()) -> GetCachedObjectProvider.
static void* CCharActionFsm_ResolveObjectProvider()
{
    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    return GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
}

static bool CCharActionFsm_ProviderSaysBusy(void* provider)
{
    return static_cast<unsigned char>(C_Unknown_InvokeSubobjectVfunc2C(provider, nullptr)) != 0;
}

// State 0x0B: pick the 2/0 variant of vSlot134 from the weapon's state, after clearing the pending state.
static void CCharActionFsm_HandleStateB(CCharActionFsm* self)
{
    if (!self->field_0069)
        return;

    bool useAltMode = false;
    void* weaponObj = reinterpret_cast<void*>(self->field_009c);
    if (weaponObj)
    {
        int weaponState = ((int(__fastcall*)(void*, void*))CSF_AT(void**, weaponObj, 0)[19])(weaponObj, nullptr);
        if (weaponState >= 10 && weaponState <= 13)
            useAltMode = true;
    }

    if (self->field_0094 != 0xFFFFFFFFu)
    {
        sub_4846D0(self, nullptr);
        self->field_0090 = self->field_0094;
        self->field_0094 = 0xFFFFFFFFu;
    }

    if (useAltMode)
        self->vSlot134(2, 0);
    else
        self->vSlot134(0, 0);
}

// State 0x0D (dispatcher already checked field_0010 == 0x17).
static void CCharActionFsm_HandleStateD(CCharActionFsm* self)
{
    void* provider = CCharActionFsm_ResolveObjectProvider();
    if (CCharActionFsm_ProviderSaysBusy(provider))
        return;

    if (static_cast<unsigned char>(Hud_IsSuspended(reinterpret_cast<void*>(self->field_0008), nullptr)) != 0)
    {
        void* obj = reinterpret_cast<void*>(self->vSlotD8());
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[12])(obj, nullptr, 0);
        int globalValue = GetGlobalValue_4FBDE0();
        sub_4FC3B0(reinterpret_cast<void*>(globalValue), nullptr, 0);
    }
    self->vSlot134(0, 0);
}

// State 0x0E (checks field_0010 == 0x18 itself).
static void CCharActionFsm_HandleStateE(CCharActionFsm* self)
{
    if (self->field_0010 != 0x18)
        return;

    void* provider = CCharActionFsm_ResolveObjectProvider();
    if (!self->field_0069)
        return;
    if (CCharActionFsm_ProviderSaysBusy(provider))
        return;
    self->vSlot134(0, 0);
}

// State 0x0F (checks field_0010 == 0x19 / 0x1A itself).
static void CCharActionFsm_HandleStateF(CCharActionFsm* self)
{
    uint32_t state = self->field_0010;
    if (state == 0x19)
    {
        void* provider = CCharActionFsm_ResolveObjectProvider();
        if (!CCharActionFsm_ProviderSaysBusy(provider))
            self->vSlot138(0x1A);
        return;
    }
    if (state != 0x1A)
        return;
    if (!self->field_0069)
        return;

    void* obj = reinterpret_cast<void*>(self->vSlotD8());
    char canContinue = ((char(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[11])(obj, nullptr);
    if (!canContinue)
        return;
    self->vSlot134(static_cast<int>(self->field_0064), 0);
}

// State 0x11 (checks field_0010 == 0x1C / 0x1D itself).
static void CCharActionFsm_HandleState11(CCharActionFsm* self)
{
    uint32_t state = self->field_0010;
    if (state == 0x1C)
    {
        if (self->field_0069)
            self->vSlot138(0x1D);
        return;
    }
    if (state != 0x1D)
        return;
    if (self->field_0024 == 0xFFFFFFFFu)
        return;

    sub_487100(self, nullptr, reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0xB0));
    CSF_AT(int, self, 0xB4) = -1;
    CSF_AT(int, self, 0xB8) = -1;
    sub_486FC0(self, nullptr);
    self->vSlot134(2, 0);
}

// State 0x12 (checks field_0010 == 0x1E / 0x1F / 0x20 itself).
static void CCharActionFsm_HandleState12(CCharActionFsm* self)
{
    uint32_t state = self->field_0010;
    if (state == 0x1E)
    {
        if (!self->field_0069)
            return;
        int value = self->vSlotFC();
        self->field_0028 = static_cast<uint32_t>(value);
        self->vSlotF8(-1);
        self->vSlot138(0x1F);
        return;
    }
    if (state == 0x1F)
    {
        if (self->field_0069)
            self->vSlot138(0x20);
        return;
    }
    if (state != 0x20)
        return;
    if (!self->field_0069)
        return;
    self->vSlot134(0, 0);
}

static void __fastcall CCharActionFsm_DispatchByState(CCharActionFsm* self, void* /*edx*/, float dt)
{
    if (self->field_00cc > g_fZero)
        self->field_00cc = self->field_00cc - dt;

    switch (self->field_000c)
    {
    case 0xA:
        CCharActionFsm_TickStateA(self, nullptr, dt);
        break;
    case 0xB:
        CCharActionFsm_HandleStateB(self);
        break;
    case 0xC:
        sub_488050(self, nullptr, FloatBits(dt));
        break;
    case 0xD:
        if (self->field_0010 == 0x17)
            CCharActionFsm_HandleStateD(self);
        break;
    case 0xE:
        CCharActionFsm_HandleStateE(self);
        break;
    case 0xF:
        CCharActionFsm_HandleStateF(self);
        break;
    case 0x10:
        CCharWeaponFsm_AdvanceTimer(self, nullptr, dt);
        break;
    case 0x11:
        CCharActionFsm_HandleState11(self);
        break;
    case 0x12:
        CCharActionFsm_HandleState12(self);
        break;
    default:
        CCharWeaponFsm_HandleDefaultState(self, nullptr, FloatBits(dt));
        break;
    }
}

HOOK(0x00488A20, CCharActionFsm_DispatchByState, "auto");
