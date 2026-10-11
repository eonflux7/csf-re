#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszMenu_P_MeCaPa, 0x899B88);
CSF_GLOBAL(const char*, g_pszMenu_P_AuUnPa, 0x899B90);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFLIn, 0x899B64);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFIOp, 0x899B68);
CSF_GLOBAL(const char*, g_pszMenu_P_EEULA, 0x899B94);

// Forwards four menu labels (and the EULA label) to the object at field_0004 through its vtable slot 11 (+0x2C).
static int __fastcall C_00840754_ApplyMenuLabels(C_00840754* self, void* /*edx*/) {
    // Each call re-reads self->field_0004 and its vtable, as the original does.
    {
        void* target = reinterpret_cast<void*>(self->field_0004);
        auto setLabel = reinterpret_cast<void(__fastcall*)(void*, void*, C_008407a8*, const char*)>(
            CSF_AT(void**, target, 0)[0x2C / 4]);
        setLabel(target, nullptr, reinterpret_cast<C_008407a8*>(self->field_0064), g_pszMenu_P_MeCaPa);
    }
    {
        void* target = reinterpret_cast<void*>(self->field_0004);
        auto setLabel = reinterpret_cast<void(__fastcall*)(void*, void*, C_008407a8*, const char*)>(
            CSF_AT(void**, target, 0)[0x2C / 4]);
        setLabel(target, nullptr, reinterpret_cast<C_008407a8*>(self->field_0068), g_pszMenu_P_AuUnPa);
    }
    {
        void* target = reinterpret_cast<void*>(self->field_0004);
        auto setLabel = reinterpret_cast<void(__fastcall*)(void*, void*, C_008407a8*, const char*)>(
            CSF_AT(void**, target, 0)[0x2C / 4]);
        setLabel(target, nullptr, reinterpret_cast<C_008407a8*>(self->field_0030), g_pszMenu_P_LbFLIn);
    }
    {
        void* target = reinterpret_cast<void*>(self->field_0004);
        auto setLabel = reinterpret_cast<void(__fastcall*)(void*, void*, C_008407a8*, const char*)>(
            CSF_AT(void**, target, 0)[0x2C / 4]);
        setLabel(target, nullptr, reinterpret_cast<C_008407a8*>(self->field_0034), g_pszMenu_P_LbFIOp);
    }
    void* target = reinterpret_cast<void*>(self->field_0004);
    auto setLabel = reinterpret_cast<int(__fastcall*)(void*, void*, C_008407a8*, const char*)>(
        CSF_AT(void**, target, 0)[0x2C / 4]);
    return setLabel(target, nullptr, reinterpret_cast<C_008407a8*>(self->field_0074), g_pszMenu_P_EEULA);
}

HOOK(0x509EE0, C_00840754_ApplyMenuLabels, "auto");
