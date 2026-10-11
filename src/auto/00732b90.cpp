#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_ValidateName, 0x00696BB0, (void* name, void* edx, const uint8_t* separator));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

static uint32_t* __fastcall C_008670f0_InitializeText(
    C_008670f0* self, void* /*edx*/, char* text)
{
    void* textInput = reinterpret_cast<uint8_t*>(self) + 0x20;
    self->field_0018 = 0;
    self->field_001c = 0;

    int result = SaveGame_ValidateName(
        textInput, nullptr, reinterpret_cast<const uint8_t*>(text));
    if (static_cast<uint8_t>(result) == 0)
        return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(result));

    self->field_0010 = 1;
    self->field_00d8 = 1;
    if (*text)
        return static_cast<uint32_t*>(TextInputSubobject_Init(
            textInput, nullptr, reinterpret_cast<uint8_t*>(text)));

    return static_cast<uint32_t*>(self->vSlot1C());
}

HOOK(0x00732B90, C_008670f0_InitializeText, "auto");
