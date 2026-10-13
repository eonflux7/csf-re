#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees bound by address (names chosen here; signatures from the packet).
CSF_FUNC(char, __fastcall, CRenderEngine_RunPostSetupStep, 0x006DB9A0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CRenderEngine_FinishSetup, 0x006DBDB0, (void* self, void* edx));

// Copies the 0x2C-byte config block into the engine, runs the vtable-driven
// check (slot 0x6C), then the post-setup step and the finish step on success.
static char __fastcall CRenderEngine_CopyConfigAndApply(CRenderEngine* self, void* /*edx*/, int a2, const void* a3)
{
    std::memcpy(&self->field_0004, a3, 0x2C);

    // Slot 0x6C: validation taking a pointer to the stack argument a2.
    char result = ((char (__fastcall*)(void*, void*, int*))CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr, &a2);
    if (result)
    {
        // Slot 0x74: no-op hook.
        ((void (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x74 / 4])(self, nullptr);

        result = CRenderEngine_RunPostSetupStep(self, nullptr);
        if (result)
        {
            // Slot 0x78: no-op hook.
            ((void (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x78 / 4])(self, nullptr);

            CRenderEngine_FinishSetup(self, nullptr);
            return result;
        }
    }
    return result;
}

HOOK(0x006DBFA0, CRenderEngine_CopyConfigAndApply, "auto");
