#pragma once

#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(NoclipPlayLayer, PlayLayer) {
    static void onModify(auto& self) {
        if (!self.setHookPriorityPre("PlayLayer::destroyPlayer", -INT_MAX)) {
            log::warn("Failed to set hook priority for destroyPlayer");
        }
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (Mod::get()->getSavedValue<bool>("isNoclip")) {
            player->m_isDead = false;
            return;
        }

        PlayLayer::destroyPlayer(player, object);
    }
};

class NoclipHandler {
public:
    void changeNoclip(bool changeNoclip);
};
