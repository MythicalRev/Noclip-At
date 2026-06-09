#include <Geode/Geode.hpp>
#include "noclip/NoclipHandler.hpp"
#include "noclip/NoclipData.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>

class $modify(NCPlayLayer, PlayLayer) {
    struct Fields {
        NoclipHandler ncHandler;
        NoclipData ncData;

        std::set<std::pair<float, bool>> m_triggeredKeyframes; 
    };

    void updateProgressbar() {
        PlayLayer::updateProgressbar();

        float currentPlayerPercent = this->getCurrentPercent(); 

        auto currentKeyframes = m_fields->ncData.getNoclipKeyframes(this->m_level->m_levelName);

        log::debug("{}", currentKeyframes);

        for (const auto& keyframe : currentKeyframes) {
            float keyframePercent = std::get<0>(keyframe);
            bool toggleValue = std::get<1>(keyframe);

            if (currentPlayerPercent > keyframePercent) {
                std::pair<float, bool> runtimeId = { keyframePercent, toggleValue };

                if (!m_fields->m_triggeredKeyframes.contains(runtimeId)) {
                    m_fields->m_triggeredKeyframes.insert(runtimeId);
                    
                    m_fields->ncHandler.changeNoclip(toggleValue);
                }
            }
        }
    }

    void startGame() {
        PlayLayer::startGame();
        
        Mod::get()->setSavedValue<bool>("isNoclip", false);
        m_fields->m_triggeredKeyframes.clear();
    }
};