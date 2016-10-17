#pragma once
#include "texture.h"
#include "renwin.h"
#include "primatives2d.h"

namespace txt
{
	// font face class
	class FontFace;
	namespace util // low level objects, not for use by end user
	{
		// RAII wrapper of FT_Library
		class FTLib
		{
		public:
			FTLib();
			~FTLib() { FT_Done_FreeType(library); };

			FT_Library library;
		};
		// RAII wrapper of FT_Face, generates font face from font file
		class FTFace
		{
		public:
			FTFace() {};
			FTFace(FTLib &ftLib,const std::string &fontName,unsigned short size) { LoadFace(ftLib,fontName,size); };
			void LoadFace(FTLib &ftLib,const std::string &fontName,unsigned short size);
			~FTFace() { FT_Done_Face(face); };

			FT_Face face;
		};

		// RAII wrapper of FT_Glyph and FT_BitmapGlyph, provides glyph metrics to user
		class FTGlyph
		{
		public:
			FTGlyph(FTFace &face,wchar_t c);
			~FTGlyph() { FT_Done_Glyph(glyph); };

			// width of glyph in pixels
			unsigned Width() const { return bitmap->bitmap.width; };
			// height of glyph in pixels
			unsigned Height() const { return bitmap->bitmap.rows; };
			// size of glyph bitmap data
			unsigned Size() const { return Width()*Height(); };
			// distance to advance the draw cursor on the x axis while rendering glyph
			unsigned XAdvance() const { return advanceX; };
			// X Offset of glyph in pixels
			unsigned XOff() const { return std::max<FT_Int>(bitmap->left,0); };
			// Y Offset of glyph in pixels
			unsigned YOff() const { return std::max<FT_Int>(bitmap->top,0); };
			// pointer to beginning of glyph bitmap buffer
			const byte *Begin() const { return (byte*)bitmap->bitmap.buffer; };
			// pointer to end of glyph bitmap buffer
			const byte *End() const { return Begin()+Size(); };

			FT_Glyph glyph;
			FT_BitmapGlyph bitmap;
			unsigned advanceX;// distance to advance the draw cursor on the x axis while rendering glyph
		};
		class MappedChar
		{
		public:
			// constructor for search operations
			MappedChar(wchar_t character_) : character(character_) {};
			MappedChar(wchar_t character_,math::vector2f txcdUprLeft_,math::vector2f txcdLwrRight_,unsigned width_,unsigned height_)
				: character(character_),txcdUprLeft(txcdUprLeft_),txcdLwrRight(txcdLwrRight_),
					width(width_),height(height_) {};
			MappedChar(const MappedChar &m)
				: character(m.character),txcdUprLeft(m.txcdUprLeft),txcdLwrRight(m.txcdLwrRight),
					width(m.width),height(m.height) {};
			MappedChar &operator=(const MappedChar &m)
				{ character=m.character;txcdUprLeft=m.txcdUprLeft;txcdLwrRight=m.txcdLwrRight;width=m.width;height=m.height;return *this; };

			bool operator==(const MappedChar &m) const { return character==m.character; };

			bool operator!=(const MappedChar &m) const { return character!=m.character; };

			bool operator<(const MappedChar &m) const { return character<m.character; };

			bool operator>(const MappedChar &m) const { return character>m.character; };

			// returns upper left texture coordinate
			const math::vector2f &TexCoordUpperLeft() const { return txcdUprLeft; };
			// returns lower right texture coordinate
			const math::vector2f &TexCoordLowerRight() const { return txcdLwrRight; };
			// returns the width of the char
			unsigned Width() const { return width; };
			// returns the height of the char
			unsigned Height() const { return height; };

