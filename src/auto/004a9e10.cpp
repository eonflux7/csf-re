#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, WaypointGraph_VisitNodePair, 0x0075D2C0, (S_004a9e10* self, void* , uint8_t* ctx));

static int __fastcall S_004a9e10_CheckNodeContext(
    S_004a9e10* self, void* /*edx*/, S_004a9e10* context)
{
    const uint32_t savedLink = CSF_AT(uint32_t, context, 0x1C);

    bool blocked = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0) {
        void* owner = CSF_AT(void*, context, 0x04);
        blocked = !((bool(__fastcall*)(void*, void*, int, S_004a9e10*))
            CSF_AT(void**, owner, 0)[0x80 / 4])(
                owner, nullptr, 0, context);
    }

    CSF_AT(uint8_t, context, 0x00) = blocked;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!blocked) {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        WaypointGraph_VisitNodePair(
            self, nullptr, reinterpret_cast<uint8_t*>(context));
    }

    const bool needsContextCheck = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (needsContextCheck) {
        void* owner = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;

        const int result =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_004a9e10*))
                CSF_AT(void**, owner, 0)[0x38 / 4])(
                    owner, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 8, context);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }

    const bool needsFinalCheck = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedLink;

    if (needsFinalCheck) {
        void* owner = CSF_AT(void*, context, 0x04);
        // Only the low byte is tested, but the full return value is forwarded.
        const int result =
            ((int(__fastcall*)(void*, void*, S_004a9e10*))
                CSF_AT(void**, owner, 0)[0x7C / 4])(
                    owner, nullptr, context);
        if (static_cast<uint8_t>(result) != 0) {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004A9E10, S_004a9e10_CheckNodeContext, "auto");
