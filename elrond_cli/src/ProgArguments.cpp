#include "ProgArguments.hpp"

using elrond::ProgArguments;
using argumentum::ParameterConfig;

void ProgArguments::add_parameters(ParameterConfig& params)
{
    params.add_parameter(this->configFile, "config-file" ).metavar("<CONFIG FILE>").help("Config JSON file").required(true);
    params.add_parameter(this->dumpConfig, "--dump-config" ).help("Show config data validated").default_value(false).required(false);
}

std::shared_ptr<ProgArguments> ProgArguments::parse(int argc, char* argv[])
{
    argumentum::argument_parser parser;
    auto params = parser.params();
    auto args = std::make_shared<ProgArguments>();

    params.add_parameters(args);

    if (!parser.parse_args(argc, argv, 1))
        throw std::invalid_argument("Invalid input arguments");

    return args;
}
