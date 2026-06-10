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
        CCSprite* noclipIndicator = nullptr;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        Mod::get()->setSavedValue<bool>("isNoclip", false);
        m_fields->m_triggeredKeyframes.clear();

        m_fields->noclipIndicator = CCSprite::create("noclipIndicator.png"_spr);
        if (m_fields->noclipIndicator) {
            m_fields->noclipIndicator->setScale(0.035f);
            m_fields->noclipIndicator->setOpacity(127);
            m_fields->noclipIndicator->setVisible(false);
            this->addChildAtPosition(m_fields->noclipIndicator, Anchor::TopRight, ccp(-8, -8), false);
        }

        return true;
    }

    void updateProgressbar() {
        PlayLayer::updateProgressbar();

        float currentPlayerPercent = this->getCurrentPercent(); 

        auto currentKeyframes = m_fields->ncData.getNoclipKeyframes(this->m_level->m_levelName);

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

        if (Mod::get()->getSavedValue<bool>("isNoclip") == true) {
            if (m_fields->noclipIndicator != nullptr) {
                m_fields->noclipIndicator->setVisible(true);
            }
        } else {
            if (m_fields->noclipIndicator != nullptr) {
                m_fields->noclipIndicator->setVisible(false);
            }
        }
    }

    void resetLevel() {
        Mod::get()->setSavedValue<bool>("isNoclip", false);
        m_fields->m_triggeredKeyframes.clear();
        
        PlayLayer::resetLevel();
    }
};