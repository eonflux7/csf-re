#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00834598_ResolveAction0Id, 0x004A43A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction1Id, 0x004A4400, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction5Id, 0x004A4460, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction6Id, 0x004A44C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction10Id, 0x004A4520, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction15Id, 0x004A45D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction17Id, 0x004A4600, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction18Id, 0x004A4620, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction24Id, 0x004A4640, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction25Id, 0x004A46A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction26Id, 0x004A4700, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction27Id, 0x004A4760, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction53Id, 0x004A47C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00834598_TryResolveActionId, 0x004A4870, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00834598_ResolveAction11Id, 0x004A4B00, (void* self, void* edx));

static int __fastcall C_00834598_ResolveActionId(
    C_00834598* self, void* /*edx*/, int action)
{
    const int result = C_00834598_TryResolveActionId(self, nullptr, action);
    if (result != -1)
        return result;

    switch (action)
    {
    case 0:
        return C_00834598_ResolveAction0Id(self, nullptr);
    case 1:
        return C_00834598_ResolveAction1Id(self, nullptr);
    case 2:
        return CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x1C4) != 0 ? 66 : 28;
    case 3:
        return CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x1C4) != 0 ? 63 : 48;
    case 4:
        return CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x1C4) != 0 ? 66 : 52;
    case 5:
        return C_00834598_ResolveAction5Id(self, nullptr);
    case 6:
        return C_00834598_ResolveAction6Id(self, nullptr);
    case 8:
        return CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x1C4) != 0 ? 67 : 30;
    case 9:
        return CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x1C4) != 0 ? 68 : 31;
    case 10:
        return C_00834598_ResolveAction10Id(self, nullptr);
    case 11:
        return C_00834598_ResolveAction11Id(self, nullptr);
    case 15:
        return C_00834598_ResolveAction15Id(self, nullptr);
    case 17:
        return C_00834598_ResolveAction17Id(self, nullptr);
    case 18:
        return C_00834598_ResolveAction18Id(self, nullptr);
    case 20:
        return 39;
    case 21:
        return 40;
    case 22:
        return 41;
    case 23:
        return CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x1C4) != 0 ? 63 : 49;
    case 24:
        return C_00834598_ResolveAction24Id(self, nullptr);
    case 25:
        return C_00834598_ResolveAction25Id(self, nullptr);
    case 26:
        return C_00834598_ResolveAction26Id(self, nullptr);
    case 27:
        return C_00834598_ResolveAction27Id(self, nullptr);
    case 31:
        return 15;
    case 32:
        return 16;
    case 33:
        return 17;
    case 34:
        return 18;
    case 41:
        return 19;
    case 42:
        return CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x1C4) != 0 ? 73 : 9;
    case 47:
        return CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x1C4) != 0 ? 80 : 46;
    case 48:
        return 82;
    case 49:
        return 83;
    case 50:
        return 84;
    case 52:
        return 20;
    case 53:
        return C_00834598_ResolveAction53Id(self, nullptr);
    default:
        return 0;
    }
}

HOOK(0x004A52C0, C_00834598_ResolveActionId, "auto");
