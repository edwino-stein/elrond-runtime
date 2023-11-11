#ifndef ELROND_CLI_PROG_ARGUMENTS_HPP
    #define ELROND_CLI_PROG_ARGUMENTS_HPP

    #include <elrond_runtime.hpp>
    #include <argumentum/argparse.h>

    namespace elrond
    {
        class ProgArguments : public argumentum::Options
        {
            public:
                std::string configFile;
                bool dumpConfig;

            protected:
                void add_parameters(argumentum::ParameterConfig&) override;

            public:
                ELROND_DEFAULT_CLASS_SPECIAL_MEMBERS(ProgArguments)
                static std::shared_ptr<ProgArguments> parse(int argc, char* argv[]);
        };
    }

#endif
