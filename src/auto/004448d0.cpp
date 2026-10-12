#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Scene_LookupEntryByKey, 0x00468750, (S_00468750* self, void* /*edx*/, int key));
CSF_FUNC(char, __fastcall, MissionScript_ProcessWithZeroFlag, 0x00468240, (S_00467cc0* self, void* /*edx*/));

// The original is __stdcall with one stack argument, but it reads its scene
// pointer from the incoming ecx (the object at +4). Written as a fastcall
// method: ecx = self, one stack argument (key), so ret 4 is preserved.
static char __fastcall TryAcceptSceneTarget(S_00468750* self, void* /*edx*/, S_00468750* key)
{
    // Original never reads the vtable here; this reference only binds the
    // address the packet lists for this function.
    (void)CSF_VTBL(0x00468750);

    S_00468750* scene = CSF_AT(S_00468750*, self, 0x4);
    int entry = Scene_LookupEntryByKey(scene, nullptr,
                                       static_cast<int>(reinterpret_cast<uintptr_t>(key)));
    if (entry == 0)
        return 0;

    MissionScript_ProcessWithZeroFlag(reinterpret_cast<S_00467cc0*>(static_cast<uintptr_t>(entry)), nullptr);
    return 1;
}

HOOK(0x004448D0, TryAcceptSceneTarget, "auto");
