#ifndef ELROND_BASE_RUNTIME_RUNTIME_FUTURE_HOLDER_IPP
    #define ELROND_BASE_RUNTIME_RUNTIME_FUTURE_HOLDER_IPP

    #include "runtime/FutureHolder.hpp"

    namespace elrond
    {
        namespace runtime
        {

            template <class T>
            FutureHolder<T>::FutureHolder(std::future<T>&& future)
            : _future(std::move(future)) {}

            template <class T>
            std::future_status FutureHolder<T>::status() const
            {
                return this->_future.wait_for(std::chrono::milliseconds(0));
            }

            template <class T>
            bool FutureHolder<T>::ready() const
            {
                return this->status() != std::future_status::timeout;
            }

            template <class T>
            bool FutureHolder<T>::valid() const
            {
                return this->_future.value();
            }
            
            template <class T>
            void FutureHolder<T>::wait() const
            {
                this->_future.wait();
            }

            template <class T>
            T FutureHolder<T>::get()
            {
                return this->_future.get();
            }

            template <class T>
            std::shared_ptr<FutureHolder<T>> FutureHolder<T>::make(std::future<T>&& future)
            {
                return std::make_shared<FutureHolder<T>>(std::move(future));
            }
        }
    }

#endif
