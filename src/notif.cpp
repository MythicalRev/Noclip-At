#define NOTIF_API_EXPORT
#include "includes/notif.hpp"
#include "includes/mutils/index.hpp"

using namespace geode::prelude;

namespace notifapi {

    cocos2d::ccColor3B typecolor(const std::string& type) {
        if (type == "info") return {50, 125, 255};
        if (type == "warning") return {255, 200, 50};
        if (type == "error") return {255, 50, 50};
        if (type == "success") return {50, 255, 50};
        if (type == "loading") return {100, 100, 255};
        return {100, 100, 100};
    }

    cocos2d::CCNode* makeIcon(const std::string& type) {
        if (type == "loading") {
            return geode::LoadingSpinner::create(20.f);
        } else if (type == "info") {
            return cocos2d::CCSprite::create("icon-info.png"_spr);
        } else if (type == "warning") {
            return cocos2d::CCSprite::create("icon-warning.png"_spr);
        } else if (type == "error") {
            return cocos2d::CCSprite::create("icon-error.png"_spr);
        } else if (type == "success") {
            return cocos2d::CCSprite::create("icon-success.png"_spr);
        }
        return nullptr;
    }

    bool notif::init(const std::string& text, const std::string& type, float time, cocos2d::ccColor3B accentColor, float scale, Position position, Animation animation, const std::string& customSound, float volume) {
        if (!cocos2d::CCNodeRGBA::init()) return false;
        
        m_time = time;
        m_scale = scale;
        m_position = position;
        m_animation = animation;
        m_customSound = customSound;
        m_volume = volume;
        this->setCascadeOpacityEnabled(true);
        
        // mmm calc
        auto tempLabel = cocos2d::CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
        tempLabel->setScale(0.4f * scale);
        float textWidth = tempLabel->getContentSize().width * 0.4f * scale;

        const float minWidth = 200.f * scale;
        const float maxWidth = 400.f * scale;
        const float iconWidth = 44.f * scale;
        const float accentWidth = 6.f * scale;
        const float textPadding = 20.f * scale;
        
        float calculatedWidth = std::min(maxWidth, std::max(minWidth, textWidth + iconWidth + accentWidth + textPadding));
        
        // sizes
        const float width = calculatedWidth;
        const float height = 35.f * scale;
        
        // bg sprite
        auto bg = geode::NineSlice::create("square02b_small.png", {0, 0, 40, 40});
        bg->setContentSize({width, height});
        bg->setPosition({width/2, height/2});
        bg->setColor({20, 20, 20});
        bg->setOpacity(200);
        bg->setID("background");
        this->addChild(bg);

        // idk
        auto fancyroundedrectangleidk = geode::NineSlice::create("square02b_small.png", {0, 0, 40, 40});
        fancyroundedrectangleidk->setContentSize({width, height});
        fancyroundedrectangleidk->setPosition({width/2, height/2});

        auto clipper = cocos2d::CCClippingNode::create();
        clipper->setStencil(fancyroundedrectangleidk);
        clipper->setAlphaThreshold(0.1f);
        clipper->setContentSize({width, height});
        this->addChild(clipper);

        // accent bar
        auto color = (accentColor.r == 0 && accentColor.g == 0 && accentColor.b == 0) 
            ? typecolor(type) 
            : accentColor;
        
        float bWidth = 6.f * scale;
        auto accent = cocos2d::CCLayerColor::create({color.r, color.g, color.b, 255}, bWidth, height);
        accent->setPosition({width - bWidth, 0});
        clipper->addChild(accent);

        // icon
        auto icon = makeIcon(type);
        if (icon) {
            icon->setPosition({22 * scale, height/2});
            icon->setScale(0.7f * scale);
            this->addChild(icon);
        }
        
        // text
        auto label = cocos2d::CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
        label->setPosition({(width/2) + (8 * scale), height/2});
        label->setScale(0.4f * scale);
        
        float availableTextWidth = width - iconWidth - accentWidth - textPadding;
        label->limitLabelWidth(availableTextWidth, 0.4f * scale, 0);
        this->addChild(label);
        
        this->setContentSize({width, height});
        return true;
    }
    
