#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, S_00493c70_IsFlag1000Allowed, 0x00493C70,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, S_00493c70_RefreshPendingFlags, 0x004948D0,
         (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// The packet also identifies the predicate's address as a vtable reference.
[[maybe_unused]] static const auto g_ReportedVtable = CSF_VTBL(0x00493C70);

static int __fastcall S_00493c70_FilterPendingFlags(
    S_00493c70* self, void* /*edx*/)
{
    self->field_02e8 = self->field_02e4;
    S_00493c70_RefreshPendingFlags(self, nullptr);

    if ((self->field_02e8 & 0x2000) != 0) {
        self->field_02e8 &= 0xFFF9F014;
        if ((self->field_02e8 & 0x1000) != 0 &&
            !S_00493c70_IsFlag1000Allowed(self, nullptr)) {
            self->field_02e8 &= ~0x1000u;
        }
        if ((self->field_02e0 & 0x2000) == 0)
            self->field_02e8 &= ~0x10u;
    }

    const uint32_t activeFlags = self->field_02e0;

    if ((activeFlags & 0x800) != 0 &&
        (self->field_02e4 & 2) != 0 &&
        ((activeFlags & 2) == 0 || self->field_02f0 == g_fZero)) {
        self->field_02e8 &= ~2u;
    }

    if ((self->field_02e8 & 0x10000) != 0) {
        if (self->field_03d0)
            self->field_02e8 &= 0x93404;
        else
            self->field_02e8 &= 0x93504;
    }

    if ((activeFlags & 0x4000) != 0)
        self->field_02e8 &= 0xFFF7FFBF;

    if ((self->field_02e8 & 0x80u) != 0 || (activeFlags & 0x80u) != 0)
        self->field_02e8 &= 0xFFF9FED7;

    if ((self->field_02e8 & 0x80u) != 0)
        self->field_02e8 &= 0xFFFFFFBF;

    if ((self->field_02e8 & 0x40000) != 0)
        self->field_02e8 &= 0xFFFDFE67;

    if ((self->field_02dc & 0x80000000) == 0)
        self->field_02e8 &= 0xFFFFFD31;

    if ((self->field_02e8 & 0x40) != 0)
        self->field_02e8 &= 0xFFFDFFF7;

    if ((activeFlags & 8) != 0)
        self->field_02e8 &= 0xFFF7FD1B;

    if ((self->field_02e8 & 0x10) != 0) {
        if ((self->field_02e8 & 4) != 0)
            self->field_02e8 &= 0xFFFFFFF6;
        self->field_02e8 &= 0xFFF7FFFD;
    }

    if ((self->field_02e8 & 1) != 0) {
        if ((self->field_02e8 & 2) != 0)
            self->field_02e8 &= 0xFFF9FE8B;
    } else {
        self->field_02e8 &= 0xFFFFFFFD;
    }

    if ((self->field_02e8 & 0x20) != 0)
        self->field_02e8 &= 0xFFF7FD64;

    if ((activeFlags & 0x20) != 0)
        self->field_02e8 &= ~0x40u;

    if ((self->field_02e8 & 0x20000) != 0)
        self->field_02e8 &= 0xFFFBFC6F;

    if ((self->field_02e8 & 0x100) != 0)
        self->field_02e8 &= 0xFFF9FD6F;

    uint32_t result = self->field_02e8;
    if ((result & 0x200) != 0 || (activeFlags & 0x200) != 0) {
        result &= 0xFFFFFF07;
        self->field_02e8 = result;
    }
    return static_cast<int>(result);
}

HOOK(0x00496940, S_00493c70_FilterPendingFlags, "auto");
