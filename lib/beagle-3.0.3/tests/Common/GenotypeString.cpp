#include "GenotypeString.hpp"

using namespace Beagle;
using namespace Test;
using namespace std;

void GenotypeString::setData(const string& inData)
{
  mData = inData;
}

void GenotypeString::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  ioStreamer.openTag("Genotype", inIndent);
  ioStreamer.insertAttribute("type","string");
  ioStreamer.insertStringContent(mData.c_str());
  ioStreamer.closeTag();
}
