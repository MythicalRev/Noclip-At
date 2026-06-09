/*
    mUtils (created by miskaa)
    mUtils::colorparse - convert hex -> ccColor3B

    miskaas geode utils
*/

#include <string>
#include <algorithm>
#include <cstdio>

#include <Geode/Geode.hpp>
using namespace geode::prelude;

namespace mutils {
    namespace colorparse {
        static cocos2d::ccColor3B fromHex(const std::string& hex) {
            cocos2d::ccColor3B color = {255, 255, 255};
            
            std::string cleanHex = hex;
            if (cleanHex.substr(0, 1) == "#") {
                cleanHex = cleanHex.substr(1);
            }
            
            if (cleanHex.length() == 3) {
                cleanHex = cleanHex.substr(0, 1) + cleanHex.substr(0, 1) +
                          cleanHex.substr(1, 1) + cleanHex.substr(1, 1) +
                          cleanHex.substr(2, 1) + cleanHex.substr(2, 1);
            }
            
            if (cleanHex.length() == 6) {
                try {
                    color.r = static_cast<GLubyte>(std::stoi(cleanHex.substr(0, 2), nullptr, 16));
                    color.g = static_cast<GLubyte>(std::stoi(cleanHex.substr(2, 2), nullptr, 16));
                    color.b = static_cast<GLubyte>(std::stoi(cleanHex.substr(4, 2), nullptr, 16));
                } catch (const std::exception&) {
                    // invalid
                }
            }
            
            return color;
        }
        
        static cocos2d::ccColor3B fromRgb(int r, int g, int b) {
            return {
                static_cast<GLubyte>(std::min(255, std::max(0, r))),
                static_cast<GLubyte>(std::min(255, std::max(0, g))),
                static_cast<GLubyte>(std::min(255, std::max(0, b)))
            };
        }
        
        static std::string toHex(const cocos2d::ccColor3B& color) {
            char buffer[8];
            snprintf(buffer, sizeof(buffer), "#%02X%02X%02X", color.r, color.g, color.b);
            return std::string(buffer);
        }
        
        static cocos2d::ccColor3B lerp(const cocos2d::ccColor3B& a, const cocos2d::ccColor3B& b, float t) {
            t = std::min(1.0f, std::max(0.0f, t));
            return {
                static_cast<GLubyte>(a.r + (b.r - a.r) * t),
                static_cast<GLubyte>(a.g + (b.g - a.g) * t),
                static_cast<GLubyte>(a.b + (b.b - a.b) * t)
            };
        }
    }
}