    class notif* notif::create(const std::string& text, const std::string& type, float time, cocos2d::ccColor3B accentColor, float scale, Position position, Animation animation, const std::string& customSound, float volume) {
        auto ret = new notif();
        if (ret && ret->init(text, type, time, accentColor, scale, position, animation, customSound, volume)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    
    void notif::show() {
        // auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
        // if (!scene) return;

        // overlay manager since geode needs it
        auto overlay = geode::OverlayManager::get();
        if (!overlay) return;
        
        auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
        auto size = this->getContentSize();
        
        cocos2d::CCPoint targetPos;
        cocos2d::CCPoint startPos;
        float margin = 15.f;
        
        switch (m_position) {
            case Position::TopRight:
                targetPos = cocos2d::CCPoint(winSize.width - size.width - margin, winSize.height - size.height - margin);
                startPos = cocos2d::CCPoint(winSize.width + 50, targetPos.y);
                break;
            case Position::TopLeft:
                targetPos = cocos2d::CCPoint(margin, winSize.height - size.height - margin);
                startPos = cocos2d::CCPoint(-50, targetPos.y);
                break;
            case Position::BottomRight:
                targetPos = cocos2d::CCPoint(winSize.width - size.width - margin, margin);
                startPos = cocos2d::CCPoint(winSize.width + 50, targetPos.y);
                break;
            case Position::BottomLeft:
                targetPos = cocos2d::CCPoint(margin, margin);
                startPos = cocos2d::CCPoint(-50, targetPos.y);
                break;
            case Position::TopCenter:
                targetPos = cocos2d::CCPoint(winSize.width/2 - size.width/2, winSize.height - size.height - margin);
                startPos = cocos2d::CCPoint(targetPos.x, winSize.height + 50);
                break;
            case Position::BottomCenter:
                targetPos = cocos2d::CCPoint(winSize.width/2 - size.width/2, margin);
                startPos = cocos2d::CCPoint(targetPos.x, -50);
                break;
            case Position::Center:
                targetPos = cocos2d::CCPoint(winSize.width/2 - size.width/2, winSize.height/2 - size.height/2);
                startPos = targetPos;
                break;
        }
        
        this->setPosition(startPos);
        overlay->addChild(this, 9000);
        
        // sound system/notification
        auto fmod = FMODAudioEngine::sharedEngine();
        if (fmod && fmod->getEffectsVolume() > 0.0f) {
            float finalVolume = m_volume * fmod->getEffectsVolume();
            if (!m_customSound.empty()) {
                fmod->playEffect(m_customSound.c_str(), 1.0f, 1.0f, finalVolume);
            } else {
                auto sound = geode::Mod::get()->getResourcesDir() / "notif.ogg";
                fmod->playEffect(utils::string::pathToString(sound).c_str(), 1.0f, 1.0f, finalVolume);
            }
        }
        
        cocos2d::CCAction* showAnim = nullptr;
        cocos2d::CCAction* hideAnim = nullptr;
        
        switch (m_animation) {
            case Animation::Slide:
                showAnim = cocos2d::CCEaseBackOut::create(cocos2d::CCMoveTo::create(0.4f, targetPos));
                hideAnim = cocos2d::CCEaseIn::create(cocos2d::CCMoveTo::create(0.4f, startPos), 2.0f);
                break;
            // this one down here doiesnt work 3:
            case Animation::Fade:
                this->setOpacity(0);
                showAnim = cocos2d::CCFadeIn::create(0.4f);
                hideAnim = cocos2d::CCSequence::create(
                    cocos2d::CCDelayTime::create(0.2f),
                    cocos2d::CCFadeOut::create(0.4f),
                    nullptr
                );
                break;
            case Animation::Bounce:
                showAnim = cocos2d::CCSequence::create(
                    cocos2d::CCJumpTo::create(0.6f, targetPos, 30.f, 1),
                    nullptr
                );
                hideAnim = cocos2d::CCSequence::create(
                    cocos2d::CCJumpTo::create(0.4f, startPos, 50.f, 2),
                    nullptr
                );
                break;
            case Animation::Elastic:
                showAnim = cocos2d::CCEaseElasticOut::create(cocos2d::CCMoveTo::create(0.8f, targetPos));
                hideAnim = cocos2d::CCEaseElasticIn::create(cocos2d::CCMoveTo::create(0.6f, startPos));
                break;
            // this one down here doiesnt work 3:
            case Animation::Scale:
                this->setScale(0);
                showAnim = cocos2d::CCSequence::create(
                    cocos2d::CCDelayTime::create(0.1f),
                    cocos2d::CCEaseBackOut::create(cocos2d::CCScaleTo::create(0.4f, m_scale)),
                    nullptr
                );
                hideAnim = cocos2d::CCSequence::create(
                    cocos2d::CCDelayTime::create(0.2f),
                    cocos2d::CCEaseBackIn::create(cocos2d::CCScaleTo::create(0.4f, 0)),
                    nullptr
                );
                break;
            case Animation::None:
                this->setPosition(targetPos);
                showAnim = nullptr;
                hideAnim = cocos2d::CCMoveTo::create(0.1f, startPos);
                break;
        }

        if (showAnim) {
            this->runAction(showAnim);
        }
        
        this->runAction(cocos2d::CCSequence::create(
            cocos2d::CCDelayTime::create(m_time),
            hideAnim ? hideAnim : cocos2d::CCCallFunc::create(this, callfunc_selector(notif::hide)),
            nullptr
        ));
    }
    
    void notif::hide() {
        destroyed();
        this->removeFromParent();
    }
    
    void notif::destroyed() {
    }
    
    void fnotif(const std::string& text, const std::string& type, float time, cocos2d::ccColor3B accentColor, float scale, Position position, Animation animation, const std::string& customSound, float volume) {
        auto notifObj = notif::create(text, type, time, accentColor, scale, position, animation, customSound, volume);
        if (notifObj) {
            notifObj->show();
        }
    }

    void notify(const std::string& text, const std::string& type) {
        fnotif(text, type, 3.0f, {0, 0, 0}, 1.0f, Position::TopRight, Animation::Slide, "");
    }

    void info(const std::string& text) { fnotif(text, "info", 3.0f, {0, 0, 0}, 1.0f, Position::TopRight, Animation::Slide, ""); }
    void warn(const std::string& text) { fnotif(text, "warning", 3.0f, {0, 0, 0}, 1.0f, Position::TopRight, Animation::Slide, ""); }
    void error(const std::string& text) { fnotif(text, "error", 3.0f, {0, 0, 0}, 1.0f, Position::TopRight, Animation::Slide, ""); }
    void success(const std::string& text) { fnotif(text, "success", 3.0f, {0, 0, 0}, 1.0f, Position::TopRight, Animation::Slide, ""); }
    void loading(const std::string& text) { fnotif(text, "loading", 3.0f, {0, 0, 0}, 1.0f, Position::TopRight, Animation::Slide, ""); }
}