#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void, __fastcall, CGuiEdit_UpdateKeyRepeat, 0x00722A40,
               (CGuiEdit* self, void* edx, float deltaTime));

static void __fastcall CGuiEdit_UpdateCursorBlink(
    CGuiEdit* self, void* /*edx*/, float deltaTime)
{
    // Avoid keeping object state in a register across the original call.
    CGuiEdit* volatile savedSelf = self;
    CGuiEdit_UpdateKeyRepeat(savedSelf, nullptr, deltaTime);
    self = savedSelf;

    if (self->field_01b4 == 0xFFFFFFFFu)
    {
        const double elapsed =
            static_cast<double>(deltaTime) +
            static_cast<double>(self->field_0144);

        self->field_0144 = static_cast<float>(elapsed);

        // Compare the unrounded sum, not the stored float.
        if (elapsed >= static_cast<double>(self->m_blinkTime))
        {
            const unsigned char previousVisibility = self->field_013c;
            self->field_0144 = 0.0f;
            self->field_013c = previousVisibility == 0;
        }
    }
}

HOOK(0x00722B20, CGuiEdit_UpdateCursorBlink, "auto");
