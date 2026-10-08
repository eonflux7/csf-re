#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, MissionInfo_ResetPageObjectsStub, 0x00436EB0, (void* self, void* edx));
CSF_FUNC(uint8_t, __stdcall, CMenuMissionInfo_IsIndexOnLastPage, 0x0053D5D0, (int pageIndex));
// Original takes `this` in ecx, the new page index on the stack (ret 4), and a2 in edi.
// edi cannot be expressed in portable C++ here; see notes in the metadata.
CSF_FUNC(int, __fastcall, CMenuMissionInfo_RefreshPanelAtIndex, 0x0053E2F0, (void* self, void* edx, int newPageIndex));

// Original: 0x0053E9E0 (CMenuMissionInfo), ret 0.
// Field offsets 0x84 (controller object pointer) and 0x88 (current page index) are not
// declared in the class, so they are accessed through CSF_AT.
static char __fastcall CMenuMissionInfo_TryAdvanceToNextPage(void* self, void* /*edx*/)
{
    void* controller = CSF_AT(void*, self, 0x84);

    // Virtual slot 0xAC / 4 = 43: controller->IsReady() (thiscall, no stack args).
    auto controllerIsReady = reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[0xAC / 4]);

    if (!controllerIsReady(controller, nullptr)) {
        // Tail jump to 0x436EB0 with the controller as `this`.
        return static_cast<char>(MissionInfo_ResetPageObjectsStub(controller, nullptr));
    }

    char lastPageResult = CMenuMissionInfo_IsIndexOnLastPage(CSF_AT(int, self, 0x88));
    if (lastPageResult != 0) {
        return lastPageResult;
    }

    // Advance page index, store it, then refresh the page.
    uint32_t nextPageIndex = CSF_AT(uint32_t, self, 0x88) + 1u;
    CSF_AT(uint32_t, self, 0x88) = nextPageIndex;
    return static_cast<char>(CMenuMissionInfo_RefreshPanelAtIndex(self, nullptr, static_cast<int>(nextPageIndex)));
}

HOOK(0x0053E9E0, CMenuMissionInfo_TryAdvanceToNextPage, "auto");
