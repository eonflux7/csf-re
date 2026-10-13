#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B055C, 0x004F9660, ());
CSF_FUNC(char, __cdecl, DevConsole_EnsureInitialized, 0x004F99D0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0560, 0x004FA6B0, ());
CSF_FUNC(char, __cdecl, DevConsole_EnsureInitialized_4FB690, 0x004FB690, ());
CSF_FUNC(int, __cdecl, NetProfile_GetOrCreateCachedBlock, 0x00518A00, ());
CSF_FUNC(uint32_t*, __cdecl, NetProfile_GetOrCreateObject, 0x00518F50, (int initArg));
CSF_FUNC(uint32_t*, __cdecl, NetProfile_GetOrCreateInstance, 0x00518FE0, (int initialValue));
CSF_FUNC(uint32_t*, __cdecl, NetProfile_GetOrCreateCachedInstance, 0x00519050, (int initialValue));
CSF_FUNC(int32_t*, __cdecl, GetOrCreateNetProfileInstance, 0x005190B0, (int32_t profileId));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, ErrorLog_SetFpuControlWordAndGetIt, 0x0069D890, ());
CSF_FUNC(int, __fastcall, MenuSystem_AssignDialogPanel, 0x00712E80, (void* self, void* edx, int panel));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(int, g_Value8B055C, 0x008B055C);
CSF_GLOBAL(int, g_globalValue_8B0560, 0x008B0560);

static int __fastcall MenuControls_InitializeDialogPanel(void* self, void* /*edx*/)
{
    ErrorLog_SetFpuControlWordAndGetIt();

    void* globalState = reinterpret_cast<void*>(GetGlobalValue_753A30());
    if (CSF_AT(uint8_t, globalState, 0x11C))
    {
        NetProfile_GetOrCreateObject(0);
        GetOrCreateNetProfileInstance(0);
        NetProfile_GetOrCreateInstance(0);
        NetProfile_GetOrCreateCachedInstance(0);
    }
    else
    {
        NetProfile_GetOrCreateObject(CSF_AT(int, self, 0xF8));
        GetOrCreateNetProfileInstance(CSF_AT(int, self, 0xF8));
        NetProfile_GetOrCreateInstance(CSF_AT(int, self, 0xF8));
        NetProfile_GetOrCreateCachedInstance(CSF_AT(int, self, 0xF8));
    }

    DevConsole_EnsureInitialized_4FB690();
    DevConsole_EnsureInitialized();

    auto* referenceHolder = reinterpret_cast<C_0085ec18*>(g_Value8B055C);
    void* embeddedReference = static_cast<uint8_t*>(self) + 4;
    referenceHolder->field_0004 =
        reinterpret_cast<uint32_t>(GetReferencedObject(embeddedReference, nullptr));

    GetValue8B055C();
    NetProfile_GetOrCreateCachedBlock();

    int panel = g_globalValue_8B0560;
    GetGlobalValue_8B0560();

    void* referencedObject = GetReferencedObject(embeddedReference, nullptr);
    return MenuSystem_AssignDialogPanel(referencedObject, nullptr, panel);
}

HOOK(0x00543860, MenuControls_InitializeDialogPanel, "auto");
