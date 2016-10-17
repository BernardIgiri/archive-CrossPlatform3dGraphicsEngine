#include "stdafx.h"
#include "text.h"
#include "filters.h"
#include "px.h"

namespace txt
{
    class Log : public lg::Log
    {
        public:
        Log() : lg::Log("log.txt","Text Object") {};
    };
	namespace util // low level objects, not for use by end user
	{
		// fliter object for blitting a glyph to a bitmap
		class BlitGlyph : public bmp::Filter
		{
		public:
			BlitGlyph(bmp::Bitmap &bmap,
					const math::vector3f &color_,
					const math::vector2i &offset_,
					const FTGlyph &glyph_)
				: color(color_), offset(offset_), glyph(glyph_)
				{ bmap.ApplyFilter(*this); };
			void operator()(bmp::RawData &raw);
		private:
			const math::vector3f &color; // color
			const math::vector2i &offset; // offset position
			const FTGlyph &glyph; // glyph
		};
		// renders text using the draw function at startPos, and returns the end position vector
		template <typename DrawFuncT>
		math::vector2i RenderText(const math::vector2i &startPos, // start render position
			const txt::FontFace &fontFace, // font face
			const std::wstring &s, // text to render
			const TextMetrics &metrics, // font metrics
			unsigned startIdx, // start index of text range to render
			unsigned endIdx, // end index of text range to render
			const colorQ_t &colorQ, // color queue
			DrawFuncT &drawFunc, // draw function
			bool inverseY) // true if y value should be inverted
		{
			math::vector2i offset(0,0); // current offset position
			unsigned glyphWidth=0,glyphHeight=0; // for catching dimensions of last glyph drawn
			math::vector3f colorDefault(1.0f,1.0f,1.0f); // default rendering color
			math::vector3f &color=colorDefault; // current rendering color
			colorQ_t::const_iterator colorIt=colorQ.begin(); // iterator into colorQ
			for (unsigned i=startIdx;i<endIdx;i++)
			{
				// get glyph
				FTGlyph glyph=fontFace.GetGlyph(s[i]);
				// if character has height
				if (glyph.YOff()>0)
				{
					// get render offset
					offset[math::X]=metrics.gPos[i].first+startPos[math::X];
					if (inverseY)
						offset[math::Y]=startPos[math::Y]-(metrics.gPos[i].second+1)*metrics.rowHeight+metrics.yOrigin-glyph.Height()+glyph.YOff();
					else
						offset[math::Y]=(metrics.gPos[i].second+1)*metrics.rowHeight-metrics.yOrigin-glyph.YOff()+startPos[math::Y];
					// select render color
					if (!colorQ.empty())
					{
						// advance to next color if index is past current range
						if (i>colorIt->first.second) ++colorIt;
						color=colorIt->second;
					}
					// render glyph
					drawFunc(i, // character index
							metrics, // metrics
							offset, // offset vector
							glyph, // glyph
							color); // render color
					// cache current glyph dimensions
					glyphWidth=glyph.Width();
					glyphHeight=glyph.Height();
				}
			}
			return math::vector2i(offset[math::X]+glyphWidth,offset[math::Y]+glyphHeight);
		}
		// draw function for blitting to bitmap
		class BlitDraw
		{
		public:
			BlitDraw(bmp::ColorTypePtr_T bmapColor,const TextMetrics &metrics)
			{
				bmp::filters::BlankBitmap(bmap,metrics.width,metrics.height,bmapColor);
			};
			inline void operator()(unsigned idx,
				const TextMetrics &metrics,
				const math::vector2i &offset,
				const FTGlyph &glyph,
				const math::vector3f &color)
			{
				BlitGlyph(bmap, color, offset, glyph);// bitmap,color,offset,glyph
			}
			// returns reference to bitmap
			bmp::Bitmap &Bitmap() { return bmap; };
		private:
			bmp::Bitmap bmap; // bitmap render target
		};
		// draw function for generating texture mapped fonts
		class MapFontDraw
		{
		public:
			typedef std::vector<MappedChar> charMap_t;

