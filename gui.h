#pragma once
#include "primatives2d.h"

namespace gui
{
    typedef prim::color_t color_t;

	class Window;
	class Pane;

	class Element
	{
	public:
		virtual ~Element() = 0;
		// sets parent pane
		virtual void SetParentPane(Pane& parentPane) = 0;
		// returns reference to parent window element
		virtual const Window& WindowR() const = 0;
		// returns reference to parent window element
		virtual Window& WindowR() = 0;
		// returns reference to parent Pane
		virtual const Pane& PaneR() const = 0;
		// returns reference to parent Pane
		virtual Pane& PaneR() = 0;
		// returns element color
		virtual color_t const &Color() const = 0;
		// returns element color
		virtual color_t &Color() = 0;
		// returns border reference
		virtual color_t const &BorderColor() const = 0;
		// returns border reference
		virtual color_t &BorderColor() = 0;
		// sets the position of the upper left corner of the element
		virtual void Position(const math::vector2i &position) = 0;
		// gets the position of the upper left corner of the element
		virtual const math::vector2i &Position() const = 0;
		// gets the bounding box surrounding this element
		virtual const prim::p2D::BBOX &OuterBounds() const = 0;
		// gets the bounding box surrounding inner portion of this element
		virtual const prim::p2D::BBOX &InnerBounds() const = 0;
		// renders the element
		virtual void Render() const = 0;
	};
	typedef boost::shared_ptr<Element> elmtPtr_t; // element pointer

	class Window
	{
	public:
		Window() : isVisible(false),isBoundsValid(true) {};
		// returns sets window state to invisible
		inline void Close() { isVisible=false; };
		// returns sets window state to visible
		inline void Show() { isVisible=true; };
		// returns true if window is set to visible
		inline bool IsVisible() const { return isVisible; };
		// gets the bounding box surrounding this element
		inline const prim::p2D::BBOX &GetBounds() { CalcBounds();return outerBB; };
		// adds a pane to the list of child panes and returns a reference to it
		inline Pane &AddPane(const Pane& pane) { childPanes.push_back(pane);return childPanes.back();isBoundsValid=false; };
	private:
		// calculates the bounding box representing dimensions of this window
		void CalcBounds();

		prim::p2D::BBOX	outerBB; // bounding box representing dimensions of window
		std::vector<Pane> childPanes; // list of child panes
		bool isVisible; // this window visible if isVisible==true
		bool isBoundsValid; // true if innerBB still represents all child panes
	};
	class Pane : public Element
	{
	public:
		Pane(Window& window_,prim::p2D::Rect2D &rectangle,color_t backgroundColor,color_t &borderColor_,
			unsigned leftPad=0,unsigned rightPad=0,unsigned topPad=0,unsigned bottomPad=0)
			: window(window_), panePtr(0),isVisible(true),backGround(rectangle,backgroundColor),borderColor(borderColor_),
			// calculate inner bounding volumne
			innerBB(rectangle.MaxX()-rightPad,rectangle.MaxY()-topPad,rectangle.MinX()+leftPad,rectangle.MinY()+bottomPad)
		{ panePtr=this; };

		Pane(const Pane& p) : isVisible(p.isVisible),window(p.window),panePtr(p.panePtr),
			borderColor(p.borderColor),innerBB(p.innerBB),backGround(p.backGround),childElements(p.childElements) {};

		Pane& operator=(const Pane& p) { isVisible=p.isVisible;window=p.window;panePtr=p.panePtr;
			borderColor=p.borderColor;innerBB=p.innerBB;backGround=p.backGround;childElements=p.childElements; return *this; };

        ~Pane() {};
		// sets parent pane
		void SetParentPane(Pane& parentPane) { panePtr=&parentPane; };
		// returns reference to parent window element
		inline const Window& WindowR() const { return window; };
		// returns reference to parent window element
		Window& WindowR() { return window; };
		// returns reference to parent Pane
		const Pane& PaneR() const { return *panePtr; };
		// returns reference to parent Pane
		Pane& PaneR() { return *panePtr; };
		// returns element color
		color_t const &Color() const { return backGround.Color(); };
		// returns element color
		color_t &Color() { return backGround.Color(); };
		// returns border reference
		color_t const &BorderColor() const { return borderColor; };
		// returns border reference
		color_t &BorderColor() { return borderColor; };
		// sets the position of the upper left corner of the element
		void Position(const math::vector2i &position);
		// gets the position of the upper left corner of the element
		const math::vector2i &Position() const { return backGround.Position(); };
		// renders the element
		virtual void Render() const { backGround.Render(); };
		// gets the bounding box surrounding this element
		const prim::p2D::BBOX &OuterBounds() const { return backGround.OuterBounds(); };
		// gets the bounding box for the space inside this element
		inline const prim::p2D::BBOX &InnerBounds() const { return innerBB; };
		// returns sets pane state to invisible
		inline void Hide() { isVisible=false; };
		// returns sets pane state to visible
		inline void Show() { isVisible=true; };
		// returns true if pane is set to visible
		inline bool IsVisible() const { return isVisible; };
		// adds a element to the list of child elements and returns a reference to it
		Element &AddElement(const elmtPtr_t element);
	private:
		bool			isVisible; // this pane visible if isVisible==true
		Window&			window; // parent window reference
		Pane*			panePtr; // pointer to parent pane
		color_t			borderColor; // border color around pane
		prim::p2D::renderable::ColoredRect backGround; // background rectangle
		prim::p2D::BBOX	innerBB; // bounding box representing inner dimensions of pane
		std::vector<elmtPtr_t> childElements; // list of child elements
	};
};
