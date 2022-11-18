#pragma once

#include <mutex>
#include <memory>

namespace kiv_vss
{
    template<typename T>
    class Singleton
    {
    public:
        static T* Get_Instance()
        {
            std::call_once(s_init_flag, [=]() {
                Singleton<T>::s_instance = std::make_unique<T>();
            });

            return s_instance.get();
        }

    private:
        static std::unique_ptr<T> s_instance;
        static std::once_flag s_init_flag;
    };

    template<typename T>
    std::unique_ptr<T> Singleton<T>::s_instance = nullptr;

    template<typename T>
    std::once_flag Singleton<T>::s_init_flag = {};
}