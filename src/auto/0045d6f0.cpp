#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(char, __fastcall, IsOverlayEnabled, 0x00521C00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, PushOverlayRect, 0x00522460, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, LookupEntitySlotIndex, 0x007686E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetEntityRecordByIndex, 0x00768740, (void* self, void* edx, int a2));

static char __stdcall ApplyEntityRectToOverlayIfEnabled(int /*firstArg*/, int /*secondArg*/, uint32_t* eventArgs)
{
    // eventArgs[0] -> object whose +4 is an entity reference; that reference's +4 is the entity id.
    uint32_t entityRef = CSF_AT(uint32_t, reinterpret_cast<void*>(eventArgs[0]), 4);

    void* entityManager = reinterpret_cast<void*>(GetValue8B0554());
    uint32_t entityId = CSF_AT(uint32_t, reinterpret_cast<void*>(entityRef), 4);
    int entitySlot = LookupEntitySlotIndex(CSF_AT(void*, entityManager, 0x1F90), nullptr, static_cast<int>(entityId));
    if (entitySlot == -1) {
        return static_cast<char>(entitySlot);
    }

    // The original fetches the manager again before reading the entity record.
    void* entityManagerAgain = reinterpret_cast<void*>(GetValue8B0554());
    void* entityRecord = reinterpret_cast<void*>(
        GetEntityRecordByIndex(CSF_AT(void*, entityManagerAgain, 0x1F90), nullptr, entitySlot));
    float rectX = CSF_AT(float, entityRecord, 4);
    float rectY = CSF_AT(float, entityRecord, 8);
    float rectZ = CSF_AT(float, entityRecord, 0xC);

    // eventArgs[1] -> object whose +4 is a pointer; that pointer's +4 is the fourth rect component.
    uint32_t extraRef = CSF_AT(uint32_t, reinterpret_cast<void*>(eventArgs[1]), 4);
    float rectW = CSF_AT(float, reinterpret_cast<void*>(extraRef), 4);

    void* overlayGlobal = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    char overlayEnabled = IsOverlayEnabled(overlayGlobal, nullptr);
    if (overlayEnabled == 0) {
        return overlayEnabled;
    }

    float rectBounds[6];
    rectBounds[0] = rectX;
    rectBounds[1] = rectY;
    rectBounds[2] = rectZ;
    rectBounds[3] = rectW;
    void* overlayGlobalAgain = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    return static_cast<char>(PushOverlayRect(overlayGlobalAgain, nullptr, rectBounds));
}

HOOK(0x0045D6F0, ApplyEntityRectToOverlayIfEnabled, "auto");
