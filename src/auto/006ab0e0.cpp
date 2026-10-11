#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, NetGatheringDdl_HandleKind4, 0x006A74E0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetGatheringDdl_HandleKind10, 0x006A7730, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetGatheringDdl_HandleCommonKind, 0x006A7A70, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetGatheringDdl_HandleKind1, 0x006A83F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetGatheringDdl_HandleCompositeSubtype3, 0x006A8580, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetGatheringDdl_HandleCompositeSubtype8, 0x006A8730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetGatheringDdl_HandleKind7, 0x006A8820, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetGatheringDdl_HandleCompositeSubtype2, 0x006A9780, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetGatheringDdl_HandleKind5, 0x006A97F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetGatheringDdl_HandleKind6, 0x006A9820, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetGatheringDdl_HandleCompositeSubtype1, 0x006AAC40, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetGatheringDdl_HandleCompositeSubtype5, 0x006AAFE0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetGatheringDdl_HandleCompositeSubtype7, 0x006AB050, (void* self, void* edx, int a2));

static void __fastcall C_0085fe28_DispatchByKind(
    C_0085fe28* self, void* /*edx*/, int argument)
{
    switch (self->field_0008)
    {
    case 1u:
        NetGatheringDdl_HandleKind1(self, nullptr, argument);
        break;
    case 2u:
        NetGatheringDdl_HandleCommonKind(self, nullptr);
        break;
    case 3u:
        NetGatheringDdl_HandleCommonKind(self, nullptr);
        switch (self->field_000c)
        {
        case 1u:
            NetGatheringDdl_HandleCompositeSubtype1(self, nullptr, argument);
            break;
        case 2u:
            NetGatheringDdl_HandleCompositeSubtype2(self, nullptr, argument);
            break;
        case 3u:
            NetGatheringDdl_HandleCompositeSubtype3(self, nullptr, argument);
            break;
        case 5u:
            NetGatheringDdl_HandleCompositeSubtype5(self, nullptr, argument);
            break;
        case 7u:
            NetGatheringDdl_HandleCompositeSubtype7(self, nullptr, argument);
            break;
        case 8u:
            NetGatheringDdl_HandleCompositeSubtype8(self, nullptr, argument);
            break;
        default:
            return;
        }
        break;
    case 4u:
        NetGatheringDdl_HandleKind4(self, nullptr, argument);
        break;
    case 5u:
        NetGatheringDdl_HandleKind5(self, nullptr, argument);
        break;
    case 6u:
        NetGatheringDdl_HandleKind6(self, nullptr, argument);
        break;
    case 7u:
        NetGatheringDdl_HandleKind7(self, nullptr, argument);
        break;
    case 0xAu:
        NetGatheringDdl_HandleKind10(self, nullptr, argument);
        break;
    default:
        return;
    }
}

HOOK(0x006AB0E0, C_0085fe28_DispatchByKind, "auto");
