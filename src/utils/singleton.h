#pragma once

#include <mutex>
#include <memory>

namespace kiv_vss
{
    /// \author Jakub Silhavy
    /// Templated singleton class. It is a utility class
    /// that can treat any class as a singleton (global variable). 
    template<typename T>
    class Singleton
    {
    public:
        /// Returns instance of class T.
        /// \return instance of the class passed in as a template
        static T* Get_Instance()
        {
            // Make sure the class gets instantiated only once.
            std::call_once(s_init_flag, [=]() {
                Singleton<T>::s_instance = std::make_unique<T>();
            });

            // Return the instance.
            return s_instance.get();
        }

    private:
        static std::unique_ptr<T> s_instance; ///< Instance of the class (singleton)
        static std::once_flag s_init_flag;    ///< Flag ensuring the class gets instantiated only once
    };

    // Initialize static class members.

    template<typename T>
    std::unique_ptr<T> Singleton<T>::s_instance = nullptr;

    template<typename T>
    std::once_flag Singleton<T>::s_init_flag = {};
}

// EOF