//
// Parsing command line arguments
//

#ifndef TRUECONF_ARGS_H
#define TRUECONF_ARGS_H

#include <string>
#include <optional>

namespace trueconf
{

struct Args
{
    std::string filename;
    std::optional<long> srcPort;
    std::optional<long> dstPort;

    Args(int argc, char *argv[]);
    static void show_help();
};

} // trueconf

#endif // TRUECONF_ARGS_H
