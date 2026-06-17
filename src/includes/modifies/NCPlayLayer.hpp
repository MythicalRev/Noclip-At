#include <Geode/Geode.hpp>
#include <Geode/binding/GameObject.hpp>
#include <format>
#include <string>
#include "Geode/cocos/actions/CCActionInterval.h"
#include "Geode/cocos/base_nodes/CCNode.h"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/loader/SettingV3.hpp"
#include "Geode/ui/Layout.hpp"
#include "Geode/utils/general.hpp"
#include "ccTypes.h"
#include "noclip/NoclipHandler.hpp"
#include "noclip/NoclipData.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

bool g_isRunTainted;
std::unordered_set<GameObject*> g_killList;
int g_deaths;

int totalFrames;
int deadFrames;

bool deadLastFrame = false;
bool wouldDieLastFrame = false;

class $modify(NCPlayLayer, PlayLayer) {
    struct Fields {
        NoclipHandler ncHandler;
        NoclipData ncData;

        std::set<std::pair<float, bool>> m_triggeredKeyframes; 

        CCMenu* statsMenu = nullptr;

        CCSprite* noclipIndicator = nullptr;
        CCLayerColor* tintNode = nullptr;
        CCLabelBMFont* deaths = nullptr;
        CCLabelBMFont* acc = nullptr;
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

        auto layout = ColumnLayout::create();
        layout->setGap(.5f);
        layout->setAxisReverse(true);
        layout->setAutoScale(false);
        layout->setAxisAlignment(AxisAlignment::End);
        layout->setCrossAxisAlignment(AxisAlignment::End);
        layout->setCrossAxisLineAlignment(AxisAlignment::End);

        m_fields->statsMenu = CCMenu::create();
        m_fields->statsMenu->setAnchorPoint({1, 1});
        m_fields->statsMenu->setContentSize({300,200});
        this->addChildAtPosition(m_fields->statsMenu, Anchor::TopRight, {-8, -8}, false);

        m_fields->noclipIndicator = CCSprite::create("noclipIndicator.png"_spr);
        if (m_fields->noclipIndicator) {
            m_fields->noclipIndicator->setScale(0.035f);
            m_fields->noclipIndicator->setOpacity(127);
            m_fields->noclipIndicator->setVisible(true);
            m_fields->noclipIndicator->setAnchorPoint({1, 1});
            m_fields->statsMenu->addChild(m_fields->noclipIndicator);
        }

        m_fields->acc = CCLabelBMFont::create("Accuracy: 100%", "bigFont.fnt");
        m_fields->acc->setScale(0.35f);
        m_fields->acc->setOpacity(127);
        m_fields->acc->setVisible(true);
        m_fields->acc->setAnchorPoint({1, 1});
        m_fields->statsMenu->addChild(m_fields->acc);

        m_fields->deaths = CCLabelBMFont::create("Deaths: 0", "bigFont.fnt");
        m_fields->deaths->setScale(0.35f);
        m_fields->deaths->setOpacity(127);
        m_fields->deaths->setVisible(true);
        m_fields->deaths->setAnchorPoint({1, 1});
        m_fields->statsMenu->addChild(m_fields->deaths);

        m_fields->statsMenu->setLayout(layout, true);

        g_isRunTainted = false;
        g_deaths = 0;
        g_killList.clear(); 

        auto wSize = CCDirector::get()->getWinSize();

        m_fields->tintNode = CCLayerColor::create();
        m_fields->tintNode->setColor(Mod::get()->getSettingValue<ccColor3B>("deathTint"));
        m_fields->tintNode->setOpacity(0);
        m_fields->tintNode->setContentSize(wSize);
        m_fields->tintNode->ignoreAnchorPointForPosition(true);

        this->addChild(m_fields->tintNode);

        return true;
    }

    void updateProgressbar() {
        PlayLayer::updateProgressbar();

        float currentPlayerPercent = this->getCurrentPercent(); 

        auto currentKeyframes = m_fields->ncData.getNoclipKeyframes(this->m_level);

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

        totalFrames++;

        if (m_fields->deaths && m_fields->acc) {
            m_fields->deaths->setString(fmt::format("Deaths: {}", g_deaths).c_str());
            
            float accuracy = (static_cast<float>(totalFrames - deadFrames) / totalFrames) * 100.0f;
            m_fields->acc->setString(fmt::format("Accuracy: {}%", accuracy, 2).c_str());
        }

        if (Mod::get()->getSavedValue<bool>("isNoclip")) {
            if (m_fields->statsMenu != nullptr) {
                m_fields->statsMenu->setVisible(true);
                g_isRunTainted = true;
            }
        }
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (Mod::get()->getSavedValue<bool>("isNoclip")) {
            wouldDieLastFrame = true;

            m_fields->tintNode->stopAllActions(); 
            m_fields->tintNode->setOpacity(50);
            m_fields->tintNode->runAction(CCFadeTo::create(0.5f, 0));

            return;
        }

        PlayLayer::destroyPlayer(player, object);
    }

    void resetLevel() {
        Mod::get()->setSavedValue<bool>("isNoclip", false);
        m_fields->m_triggeredKeyframes.clear();
        
        g_isRunTainted = false;
        g_deaths = 0;
        g_killList.clear(); 

        totalFrames = 0;
        deadFrames = 0;

        wouldDieLastFrame = false;

        if (m_fields->statsMenu) {
            m_fields->statsMenu->setVisible(false);
        }

        PlayLayer::resetLevel();
    }

    void levelComplete() {
        bool testMode = this->m_isTestMode;

        if (g_isRunTainted) {
            m_isTestMode = true;
        }

        PlayLayer::levelComplete();

        m_isTestMode = testMode;
    }

    void onQuit() {
        g_isRunTainted = false;
        g_deaths = 0;
        g_killList.clear();

        totalFrames = 0;
        deadFrames = 0;

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

class $modify(NoClipGJBGLHook, GJBaseGameLayer) {
    void processNoclipDeaths() {
        if (wouldDieLastFrame) {
            deadFrames++;
            if (!deadLastFrame) {
                g_deaths++;
            }
        }

        deadLastFrame = wouldDieLastFrame;
        wouldDieLastFrame = false;
    }

    #ifndef GEODE_IS_MACOS
    void processCommands(float dt, bool isHalfTick, bool isLastTick) {
        GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);
        this->processNoclipDeaths();
    }
    #else
    void processQueuedButtons(float dt, bool clearInputQueue) {
        GJBaseGameLayer::processQueuedButtons(dt, clearInputQueue);
        this->processNoclipDeaths();
    }
    #endif
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
        this->m_mainLayer->getChildByID("summary-container")->addChild(banner);
        this->m_mainLayer->getChildByID("summary-container")->updateLayout();
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