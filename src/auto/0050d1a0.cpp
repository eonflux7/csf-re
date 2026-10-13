#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using DeleteCriticalSectionFn = void(__stdcall*)(void*);

CSF_FUNC(int, __cdecl, SetGlobalValue8BAD60, 0x006910A0, (int32_t value));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void, __cdecl, MenuAccounts_ResetPendingEdit, 0x006A6D40, ());
CSF_FUNC(int, __cdecl, ReleaseSessionDumpBuffer, 0x0073F380, ());
CSF_FUNC(int, __cdecl, CampaignBriefing_ReleaseCachedInstance, 0x0043E020, ());
CSF_FUNC(int, __cdecl, ReleaseInputBindingsInstance, 0x00769BF0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int*, __cdecl, GetGlobalIntPtr, 0x0069C580, ());
CSF_FUNC(int, __cdecl, CoreFiles_ResetArchiveSelection, 0x006980D0, (char* name));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void*, __cdecl, MenuSubsystem_ReleaseState, 0x00521BC0, ());
CSF_FUNC(int, __cdecl, ReleaseCachedValueInstance, 0x006B1F80, ());
CSF_FUNC(int, __cdecl, RwEngineTerm, 0x00626EC0, ());
CSF_FUNC(int, __cdecl, CachedInstance_Release, 0x0069B710, ());
CSF_FUNC(int, __cdecl, RwNodeRelease, 0x007C66D0, (int a1, int a2));

CSF_GLOBAL(void*, g_pMenuManager, 0x008B0570);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);
CSF_GLOBAL(int, dword_8BC7C8, 0x008BC7C8);
CSF_GLOBAL(int, dword_8BC7D0, 0x008BC7D0);
CSF_GLOBAL(int, dword_8BC7D4, 0x008BC7D4);
CSF_GLOBAL(void*, g_pfnDeleteCriticalSection, 0x0082D064);

static void* IntToPointer(int value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

static void MenuSystem_Shutdown()
{
    SetGlobalValue8BAD60(0);
    if (GetGlobalState_6A6D70() != 0)
    {
        void* stateObj = IntToPointer(GetGlobalState_6A6D70());
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stateObj, 0)[17])(stateObj, nullptr, 1);
    }

    MenuAccounts_ResetPendingEdit();
    ReleaseSessionDumpBuffer();
    CampaignBriefing_ReleaseCachedInstance();
    ReleaseInputBindingsInstance();

    if (g_pMenuManager != nullptr)
    {
        void* valueObj = IntToPointer(GetGlobalValue_8BC1E0());
        if (CSF_AT(uint8_t, valueObj, 0xA5C) != 0)
        {
            CoreFiles_ResetArchiveSelection(reinterpret_cast<char*>(GetGlobalIntPtr()));
        }
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, g_pMenuManager, 0)[36])(g_pMenuManager, nullptr);
        if (g_pMenuManager != nullptr)
        {
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, g_pMenuManager, 0)[0])(g_pMenuManager, nullptr, 1);
            g_pMenuManager = nullptr;
        }
    }

    void* globalValueObj = IntToPointer(GetGlobalValue_8B056C());
    MenuSubsystem_ReleaseState();

    void* slotOne = CSF_AT(void*, globalValueObj, 0x4);
    if (slotOne != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, slotOne, 0)[0])(slotOne, nullptr, 1);
        CSF_AT(void*, globalValueObj, 0x4) = nullptr;
    }

    void* slotTwo = CSF_AT(void*, globalValueObj, 0x8);
    if (slotTwo != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, slotTwo, 0)[0])(slotTwo, nullptr, 1);
        CSF_AT(void*, globalValueObj, 0x8) = nullptr;
    }

    void* slotSix = CSF_AT(void*, globalValueObj, 0x18);
    if (slotSix != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, slotSix, 0)[0])(slotSix, nullptr, 1);
        CSF_AT(void*, globalValueObj, 0x18) = nullptr;
    }

    void* slotSeven = CSF_AT(void*, globalValueObj, 0x1C);
    if (slotSeven != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, slotSeven, 0)[1])(slotSeven, nullptr, 1);
        CSF_AT(void*, globalValueObj, 0x1C) = nullptr;
    }

    void* slotFive = CSF_AT(void*, globalValueObj, 0x14);
    if (slotFive != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, slotFive, 0)[0])(slotFive, nullptr, 1);
        CSF_AT(void*, globalValueObj, 0x14) = nullptr;
    }

    ReleaseCachedValueInstance();
    RwEngineTerm();
    CachedInstance_Release();

    void* optionsSlot19 = CSF_AT(void*, g_pOptions, 0x4C);
    if (optionsSlot19 != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, optionsSlot19, 0)[0])(optionsSlot19, nullptr, 1);
        CSF_AT(void*, g_pOptions, 0x4C) = nullptr;
    }

    void* optionsSlot17 = CSF_AT(void*, g_pOptions, 0x44);
    if (optionsSlot17 != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, optionsSlot17, 0)[0])(optionsSlot17, nullptr, 1);
        CSF_AT(void*, g_pOptions, 0x44) = nullptr;
    }

    void* optionsSlot18 = CSF_AT(void*, g_pOptions, 0x48);
    if (optionsSlot18 != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, optionsSlot18, 0)[0])(optionsSlot18, nullptr, 1);
        CSF_AT(void*, g_pOptions, 0x48) = nullptr;
    }

    if (dword_8BC7D4 == 1)
    {
        int nodeAddr = dword_8BC7C8;
        if (nodeAddr != 0)
        {
            do
            {
                void* node = IntToPointer(nodeAddr);
                int nextAddr = CSF_AT(int, node, 0);
                ((void(__cdecl*)(int))CSF_AT(void*, node, 0x18))(nodeAddr);

                int rwHandle = CSF_AT(int, node, 0xC);
                ((void(__cdecl*)(int))CSF_AT(void*, g_pRwEngineInstance, 0x10C))(rwHandle);
                CSF_AT(int, node, 0xC) = 0;

                RwNodeRelease(nodeAddr, 2);
                ((void(__cdecl*)(int))CSF_AT(void*, g_pRwEngineInstance, 0x10C))(nodeAddr);

                nodeAddr = nextAddr;
            } while (nodeAddr != 0);
        }

        reinterpret_cast<DeleteCriticalSectionFn>(g_pfnDeleteCriticalSection)(
            reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BEB40)));
        dword_8BC7D4 = 0;
        dword_8BC7D0 = 0;
    }
}

HOOK(0x0050D1A0, MenuSystem_Shutdown, "auto");
