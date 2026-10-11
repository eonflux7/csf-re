#include <cstdint>
#include "hook.h"

// Returns 1 for scene object type ids that the game may remove from a scene, 0 otherwise.
static int GameConfig_IsSceneObjectRemovable(int objectType) {
    switch (objectType) {
    case 8:
    case 9:
    case 10:
    case 11:
    case 14:
    case 15:
    case 16:
    case 17:
    case 20:
    case 21:
        return 1;
    default:
        return 0;
    }
}

HOOK(0x007558C0, GameConfig_IsSceneObjectRemovable, "auto");
