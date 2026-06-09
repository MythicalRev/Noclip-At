/*
    mUtils (created by miskaa)
    mUtils::format     - safe string formatting
    mUtils::truncation - truncation with ellipsis
    
    miskaas geode utils

    how to use:
    mutils::format::with("Hello, %s!", "world");
    mutils::truncation::withEllipsis("Hello, world!", 10);
*/

#include <cstdarg>
#include <cstdio>
#include <memory>

#include <Geode/Geode.hpp>
using namespace geode::prelude;

namespace mutils {
   class format {
        public:
        static std::string with(const std::string& format, ...) {
            va_list args;
            va_start(args, format);
            
            char buffer[1024];
            vsnprintf(buffer, sizeof(buffer), format.c_str(), args);
            
            va_end(args);
            return std::string(buffer);
        }
        
        template<typename... Args>
        static std::string args(const std::string& format, Args... args) {
            size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
            std::unique_ptr<char[]> buf(new char[size]);
            snprintf(buf.get(), size, format.c_str(), args...);
            return std::string(buf.get(), buf.get() + size - 1);
        }
   };
   
   class truncation {
        public:
        static std::string withEllipsis(const std::string& text, size_t maxLength) {
            if (text.length() <= maxLength) {
                return text;
            }
            
            if (maxLength <= 3) {
                return text.substr(0, maxLength);
            }
            
            return text.substr(0, maxLength - 3) + "...";
        }
        
        static std::string smart(const std::string& text, size_t maxLength) {
            if (text.length() <= maxLength) {
                return text;
            }
            
            if (maxLength <= 3) {
                return text.substr(0, maxLength);
            }
            
            size_t spacePos = text.rfind(' ', maxLength - 3);
            if (spacePos != std::string::npos && spacePos > 0) {
                return text.substr(0, spacePos) + "...";
            }
            
            return text.substr(0, maxLength - 3) + "...";
        }
        
        static std::string middle(const std::string& text, size_t maxLength) {
            if (text.length() <= maxLength) {
                return text;
            }
            
            if (maxLength <= 3) {
                return text.substr(0, maxLength);
            }
            
            size_t keepLength = (maxLength - 3) / 2;
            return text.substr(0, keepLength) + "..." + text.substr(text.length() - keepLength);
        }
   };
}

// The Unlicense license (do anything you want with this code loll)