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

// Callees bound by address.
CSF_FUNC(void, __fastcall, InitMenuScreenBase, 0x712CC0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ResetInnerBuffer, 0x501170, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AssignStdStringRange, 0x429B60, (void* self, void* edx, char* text, unsigned int length));

// Vtables and data referenced by the original, bound by address.
CSF_GLOBAL(uint8_t, g_VtblCMenuScreenBase, 0x83042C);
CSF_GLOBAL(uint8_t, g_VtblCMenuLoadGameSub, 0x842280);
CSF_GLOBAL(uint8_t, g_VtblCMenuLoadGame, 0x842220);
CSF_GLOBAL(uint8_t, g_EmptyStringLiteral, 0x82E65C);

// Vtable pointer values stored into the object (immediates from the original).
static constexpr uint32_t kVtblCMenuScreenBaseAddr = 0x0083042C;
static constexpr uint32_t kVtblCMenuLoadGameSubAddr = 0x00842280;
static constexpr uint32_t kVtblCMenuLoadGameAddr = 0x00842220;
static constexpr uint32_t kEmptyStringLiteralAddr = 0x0082E65C;

// CMenuLoadGame constructor: __thiscall with one stack argument (ret 4).
static CMenuLoadGame* __fastcall CMenuLoadGame_Construct(CMenuLoadGame* self, void* /*edx*/, int ownerArg)
{
    // Base vtable (0x83042C), then base sub-object init at this + 4.
    CSF_AT(uint32_t, self, 0) = kVtblCMenuScreenBaseAddr;
    InitMenuScreenBase(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    // Sub-object vtable (0x842280) stored at this + 4.
    self->field_0004 = kVtblCMenuLoadGameSubAddr;
    // Final vtable (0x842220) for CMenuLoadGame.
    CSF_AT(uint32_t, self, 0) = kVtblCMenuLoadGameAddr;

    // Inner string-like object at this + 0x1C: capacity 15, length 0, flag 0.
    uint8_t* innerStr = reinterpret_cast<uint8_t*>(self) + 0x1C;
    CSF_AT(uint32_t, innerStr, 0x18) = 0xF;
    CSF_AT(uint32_t, innerStr, 0x14) = 0;
    CSF_AT(uint8_t, innerStr, 0x04) = 0;

    // Sub-object at this + 0x38.
    ResetInnerBuffer(reinterpret_cast<uint8_t*>(self) + 0x38, nullptr);

    self->field_010c = 0;
    self->field_0110 = 0;
    self->field_0114 = 0;
    self->field_0118 = static_cast<uint32_t>(ownerArg);
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_011c = 0;
    self->field_0100 = 0;

    // Assign empty literal (0x82E65C), length 0, to the inner string at this + 0x1C.
    AssignStdStringRange(innerStr, nullptr,
                         reinterpret_cast<char*>(kEmptyStringLiteralAddr), 0);

    self->field_0104 = 0;

    // Virtual call through vtable slot 0x40 (index 16), ecx = this.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x40 / 4])(self, nullptr);

    return self;
}

HOOK(0x539A80, CMenuLoadGame_Construct, "auto");
