#if !defined  _ELROND_SIGNAL_STANDALONE_HPP
    #define _ELROND_SIGNAL_STANDALONE_HPP

    #include <functional>
    #include <map>
    #include <csignal>

    enum class SIG : int {
        ABRT = SIGABRT,
        FPE = SIGFPE,
        ILL = SIGILL,
        INT = SIGINT,
        SEGV = SIGSEGV,
        TERM = SIGTERM
    };

    class Signal {

        private:
            Signal();

        public:

            using signalCallbackT = std::function<void()>;

            static void attach(enum SIG sig, Signal::signalCallbackT task);
            static void detach(enum SIG sig);
            static void trigger(enum SIG sig);
            static bool hasAttached(enum SIG sig);

        protected:
            static std::map<enum SIG, Signal::signalCallbackT> callbacks;
            static void entryPoint(int signum);

    };

#endif
