#include "stdafx.h"
#include "primatives2d.h"


using namespace prim;

void RectVerts(math::vector2f verts[],const p2D::Rect2D &rect)
{
    p2D::Rect2D rectCpy=rect;
    rectCpy.Translate(-rectCpy.UpperLeft());
    verts[0]=math::vector2f(rectCpy.MaxX(),rectCpy.MinY());
    verts[1]=math::vector2f(rectCpy.MinX(),rectCpy.MinY());
    verts[2]=math::vector2f(rectCpy.MinX(),rectCpy.MaxY());
    verts[3]=math::vector2f(rectCpy.MaxX(),rectCpy.MaxY());
}

void p2D::renderable::RectVBO(p2D::renderable::ColoredRectVBO_t &vtxVBO,const p2D::Rect2D &rect)
{
    math::vector2f verts[nRectVerts];
    RectVerts(verts,rect);
    vtxVBO.Upload(verts,nRectVerts,vbo::STATIC_DRAW);
};

void p2D::renderable::RectVBO(p2D::renderable::TexturedRectVBO_t &vtxVBO,const p2D::Rect2D &rect)
{
    math::vector2f verts[nRectVerts];
    RectVerts(verts,rect);

    math::vector2f texCoords[nRectVerts];
    texCoords[0]=math::vector2f( 1.0f, 1.0f);
	texCoords[1]=math::vector2f( 0.0f, 1.0f);
	texCoords[2]=math::vector2f( 0.0f, 0.0f);
	texCoords[3]=math::vector2f( 1.0f, 0.0f);

    vtxVBO.First().Upload(verts,nRectVerts,vbo::STATIC_DRAW);
    vtxVBO.Second().Upload(texCoords,nRectVerts,vbo::STATIC_DRAW);
};

void p2D::renderable::ColoredRect::Render(const p2D::renderable::RenderMode mode) const
{
    glPushMatrix();
                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4fv(color.data());
    glTranslatef(bb.UpperLeft()[math::X],bb.UpperLeft()[math::Y],0.0f);
    vtxVBO.Render(p2D::renderable::nRectVerts,static_cast<prim::DrawMode_t>(mode));
                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
}

void p2D::renderable::TexturedRect::Render(const p2D::renderable::RenderMode mode) const
{
    glPushMatrix();
                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    texture.Bind();
    glColor4fv(color.data());
    glTranslatef(bb.UpperLeft()[math::X],bb.UpperLeft()[math::Y],0.0f);
    vtxVBO.Render(p2D::renderable::nRectVerts,static_cast<prim::DrawMode_t>(mode));
                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
}