			// returns upper right texture coordinate (by copy)
			math::vector2f TexCoordUpperRight() const { return math::vector2f(txcdLwrRight[math::X],txcdUprLeft[math::Y]); };
			// returns lower left texture coordinate (by copy)
			math::vector2f TexCoordLowerLeft() const { return math::vector2f(txcdUprLeft[math::X],txcdLwrRight[math::Y]); };
		private:
			wchar_t character; // character
			math::vector2f txcdUprLeft; // upper left texture coordinate
			math::vector2f txcdLwrRight; // lower left texture coordinate
			unsigned width,height; // width and height of character
		};
		class MappedFont
		{
		public:
			// generates a texture mapped font from the given font face and char list
			MappedFont(const txt::FontFace &fontFace,const std::wstring &charListUnsorted);
			// binds the font texture
			inline void Bind() const { texture.Bind(); };
			// returns the texture mapped char for the given character
			inline MappedChar &Get(wchar_t c)
			{
				typedef std::vector<MappedChar>::iterator it_t;
				std::pair<it_t,it_t> result=std::equal_range(charMap.begin(),charMap.end(),MappedChar(c));
				if (result.first!=result.second)
					return *result.first;
				return charMap.front();
			}
		private:
			tex::Texture texture; // font texture
			std::vector<MappedChar> charMap; // sorted vector of texture mapped chars
		};
		// helper constant for english char list
		const wchar_t DEFAULT_CHAR_LIST[]=
			L" ~!@#$%^&*()_+`1234567890-=abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ{}|:\"<>?[]\\;',./";
		// typdef for a colored text range
		typedef std::pair<unsigned,unsigned> range_t;
		// type for color paired to range of colored text
		typedef std::pair<range_t,math::vector3f> coloredTxt_t;
		// typdef for a colorQ
		typedef std::vector<coloredTxt_t> colorQ_t;
	};
	// dimensions of text data
	class TextMetrics
	{
	public:
		TextMetrics() : width(0),height(0),yOrigin(0),nRows(0),rowHeight(0),gPos(0) {};
		TextMetrics(unsigned maxWidth) : width(maxWidth),height(0),yOrigin(0),nRows(0),rowHeight(0),gPos(0) {};
		TextMetrics(const TextMetrics &t)
			: width(t.width),height(t.height),yOrigin(t.yOrigin),
			nRows(t.nRows),rowHeight(t.rowHeight),gPos(t.gPos) {};
		inline TextMetrics &operator=(const TextMetrics &t)
			{ width=t.width;height=t.height;yOrigin=t.yOrigin;
				nRows=t.nRows;rowHeight=t.rowHeight;gPos=t.gPos;
				return *this; }

		typedef std::pair<unsigned,unsigned> glyphPos_t; // xOffset, "row index" pair
		typedef std::vector<glyphPos_t> glyphPosVec_t; // vector of glyphPos_t
		// Calculates the text metrics from given font and string
		void Calculate(const FontFace &font,const std::wstring &s);

		unsigned width; // width of text block
		unsigned height; // height of text block
		unsigned yOrigin; // y Origin
		unsigned nRows; // number of rows
		unsigned rowHeight; // height of each row
		glyphPosVec_t gPos; // list of glyph positions (xOffset,row Index)
	};
	// Font face object, used to defin a new font face
	class FontFace
	{
	public:
		friend class util::FTGlyph;
		typedef boost::shared_ptr<util::MappedFont> mappedFontPtr_t;

		FontFace(const FontFace &f) : ftData(f.ftData),mappedFont(f.mappedFont),
			lineSpacing(f.lineSpacing),charSpacing(f.charSpacing) {};
		FontFace &operator=(const FontFace &f) { ftData=f.ftData;mappedFont=f.mappedFont;
			lineSpacing=f.lineSpacing;charSpacing=f.charSpacing;return *this; };
		FontFace(const std::string &fontName,unsigned short size,unsigned short lineSpacing_=1,unsigned short charSpacing_=1);
		// returns the glyph with the matching character
		util::FTGlyph GetGlyph(wchar_t c) const { return util::FTGlyph(ftData->second,c); };
		// returns pointer to the texture mapped font, creates one if it doesn't exist
		inline mappedFontPtr_t GetMappedFont()
		{
			if (mappedFont.get()==0)
				mappedFont.reset(new util::MappedFont(*this,std::wstring(util::DEFAULT_CHAR_LIST)) );
			return mappedFont;
		}
		// returns the distance between lines in pixels
		unsigned short LineSpacing() const { return lineSpacing; };
		// returns the distance between characters (horizontally) in pixels
		unsigned short CharSpacing() const { return charSpacing; };
	private:
		boost::shared_ptr<std::pair<util::FTLib,util::FTFace> > ftData; // ft face and library
		mappedFontPtr_t mappedFont; // pointer to mapped font, generated on first demand
		unsigned short lineSpacing, // distance between lines in pixels
			charSpacing; // distance between characters (horizontally) in pixels
	};

