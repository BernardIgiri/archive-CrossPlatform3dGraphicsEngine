#include "stdafx.h"
#include "fileio.h"

using namespace io;

bool MatchExt::operator()(const std::string &fname) const
{
	if (fname.length()>extension.length())
		return (fname.substr(fname.length()-extension.length(),extension.length())==extension);
	return false;
}
