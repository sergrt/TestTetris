#pragma once
// std::optional is unavailable for cocos2d with MSVC, so using simple custom one
namespace Utilities {
    template<typename T>
    class optional {
    public:
        optional()
            : valid_{ false } {
        }
        optional(T t)
            : value_{ t }, valid_{ true } {
        }
        optional& operator=(const T& t) {
            value_ = t;
            valid_ = true;

            return *this;
        }
        optional& operator=(const T&& t) {
            value_ = t;
            valid_ = true;
            
            return *this;
        }
        T& operator*() {
            if (!valid_)
                throw std::runtime_error("Obtaining data from uninitialized optional");
            return value_;
        }
        const T& operator*() const {
            if (!valid_)
                throw std::runtime_error("Obtaining data from uninitialized optional");
            return value_;
        }
        operator bool() const{
            return valid_;
        }
        void reset() {
            valid_ = false;
        }
    private:
        T value_;
        bool valid_;
    };
}
