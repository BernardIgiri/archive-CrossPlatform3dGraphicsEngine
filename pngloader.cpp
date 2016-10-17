#include "stdafx.h"
#include "pngloader.h"

void image_loader::util::SetRows(std::vector<png_bytep> &rows,bmp::RawData &raw)
{
	rows.resize(raw.Height());
	RowCounter counter(raw.pixels, raw.Width()*raw.BytesPerPixel());
	std::generate(rows.begin(),rows.end(),counter);
}
