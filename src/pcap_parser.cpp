#include "pcap_parser.h"
#include <pcap.h>
#include <stdexcept>

namespace trueconf
{

class PcapParser::Impl
{
public:
	Impl(std::string filename, std::string filter);
	~Impl();
	size_t countTcpPackets();

	void setFilename(const std::string &filename);
	void setFilter(const std::string &filter);
	std::string getFilename() const;
	std::string getFilter() const;

private:
	std::string m_filename;
	std::string m_filter;
	pcap_t *m_handle;

	void closeFile();
	void openFile(const std::string &filename);
};

PcapParser::Impl::Impl(std::string filename, std::string filter)
	: m_filename(std::move(filename)), m_filter(std::move(filter)), m_handle(nullptr)
{}

PcapParser::Impl::~Impl()
{ closeFile(); }

void PcapParser::Impl::closeFile()
{
	if (m_handle) {
		pcap_close(m_handle);
		m_handle = nullptr;
	}
}

void PcapParser::Impl::openFile(const std::string &filename)
{
	closeFile();
	m_filename = filename;

	char errbuf[PCAP_ERRBUF_SIZE];
	m_handle = pcap_open_offline(m_filename.c_str(), errbuf);

	if (!m_handle) {
		throw std::runtime_error("Failed to open pcap file: " + std::string(errbuf));
	}
}

size_t PcapParser::Impl::countTcpPackets()
{
	openFile(m_filename);

	struct bpf_program filter;

	if (pcap_compile(m_handle, &filter, m_filter.c_str(), 0, PCAP_NETMASK_UNKNOWN) == PCAP_ERROR) {
		throw std::runtime_error("Failed to compile filter: " + std::string(pcap_geterr(m_handle)));
	}

	if (pcap_setfilter(m_handle, &filter) == PCAP_ERROR) {
		throw std::runtime_error("Failed to set filter: " + std::string(pcap_geterr(m_handle)));
	}

	size_t packetCount = 0;

	if (pcap_loop(m_handle, 0,
				  [](u_char *user_data, const struct pcap_pkthdr */*pkthdr*/, const u_char */*packet*/)
				  {
					  auto *counter = reinterpret_cast<size_t *>(user_data);
					  (*counter)++;
				  },
				  reinterpret_cast<u_char *>(&packetCount)) == PCAP_ERROR) {
		throw std::runtime_error("Failed to process packets from file: " + std::string(pcap_geterr(m_handle)));
	}

	closeFile();

	return packetCount;
}

void PcapParser::Impl::setFilename(const std::string &filename)
{ m_filename = filename; }

void PcapParser::Impl::setFilter(const std::string &filter)
{ m_filter = filter; }

std::string PcapParser::Impl::getFilename() const
{ return m_filename; }

std::string PcapParser::Impl::getFilter() const
{ return m_filter; }

PcapParser::PcapParser(const std::string &filename, const std::string &filter = "tcp")
	: pimpl(std::make_unique<Impl>(filename, filter))
{}

PcapParser::~PcapParser() = default;

size_t PcapParser::countTcpPackets()
{ return pimpl->countTcpPackets(); }

void PcapParser::setFilename(const std::string &filename)
{ pimpl->setFilename(filename); }

void PcapParser::setFilter(const std::string &filter)
{ pimpl->setFilter(filter); }

std::string PcapParser::getFilename() const
{ return pimpl->getFilename(); }

std::string PcapParser::getFilter() const
{ return pimpl->getFilter(); }

} // trueconf
