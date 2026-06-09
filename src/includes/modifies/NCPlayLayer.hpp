#include <Geode/Geode.hpp>
#include "noclip/NoclipHandler.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>

class $modify(NCPlayLayer, PlayLayer) {
    /*void update(float dt) {
        PlayLayer::update(dt);

        NoclipHandler ncHandler;

        int levelID = this->m_level->m_levelID.value();
        auto& session = NoclipData::get()->levelSettings;

        if (session.find(levelID) != session.end()) {
            auto& setting = session[levelID];

            if (!setting.hasTriggered) {
                float currentPercent = this->getCurrentPercent();

                if (currentPercent >= setting.targetPercent) {
                    ncHandler.changeNoclip(setting.enableNoclip);
                    log::info("Threshold hit! Setting noclip to {}", setting.enableNoclip);
                    setting.hasTriggered = true; 
                }
            }
        }
    }*/

    void startGame() {
        PlayLayer::startGame();

        Mod::get()->setSavedValue<bool>("isNoclip", false);
    }
};