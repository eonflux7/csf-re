#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, SceneLoader_SetPrimarySceneIndex, 0x00492BA0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_SetSecondarySceneIndex, 0x00492BB0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_AssociateEntryRecord, 0x004A1060,
         (void* self, void* edx, int a2));

static int __fastcall CGuiControl_AssociateSceneEntry(
    CGuiControl* self, void* /*edx*/, int mode, int entryAddress, int sceneIndex)
{
    void* entry = reinterpret_cast<void*>(entryAddress);
    int result = CSF_AT(int, entry, 0x08);
    if (result < 0 || result >= static_cast<int>(self->m_id.alloc))
        return result;

    void* object = reinterpret_cast<void**>(self->field_0010)[result];
    if (!object)
        return result;

    result = CSF_AT(int, object, 0x20);
    if (result != CSF_AT(int, entry, 0x04))
        return result;

    result = CSF_AT(int, object, 0x170);
    if ((result & 0x2000) == 0)
        return result;

    result = mode;
    if (mode == 0)
    {
        if (CSF_AT(int, object, 0x1F4) != sceneIndex)
        {
            void* record = reinterpret_cast<void*>(
                self->field_00cc + static_cast<std::uint32_t>(sceneIndex) * 0x30);
            SceneLoader_AssociateEntryRecord(record, nullptr, entryAddress);
            return SceneLoader_SetPrimarySceneIndex(object, nullptr, sceneIndex);
        }
    }
    else if (mode == 1 && CSF_AT(int, object, 0x1F8) != sceneIndex)
    {
        void* record = reinterpret_cast<void*>(
            self->field_00cc + static_cast<std::uint32_t>(sceneIndex) * 0x30);
        SceneLoader_AssociateEntryRecord(record, nullptr, entryAddress);
        return SceneLoader_SetSecondarySceneIndex(object, nullptr, sceneIndex);
    }

    return result;
}

HOOK(0x00452A80, CGuiControl_AssociateSceneEntry, "auto");
