#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_457A80, 0x00457A80, ());
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SaveGames_ProcessHeader, 0x005144C0, (void* self, void* edx, int a2));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __stdcall SaveGames_ProcessArchive(int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    S_004f21d0* header = CSF_AT(S_004f21d0*, archive, 0x20);

    if (CSF_AT(uint8_t, archive, 0x14) && !CSF_AT(uint8_t, archive, 0))
    {
        header->field_00b4 =
            CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xB44);
        header->field_00bc =
            CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x84);
    }

    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!CSF_AT(uint8_t, archive, 0))
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        SaveGames_ProcessHeader(header, nullptr, archiveAddress);
    }

    if (header->field_00b8 != 1527564717)
        CSF_AT(uint8_t, archive, 0) = 1;

    if (!CSF_AT(uint8_t, archive, 0x14) && !CSF_AT(uint8_t, archive, 0))
    {
        uint32_t savedState = header->field_00bc;
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x84) =
            savedState;
    }

    CSF_AT(uint32_t, archive, 0x18) = header->field_00b4;
    if (!CSF_AT(uint8_t, archive, 0))
    {
        do
        {
            void* subsystem = reinterpret_cast<void*>(GetGlobalValue_8B0568());
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[2])(
                subsystem, nullptr, archiveAddress);
            if (CSF_AT(uint8_t, archive, 0))
                break;

            subsystem = reinterpret_cast<uint8_t*>(GetValue8B0554()) + 0x1FBC;
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[2])(
                subsystem, nullptr, archiveAddress);
            if (CSF_AT(uint8_t, archive, 0))
                break;

            subsystem = reinterpret_cast<uint8_t*>(GetGlobalValue_51F740()) + 4;
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[2])(
                subsystem, nullptr, archiveAddress);
            if (CSF_AT(uint8_t, archive, 0))
                break;

            subsystem = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[2])(
                subsystem, nullptr, archiveAddress);
            if (CSF_AT(uint8_t, archive, 0))
                break;

            subsystem = reinterpret_cast<uint8_t*>(GetGlobalValue_521BB0()) + 4;
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[2])(
                subsystem, nullptr, archiveAddress);
            if (CSF_AT(uint8_t, archive, 0))
                break;

            subsystem = reinterpret_cast<void*>(Get_008b0678());
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[2])(
                subsystem, nullptr, archiveAddress);
            if (CSF_AT(uint8_t, archive, 0))
                break;

            subsystem = reinterpret_cast<void*>(GetGlobal8afc7c());
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[2])(
                subsystem, nullptr, archiveAddress);
            if (CSF_AT(uint8_t, archive, 0))
                break;

            subsystem = reinterpret_cast<void*>(GetGlobal8B0558());
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[2])(
                subsystem, nullptr, archiveAddress);
            if (CSF_AT(uint8_t, archive, 0))
                break;

            subsystem = reinterpret_cast<void*>(GetGlobalValue_457A80());
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[2])(
                subsystem, nullptr, archiveAddress);
        } while (false);
    }

    int result = CSF_AT(int, reinterpret_cast<void*>(GetGlobal8afc78()), 0x248);
    if (result && !CSF_AT(uint8_t, archive, 0))
    {
        void* subsystem = reinterpret_cast<uint8_t*>(result) + 4;
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[2])(
            subsystem, nullptr, archiveAddress);
    }
    return result;
}

HOOK(0x00514680, SaveGames_ProcessArchive, "auto");
