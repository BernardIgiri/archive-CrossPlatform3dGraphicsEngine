#include "stdafx.h"
#include "gui.h"

using namespace gui;

void Window::CalcBounds()
{
	if (!isBoundsValid&&!childPanes.empty())
	{
		int maxX=childPanes[0].OuterBounds().MaxX(),
			maxY=childPanes[0].OuterBounds().MaxY(),
			minX=childPanes[0].OuterBounds().MinX(),
			minY=childPanes[0].OuterBounds().MinY();
		BOOST_FOREACH(const Pane &pane,childPanes)
		{
			minX=std::min(minX,pane.OuterBounds().MinX());
			maxX=std::max(maxX,pane.OuterBounds().MaxX());
			minY=std::min(minY,pane.OuterBounds().MinY());
			maxY=std::max(maxY,pane.OuterBounds().MaxY());
		}
		outerBB=prim::p2D::BBOX(maxX,maxY,minX,minY);
		isBoundsValid=true;
	}
}

Element& Pane::AddElement(const elmtPtr_t element)
{
	if (element.get()!=this)
	{
		element->SetParentPane(*this);
		childElements.push_back(element);
		return *childElements.back();
	}
	return *this;
}

Element::~Element() {};

void Pane::Position(const math::vector2i &position)
{
	//SetElementPosition(position,*this,mesh,outerBB,innerBB);
}
