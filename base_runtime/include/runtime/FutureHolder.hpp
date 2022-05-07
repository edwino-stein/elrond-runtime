#ifndef ELROND_BASE_RUNTIME_RUNTIME_FUTURE_HOLDER_HPP
    #define ELROND_BASE_RUNTIME_RUNTIME_FUTURE_HOLDER_HPP

    #include "elrond_base_runtime_types.hpp"

    namespace elrond
    {
        namespace runtime
        {
            template <class T>
            class FutureHolder
            {
                private:
                    std::future<T> _future;

                public:

                    FutureHolder() = delete;
                    FutureHolder(const FutureHolder &other) = delete;
                    virtual ~FutureHolder() = default;

                    FutureHolder& operator= (const FutureHolder& other) = delete;

                    FutureHolder(std::future<T>&& future);

                    std::future_status status() const;
                    bool ready() const;
                    bool valid() const;
                    void wait() const;
                    T get();

                    static std::shared_ptr<FutureHolder<T>> make(std::future<T>&& future);
            };
        }
    }

#endif