			MapFontDraw(charMap_t &charMap_,bmp::ColorTypePtr_T bmapColor,const TextMetrics &metrics,const std::wstring &charList_)
				: blitDrawer(bmapColor,metrics), charMap(charMap_),charList(charList_) {}
			inline void operator()(unsigned idx,
				const TextMetrics &metrics,
				const math::vector2i &offset,
				const FTGlyph &glyph,
				const math::vector3f &color)
			{
				charMap.push_back(MappedChar(charList[idx],
					 // upper left tex coord
					math::vector2f(static_cast<float>(offset[math::X])/blitDrawer.Bitmap().Width(),
						static_cast<float>(offset[math::Y])/blitDrawer.Bitmap().Height()),
					 // lower right text coord
					math::vector2f(static_cast<float>(offset[math::X]+glyph.Width())/blitDrawer.Bitmap().Width(),
						static_cast<float>(offset[math::Y]+glyph.Height())/blitDrawer.Bitmap().Height()),
					glyph.Width(),glyph.Height() )  );
				// delegate blitting to BlitDraw
				blitDrawer.operator()(idx,metrics,offset,glyph,color);
			}
			// returns reference to bitmap
			bmp::Bitmap &Bitmap() { return blitDrawer.Bitmap(); };
		private:
			const std::wstring &charList; // reference to rendered text
			BlitDraw blitDrawer; // used to draw text
			charMap_t &charMap; // reference to sorted vector of texture mapped chars
		};
		// draw function for rendering texture mapped fonts NOTE: AUTOMATICALLY Calls glBegin and glEnd
		class RenderMapText
		{
		public:
			RenderMapText(MappedFont &mappedFont_,const txt::FontFace &fontFace_,const TextMetrics &metrics_,const std::wstring &text_)
				: mappedFont(mappedFont_),fontFace(fontFace_),metrics(metrics_),text(text_)
			{
				mappedFont.Bind();
				glBegin(GL_QUADS);
			}
			~RenderMapText()
			{
				glEnd();
			}
			inline void operator()(unsigned idx,
				const TextMetrics &metrics,
				const math::vector2i &offset,
				const FTGlyph &glyph,
				const math::vector3f &color)
			{
				glColor3fv(color.data());
				MappedChar mc(mappedFont.Get(text[idx]));
				glTexCoord2fv(mc.TexCoordLowerLeft().data());glVertex2iv((GLint*)offset.data());
				glTexCoord2fv(mc.TexCoordLowerRight().data());glVertex2i(glyph.Width()+offset[math::X]	,(offset[math::Y]));
				glTexCoord2fv(mc.TexCoordUpperRight().data());glVertex2i(glyph.Width()+offset[math::X]	,(offset[math::Y]+glyph.Height()));
				glTexCoord2fv(mc.TexCoordUpperLeft().data());glVertex2i(offset[math::X]					,(offset[math::Y]+glyph.Height()));
			}
		private:
			const std::wstring &text; // reference to rendered text
			const TextMetrics &metrics; // reference to metrics
			const txt::FontFace &fontFace; // reference to font face
			MappedFont &mappedFont; // reference to mapped font
		};
	}
}

using namespace txt;

void util::BlitGlyph::operator()(bmp::RawData &raw)
{
	int i=offset[math::X]+offset[math::Y]*raw.Width();
	bmp::px::iterator begin=bmp::px::Begin(raw)+offset[math::X]+offset[math::Y]*raw.Width();
	bmp::px::iterator end=bmp::px::End(raw);
	const byte *pxIt=glyph.Begin(),*pxEnd=glyph.End();
	unsigned sEnd=glyph.Size(),xPos=glyph.Width(),deltaX=raw.Width()-glyph.Width();
	while ((begin!=end)&&(pxIt!=pxEnd))
	{
		if (xPos<1)
		{
			xPos=glyph.Width();
			begin+=deltaX;
		}
		else
		{
			begin->Red(color[math::R]*(*pxIt));
			begin->Green(color[math::G]*(*pxIt));
			begin->Blue(color[math::B]*(*pxIt));
			begin->Alpha(*pxIt);
			++pxIt;
			--xPos;
			++begin;
		}
	}
}

void util::FTFace::LoadFace(util::FTLib &ftLib,const std::string &fontName,unsigned short size)
{
    Log log;
	if (FT_New_Face( ftLib.library, fontName.c_str(), 0, &face ))
		throw ex::logic_error(log<<"FT_New_Face failed (there is probably a problem with your font file) font file:"<<fontName<<lg::endl());
	FT_Set_Char_Size( face, size*64, size*64, 96, 96);
}

FontFace::FontFace(const std::string &fontName,unsigned short size,unsigned short lineSpacing_,unsigned short charSpacing_)
{
	lineSpacing=lineSpacing_;
	charSpacing=charSpacing_;
	ftData.reset(new std::pair<util::FTLib,util::FTFace>());
	ftData->second.LoadFace(ftData->first,fontName,size);
}

