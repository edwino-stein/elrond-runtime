#ifndef ELROND_SIGNAL_STANDALONE_HPP
    #define ELROND_SIGNAL_STANDALONE_HPP

    #include <functional>
    #include <map>
    #include <csignal>

    namespace elrond
    {
        class Signal {

            public:
                enum SIGID : int {
                    ABRT = SIGABRT,
                    FPE = SIGFPE,
                    ILL = SIGILL,
                    INT = SIGINT,
                    SEGV = SIGSEGV,
                    TERM = SIGTERM
                };

                using SignalCallbackH = std::function<void(enum SIGID)>;

            private:
                static std::map<enum SIGID, SignalCallbackH> callbacks;

            public:
                Signal () = delete;
                ~Signal () = delete;
                Signal(const Signal&) =delete;
                Signal(Signal&&) =delete;
                Signal& operator=(const Signal&) =delete;
                Signal& operator=(Signal&&) =delete;

                static void attach(enum SIGID sig, SignalCallbackH handle);
                static void detach(enum SIGID sig);
                static void trigger(enum SIGID sig);
                static bool hasAttached(enum SIGID sig);

            protected:
                static void entryPoint(int signum);

        };
    }

#endif