	class ColoredText;
	namespace util // low level objects, not for use by end user
	{
		void Append(ColoredText &d,const wchar_t *s,std::streamsize n);
		class TextSink
		{
		public:
			typedef wchar_t						char_type;
			typedef boost::iostreams::sink_tag	category;
			TextSink(ColoredText &d) : text(d) {};
			inline std::streamsize write(const char_type* s, std::streamsize n) { Append(text,s,n); return n; };
			ColoredText &text;
		};
	}
	// object for setting the color of dynamic text
	class Color
	{
	public:
		Color(math::vector3f &color_) : color(color_) {};
		Color(float red,float green,float blue) : color(red,green,blue) {};
		math::vector3f color; // color vector
	};
	// object for flushing the buffer of dynamic text
	class Endl {};

	class StaticText;
	// colored text object, may be used as an output stream
	class ColoredText
	{
	public:
		friend std::wostream& operator << (std::wostream& os, const ColoredText& d) { os<<d.Text();return os; };
		template <typename T>
		friend ColoredText& operator << (ColoredText& d, const T& s) { d.os<<s;return d; };

		ColoredText() { os.open(*this); };
		ColoredText(const std::wstring &text_) : text(text_) { os.open(*this); };
		ColoredText(const ColoredText &t) : text(t.text),colorQ(t.colorQ) { os.open(*this); };
		ColoredText &operator=(const ColoredText &t) { text=t.text;colorQ=t.colorQ;return *this; };
		// removes all data from Buffer
		inline void Clear() { text.clear();colorQ.clear(); };
		// sets the text color for all subsequent characters
		inline void SetColor(const math::vector3f &color)
			{ colorQ.push_back( util::coloredTxt_t(util::range_t(text.length(),text.length()),color) ); };
		// resets the text color for the entire text string
		inline void ResetColor(math::vector3f &color) { colorQ.clear();colorQ.back().second=color; };
		// appends a character to the text
		inline void Append(wchar_t c) { text.push_back(c);AppendColorRange(); };
		// appends a string to the text
		inline void Append(const std::wstring &s) { text.append(s);AppendColorRange(); };
		// appends a n characters from C string to the text
		inline void Append(const wchar_t *s,std::streamsize n) { text.append(s,n);AppendColorRange(); };
		// returns the length of text string
		inline unsigned Length() const { return text.length(); };
		// retrieves the current text
		const std::wstring &Text() const { return text; };
		// retrieves the color queue
		const util::colorQ_t &ColorQueue() const { return colorQ; };
	private:
		// adjust the range of the current color range to match the current text
		inline void AppendColorRange() { if (!colorQ.empty()) colorQ.back().first.second=text.length(); }
		// color queue lists pairs color changes with index into text string
		util::colorQ_t colorQ;
		std::wstring text; // text string

		boost::iostreams::stream<util::TextSink> os; // output stream, for recieving stream data
	};
	template <>
	inline ColoredText& operator << (ColoredText& d, const Color& s) { d.os.flush();d.SetColor(s.color);return d; };
	template <>
	inline ColoredText& operator << (ColoredText& d, const Endl& s) { d.os.flush();return d; };

	namespace util // low level objects, not for use by end user
	{
		inline void Append(ColoredText &d,const wchar_t *s,std::streamsize n) { d.Append(s,n); };
	}
	// Dynamic Text object, ostream compatible, should be used for text that changes frequently
	class DynamicText
	{
	public:
		// constructs DynamicText from a wstring
		explicit DynamicText(const FontFace &font,unsigned maxWidth,const std::wstring &text_,const math::vector2i &position)
			: fontFace(font), isMetricsValid(false),text(text_),
			metrics(maxWidth),renderPos(position) { mappedFont=fontFace.GetMappedFont(); };
		// constructs DynamicText from a ColoredText
		explicit DynamicText(const FontFace &font,unsigned maxWidth,ColoredText &text_,const math::vector2i &position)
			: fontFace(font), isMetricsValid(false),text(text_),
			metrics(maxWidth),renderPos(position) { mappedFont=fontFace.GetMappedFont(); };
		// constructs blank DynamicText from font and maxWidth
		DynamicText(const FontFace &font,unsigned maxWidth,const math::vector2i &position) : fontFace(font), isMetricsValid(false),
			metrics(maxWidth),renderPos(position) { mappedFont=fontFace.GetMappedFont(); };

