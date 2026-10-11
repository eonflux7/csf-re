#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(uint32_t*, __fastcall, NetGatheringDdl_InitializeSession, 0x006A74C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ReportRequestResult, 0x006A7B50, (void* self, void* edx, int a2, int* a3));
CSF_FUNC(char, __fastcall, NetGatheringDdl_CheckGatheringAccess, 0x00791790, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __stdcall, NetGatheringDdl_RegisterSession, 0x00798F70, (int a1));
CSF_FUNC(char, __fastcall, NetGatheringDdl_CheckGatheringReady, 0x007A0550, (void* self, void* edx, int a2));

using SleepFunction = void (__stdcall*)(int);
CSF_GLOBAL(SleepFunction, g_SleepImportSlot, 0x0082D05C);

static void __fastcall S_006a7640_UpdateGatheringRequest(
    S_006a7640* self, void* /*edx*/, int /*unused*/)
{
    if (self->field_0008 != 1)
        return;

    auto* request = reinterpret_cast<C_0085e760*>(self->field_0090);
    switch (CSF_AT(uint32_t, request, 0x0C))
    {
    case 1:
        g_SleepImportSlot(10);
        return;

    case 2:
        if (request)
        {
            request->vSlot0(1);
            self->field_0090 = 0;
        }

        if (self->field_0038)
        {
            if (NetGatheringDdl_CheckGatheringAccess(
                    CSF_AT(void*, self, 0x2C), nullptr,
                    reinterpret_cast<uint32_t*>(self->field_0038)) &&
                NetGatheringDdl_CheckGatheringAccess(
                    CSF_AT(void*, self, 0x30), nullptr,
                    reinterpret_cast<uint32_t*>(self->field_0038)) &&
                NetGatheringDdl_CheckGatheringReady(
                    CSF_AT(void*, self, 0x3C), nullptr,
                    static_cast<int>(self->field_0038)))
            {
                void* storage = NetZ_OperatorNew(600);
                uint32_t* session = storage
                    ? NetGatheringDdl_InitializeSession(storage, nullptr)
                    : nullptr;

                NetGatheringDdl_RegisterSession(reinterpret_cast<int>(session));
                self->field_0008 = 2;
                CSF_AT(uint32_t, self, 0x0C) = 0;
                CSF_AT(uint8_t, self, 0x334) = 0;
                g_SleepImportSlot(10);
                return;
            }

            self->field_02b0 = 1229730102;
        }
        else
        {
            NetGatheringDdl_ReportRequestResult(
                self, nullptr, 3,
                reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x94));
        }
        break;

    case 3:
        self->field_02b0 = 1229730102;
        NetGatheringDdl_ReportRequestResult(
            self, nullptr, 3,
            reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x94));

        request = reinterpret_cast<C_0085e760*>(self->field_0090);
        if (request)
        {
            request->vSlot0(1);
            self->field_0090 = 0;
        }
        break;

    default:
        self->field_02b0 = 1229730103;
        if (request)
        {
            request->vSlot0(1);
            self->field_0090 = 0;
        }
        break;
    }

    auto* vtable = reinterpret_cast<void**>(self->field_0000);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(vtable[17])(
        self, nullptr, 0);
    g_SleepImportSlot(10);
}

HOOK(0x006A83F0, S_006a7640_UpdateGatheringRequest, "auto");
