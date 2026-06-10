#include <Geode/Geode.hpp>
#include <Geode/binding/GameObject.hpp>
#include "noclip/NoclipHandler.hpp"
#include "noclip/NoclipData.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>

bool g_isRunTainted;
GameObject* g_lastKill = nullptr;
int g_deaths;

class $modify(NCPlayLayer, PlayLayer) {
    struct Fields {
        NoclipHandler ncHandler;
        NoclipData ncData;

        std::set<std::pair<float, bool>> m_triggeredKeyframes; 
        CCSprite* noclipIndicator = nullptr;
    };

    static void onModify(auto& self) {
        if (!self.setHookPriorityPre("PlayLayer::destroyPlayer", -INT_MAX)) {
            log::warn("Failed to set hook priority for destroyPlayer");
        }
    }

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

        g_isRunTainted = false;
        g_deaths = 0;

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
                g_isRunTainted = true;
            }
        } else {
            if (m_fields->noclipIndicator != nullptr) {
                m_fields->noclipIndicator->setVisible(false);
            }
        }
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (Mod::get()->getSavedValue<bool>("isNoclip")) {
            player->m_isDead = false;
            if (g_lastKill != object) {
                g_deaths++;
                g_lastKill = object;
            }
            return;
        }

        PlayLayer::destroyPlayer(player, object);
    }

    void resetLevel() {
        Mod::get()->setSavedValue<bool>("isNoclip", false);
        m_fields->m_triggeredKeyframes.clear();
        
        g_isRunTainted = false;
        g_deaths = 0;

        PlayLayer::resetLevel();
    }

    void onQuit() {
        g_isRunTainted = false;
        g_deaths = 0;

        PlayLayer::onQuit();
    }

    void showSafemodeEnd(float) {
        this->showEndLayer();
    }

    void showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle) {
        if (g_isRunTainted) return;
        PlayLayer::showNewBest(newReward, orbs, diamonds, demonKey, noRetry, noTitle);
    }
};

#include <Geode/modify/EndLevelLayer.hpp>
class $modify(SafeModeEndLevel, EndLevelLayer) {
 
    void customSetup() {
        EndLevelLayer::customSetup();
 
        if (!g_isRunTainted) return;
 
        auto winSize = CCDirector::sharedDirector()->getWinSize();
 
        auto* banner = CCLabelBMFont::create(
            fmt::format("Noclip Deaths: {}", g_deaths).c_str(), "goldFont.fnt"
        );
        banner->setScale(0.7f);
        banner->setColor({ 255, 80, 80 });
        banner->setPosition({ winSize.width / 2.f, winSize.height / 2.f - 40.f });
        banner->setZOrder(10);
        this->m_mainLayer->addChild(banner);
    }
};
 
#include <Geode/modify/GJGameLevel.hpp>

class $modify(SafeModeGJGameLevel, GJGameLevel) {
    void savePercentage(int percent, bool isPracticeMode, int clicks, int attempts, bool isChkValid) {
        if (g_isRunTainted) return;
        GJGameLevel::savePercentage(percent, isPracticeMode, clicks, attempts, isChkValid);
    }
};
 
#include <Geode/modify/GameStatsManager.hpp>

class $modify(SafeModeStatsManager, GameStatsManager) {
 
    void completedLevel(GJGameLevel* level) {
        if (g_isRunTainted) return;
        GameStatsManager::completedLevel(level);
    }
 
    void setStat(char const* key, int value) {
        if (g_isRunTainted) return;
        GameStatsManager::setStat(key, value);
    }
};