util::FTLib::FTLib()
{
    Log log;
	if (FT_Init_FreeType( &library ))
		throw ex::logic_error(log<<"FT_Init_FreeType failed"<<lg::endl());
}

util::FTGlyph::FTGlyph(FTFace &face,wchar_t c)
{
    Log log;
	if(FT_Load_Glyph( face.face, FT_Get_Char_Index( face.face, c ), FT_LOAD_DEFAULT ))
		throw ex::logic_error(log<<"FT_Load_Glyph failed"<<lg::endl());

	//Move the face's glyph into a Glyph object.
    if(FT_Get_Glyph( face.face->glyph, &glyph ))
		throw ex::logic_error(log<<"FT_Get_Glyph failed"<<lg::endl());

	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	bitmap = (FT_BitmapGlyph)glyph;
	advanceX=std::max<FT_Int>(face.face->glyph->advance.x/64,bitmap->bitmap.width);
};

void StaticText::Reset(const std::wstring &s,const util::colorQ_t &colorQ)
{
	// calculate the size of the text block and then calculate height of each row
	metrics.Calculate(fontFace,s);
	// set color type to based on wether or not color data is present
	bmp::ColorTypePtr_T colorType;
	if (colorQ.empty()) // if no color data, then store text as brightness alpha type
		colorType.reset(new bmp::BA8());
	else // elsewise store as RGBA type
		colorType.reset(new bmp::RGBA8());
	// create BlitDraw object
	util::BlitDraw bdraw(colorType,metrics);
	// render to text to bitmap
	util::RenderText(math::vector2i(0,0),fontFace,s,metrics,0,s.length(),colorQ,bdraw,false);
	// upload bitmap to texture
	textGlyph.Texture().Upload(bdraw.Bitmap(),false);
	// set up glyph data
	textGlyph.Resize(prim::p2D::Rect2D(Position(),metrics.width,metrics.height));
}

void TextMetrics::Calculate(const FontFace &font,const std::wstring &s)
{
    Log log;
	// reset list of character positions
	gPos.clear();
	if (s.empty())
		throw ex::invalid_input(log<<"Cannot calculate text metrics for an empty string!"<<lg::endl());
	typedef std::wstring::const_iterator sIt_t;
	unsigned rowLen=0, maxHeight=0;
	nRows=1;
	yOrigin=0;
	for (sIt_t sIt=s.begin();sIt!=s.end();++sIt)
	{
		unsigned xOff=rowLen;
		util::FTGlyph glyph=font.GetGlyph(*sIt);
		unsigned glyphWidth=glyph.XAdvance()+font.CharSpacing();
		rowLen+=glyphWidth+glyph.XOff();
		if (glyph.YOff()<glyph.Height())
			yOrigin=std::max(yOrigin,glyph.Height()-glyph.YOff());
		maxHeight=std::max(glyph.Height(),maxHeight);
		if (rowLen>width) // wrap width
		{
			nRows++;
			xOff=0;
			rowLen=glyphWidth;
		}
		gPos.push_back(glyphPos_t(xOff,nRows-1));
	}
	rowHeight=(maxHeight+yOrigin)+font.LineSpacing();
	height=rowHeight*nRows;
}
void DynamicText::Render()
{
	if (text.Text().empty())
		return; // do not render blank text
	// make sure that the metrics have been calculated
	CalcMetrics();
	// create RenderMapText object
	util::RenderMapText renderer(*mappedFont,fontFace,metrics,text.Text());
	// render text
	util::RenderText(renderPos,fontFace,text.Text(),metrics,0,text.Text().length(),text.ColorQueue(),renderer,true);
}

util::MappedFont::MappedFont(const txt::FontFace &fontFace, const std::wstring &charListUnsorted)
{
	std::wstring charList(charListUnsorted);
	std::sort(charList.begin(),charList.end());
	TextMetrics metrics;
	metrics.width=64; // set width to an arbitrary value wide enough for the widest character
	metrics.Calculate(fontFace,charList);
	// set color type to brightness alpha
	bmp::ColorTypePtr_T colorType(new bmp::BA8());
	// create BlitDraw object
	util::MapFontDraw mapdraw(charMap,colorType,metrics,charList);
	// render to text to bitmap
	util::RenderText(math::vector2i(0,0),fontFace,charList,metrics,0,charList.length(),util::colorQ_t(),mapdraw,false);
	// upload to texture
	texture.Upload(mapdraw.Bitmap(),false);
}
