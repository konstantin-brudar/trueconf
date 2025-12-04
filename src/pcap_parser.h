//
// Class for parsing PCAP files
//

#ifndef TRUECONF_PCAP_PARSER_H
#define TRUECONF_PCAP_PARSER_H

#include <string>
#include <memory>

namespace trueconf
{

class PcapParser
{
public:
    PcapParser(const std::string &filename, const std::string &filter);
    ~PcapParser();

    size_t countTcpPackets();

    void setFilename(const std::string &filename);
    void setFilter(const std::string &filter);
    std::string getFilename() const;
    std::string getFilter() const;

private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
};

} // trueconf

#endif // TRUECONF_PCAP_PARSER_H
