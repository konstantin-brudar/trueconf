//
// Command line tool to count TCP packets in PCAP file
//

#include "args.h"
#include "pcap_parser.h"
#include <iostream>
#include <string>

using trueconf::Args;
using trueconf::PcapParser;

int main(int argc, char *argv[])
{
    Args args(argc, argv);

    std::string filter = "tcp";
    filter += args.srcPort ? " and src port " + std::to_string(*args.srcPort) : "";
    filter += args.dstPort ? " and dst port " + std::to_string(*args.dstPort) : "";

    size_t packets = 0;

    try {
        PcapParser parser(args.filename, filter);
        packets = parser.countTcpPackets();
    } catch (const std::runtime_error &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << packets << std::endl;

    return EXIT_SUCCESS;
}
