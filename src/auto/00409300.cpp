#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, C_0082d844_GetModeCode, 0x004092A0, (void* self, void* edx));

static void __fastcall C_0082d844_RefreshChildPosition(C_0082d844* self, void* /*edx*/)
{
    using ObjGetter = void* (__fastcall*)(void*, void*);
    using ObjSetter = void (__fastcall*)(void*, void*, void*);
    using FloatSetter = void (__fastcall*)(void*, void*, float);

    C_00833908* parent = reinterpret_cast<C_00833908*>(self->field_0008);
    if (parent == nullptr) {
        return;
    }

    void* stateObj = ((ObjGetter)CSF_AT(void**, parent, 0)[0x28 / 4])(parent, nullptr);
    if (stateObj != nullptr) {
        void* targetObj = ((ObjGetter)CSF_AT(void**, stateObj, 0)[0x18 / 4])(stateObj, nullptr);
        if (targetObj != nullptr) {
            if (C_0082d844_GetModeCode(self, nullptr) == 1) {
                void* source = ((ObjGetter)CSF_AT(void**, targetObj, 0)[0x18 / 4])(targetObj, nullptr);
                const uint32_t* sourceWords = static_cast<const uint32_t*>(source);

                uint32_t transform[3];
                transform[1] = sourceWords[1];
                transform[2] = sourceWords[2];
                const float offset = self->field_0010;
                std::memcpy(&transform[0], &offset, sizeof(float));

                ((ObjSetter)CSF_AT(void**, targetObj, 0)[0x1c / 4])(targetObj, nullptr, transform);
                self->field_0024 = 1;
                return;
            }

            const float target = self->field_0010 + self->field_000c;
            ((FloatSetter)CSF_AT(void**, targetObj, 0)[0x34 / 4])(targetObj, nullptr, target);
        }
    }

    self->field_0024 = 1;
}

HOOK(0x00409300, C_0082d844_RefreshChildPosition, "auto");
