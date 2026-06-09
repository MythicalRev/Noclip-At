/*
    mUtils (created by miskaa)
    
    will be used by ../ui.hpp

    miskaas geode utils
*/

namespace mutils {
    namespace ui {
        namespace animation {
            namespace sine {
                static float in(float t) { return 1.0f - cosf((t * M_PI) / 2.0f); }
                static float out(float t) { return sinf((t * M_PI) / 2.0f); }
                static float inOut(float t) { return -(cosf(M_PI * t) - 1.0f) / 2.0f; }
            }
        }
    }
}

// The Unlicense license (do anything you want with this code loll)