		// removes all data from DynamicText
		inline void Clear() { isMetricsValid=false;text.Clear(); };
		// sets the text color for all subsequent characters
		inline void SetColor(const math::vector3f &color) { text.SetColor(color); };
		// resets the text color for the entire text string
		inline void ResetColor(math::vector3f &color) { text.ResetColor(color); };
		// appends a character to the text
		inline void Append(wchar_t c) { text.Append(c);isMetricsValid=false; };
		// appends a string to the text
		inline void Append(const std::wstring &s) { text.Append(s);isMetricsValid=false; };
		// appends a n characters from C string to the text
		inline void Append(const wchar_t *s,std::streamsize n) { text.Append(s,n);isMetricsValid=false; };
		// returns the length of text string
		inline unsigned Length() const { return text.Length(); };
		// returns the text metrics
		const TextMetrics &Metrics() { CalcMetrics();return metrics; };
		// sets the position of the top left corner of the text to be rendered
		inline void Position(math::vector2i &position) { renderPos=position; };
		// returns the position of the top left corner of the text to be rendered
		inline const math::vector2i &Position() const { return renderPos; };
		// renders the text at current position
		void Render();
		// retrieves the current text
		const std::wstring &Text() const { return text.Text(); };
		// retrieves the color queue
		const util::colorQ_t &ColorQueue() const { return text.ColorQueue(); };
		// retrieves a reference to the colored text buffer
		ColoredText &CText() { isMetricsValid=false;return text; };
		// retrieves a reference to the colored text buffer
		const ColoredText &CText() const { return text; };
		// retrieves the font face
		const FontFace &Font() const { return fontFace; };
	private:
		// calculates the metrics of the current text if the text has changed
		inline void CalcMetrics() { if (!isMetricsValid) metrics.Calculate(fontFace,text.Text());isMetricsValid=true; };

		math::vector2i renderPos; // render position

		FontFace fontFace; // font face (used for calculating text metrics)
		FontFace::mappedFontPtr_t mappedFont; // texture mapped font (used for rendering text)
		ColoredText text; // text buffer
		TextMetrics metrics; // dimensions of text
		bool isMetricsValid; // false if metrics need to be recalculated
	};
	// Static Text object, should be used for text that infrequently updated
	class StaticText
	{
	public:
		// constructs a static block of text with the specified font, maxWidth, and string
		StaticText(const FontFace &font,unsigned maxWidth,const std::wstring &textString,const math::vector2i &position)
			: fontFace(font),metrics(maxWidth),textGlyph(position) { Reset(textString,metrics.width); };
		// constructs a static block of text from a DynamicText object
		explicit StaticText(DynamicText &text)
			: fontFace(text.Font()),textGlyph(text.Position()) { Reset(text); };
		// constructs a static block of text from a ColoredText
		explicit StaticText(const FontFace &font,unsigned maxWidth,ColoredText &text,const math::vector2i &position)
			: fontFace(font),metrics(maxWidth),textGlyph(position) { Reset(text); };
		// resets the prerendered text to DynamicText text
		inline void Reset(DynamicText &text) { metrics=text.Metrics();fontFace=text.Font();Reset(text.CText().Text(),text.ColorQueue()); };
		// resets the prerendered text to ColoredText text
		inline void Reset(ColoredText &text) { Reset(text.Text(),text.ColorQueue()); };
		// resets the prerendered text to a new string and the current maximum width
		inline void Reset(const std::wstring &s) { Reset(s,util::colorQ_t()); };
		// resets the prerendered text to a new string and maximum width
		inline void Reset(const std::wstring &s,unsigned width) { metrics.width=width;Reset(s); };
		// sets the position of the top left corner of the text to be rendered
		inline void Position(math::vector2i &position) { textGlyph.Position(position); };
		// returns the position of the top left corner of the text to be rendered
		inline const math::vector2i &Position() const { return textGlyph.Position(); };
		// renders the text at current position
		inline void Render() const { textGlyph.Render(); };
		// binds the static text texture
		void Bind() { textGlyph.Texture().Bind(); };
		// returns the text metrics
		const TextMetrics &Metrics() const { return metrics; };
		// retrieves the font face
		const FontFace &Font() const { return fontFace; };
		// retrieves render glyph
		const prim::p2D::renderable::TexturedRect &TextGlyph() { return textGlyph; };
	private:
		// resets the prerendered text to a new string and color queue
		void Reset(const std::wstring &s,const util::colorQ_t &colorQ);

		FontFace fontFace; // font face
		TextMetrics metrics; // dimensions of text
		prim::p2D::renderable::TexturedRect textGlyph; // glyph of text to be rendered
	};
};
