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

CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(int, __fastcall, sub_6B8230, 0x006B8230, (void* self, void* edx));

// __thiscall wrapper: this->field_0004 is passed to SceneLoader_GetObjectAttachment,
// and the resulting attachment is tail-jumped into sub_6B8230 (ecx = attachment, edx untouched).
static int __fastcall SkinGeometry_RunAttachmentOperation(void* self, void* /*edx*/)
{
    const uint32_t objectHandle = CSF_AT(uint32_t, self, 0x4);
    void* attachment = reinterpret_cast<void*>(
        static_cast<intptr_t>(SceneLoader_GetObjectAttachment(static_cast<int>(objectHandle))));
    return sub_6B8230(attachment, nullptr);
}

HOOK(0x006C6220, SkinGeometry_RunAttachmentOperation, "auto");
