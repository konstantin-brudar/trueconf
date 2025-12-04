#include "args.h"
#include <iostream>
#include <cstdlib>

namespace trueconf
{

void Args::show_help()
{
    std::cout << "Usage: ./tcpcount <filename> [--src-port <port>] [--dst-port <port>] [--help]\n"
              << "  <filename>  - path to PCAP file\n"
              << "  --src-port  - filter packets by source port (optional)\n"
              << "  --dst-port  - filter packets by destination port (optional)\n"
              << "  --help      - show this help (optional)\n"
              << "This is a command line tool to count the number of TCP packets in a PCAP file\n";
}

Args::Args(int argc, char *argv[])
{
    if (argc < 2) {
        show_help();
        std::exit(EXIT_FAILURE);
    }

    filename = argv[1];

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            show_help();
            std::exit(EXIT_SUCCESS);
        } else if (arg == "--src-port" && i + 1 < argc) {
            srcPort = std::stol(argv[++i]);
        } else if (arg == "--dst-port" && i + 1 < argc) {
            dstPort = std::stol(argv[++i]);
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            show_help();
            std::exit(EXIT_FAILURE);
        }
    }
}

} // trueconf
