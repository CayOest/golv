#pragma once

namespace skatomat {
    template <class T>
    class Singleton {
    public:
        static T& instance();
    protected:
        Singleton() {}
    };

    template <class T>
    T& Singleton<T>::instance() {
        static T instance;
        return instance;
    }


}
