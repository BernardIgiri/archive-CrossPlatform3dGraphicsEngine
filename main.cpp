#include "stdafx.h"
#include "timer.h"
#include "renwin.h"
#include "RenderModes.h"
#include "texture.h"
#include "pool.h"
#include "pngloader.h"
#include "text.h"
#include "vbo.h"

// loads textures from file
template <typename Stream,bool GEN_MIPMAPS>
class TextureLoad
{
public:
	// loads the texture from file in stream fs
	void operator()(tex::Texture &texture,Stream &fs)
	{
		image_loader::PNGLoad<Stream>(bmap,fs);
		texture.Upload(bmap,GEN_MIPMAPS);
	}
private:
	bmp::Bitmap bmap; // bitmap buffer
};

int main()
{
    lg::Log log("log.txt","Main()");

    bool	running = true;

    timing::Timer timer;
    float angle = 0.0f,seconds=0.0f;
    unsigned frame=0;

    math::vector2i screen(1024,768);
    renwin::Window window("GLFW Window",screen,false);
    glViewport( 0, 0, screen[math::X], screen[math::Y] );

    renwin::Window::resChangeLsnr_t func=window.OnResChange();
    //window.SetResolution(math::vector2i(800,600));
    if (func->IsChanged())
        screen=func->Resolution();

	renderer::modes::Mode nullMode;
	renderer::projMat_t projection;
	renderer::Projection(projection,screen,45.0f,0.2f,100.0f);
	renderer::modes::Render3D mode3D(nullMode,projection);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::ifstream ifs;
	typedef std::ifstream stream_t;
	typedef TextureLoad<stream_t,false> textureLoader_t;
	typedef pool::SourceM<tex::Texture,textureLoader_t,stream_t>
		textureSource_t;

	textureLoader_t textureLoader;
	textureSource_t textureSrc(textureLoader,ifs,std::ios::in|std::ios::binary);
	pool::SmartPool<tex::Texture,textureSource_t &>
		TexturePool(textureSrc);

	boost::shared_ptr<tex::Texture> t1,t2;

	TexturePool(t1,"tex7.png");
	TexturePool(t2,"tex5.png");

	txt::FontFace arial("Test.ttf",12,2,1);
	txt::StaticText test(arial,320,std::wstring(L"Hello. World! This is a demonstration of texture mapped text! Any questions?"),
		math::vector2i(10,screen[math::Y]-400));
	txt::DynamicText dTest(arial,150,math::vector2i(10,screen[math::Y]-550));

	txt::ColoredText buff;
	buff<<txt::Color(1.0f,0.0f,0.0f)<<L"This text has"<<txt::Color(1.0f,1.0f,1.0f)<<" mutliple "<<txt::Color(0.0f,0.0f,1.0f)
		<<"c"<<txt::Color(0.5f,0.5f,0.5f)<<"o"<<
		txt::Color(0.5f,0.5f,1.0f)<<"l"<<
		txt::Color(0.375f,0.625f,1.0f)<<"o"<<
		txt::Color(0.25f,0.75f,1.0f)<<"r"<<
		txt::Color(0.125f,0.875f,1.0f)<<"s"<<
		txt::Color(0.0f,1.0f,1.0f)<<"."<<
		txt::Endl();

	txt::StaticText test2(arial,150,buff,math::vector2i(10,screen[math::Y]-650));

	typedef math::vector3f vertex_t;
	typedef math::vector2f texcoord_t;
	const int vSz=4;
	vertex_t verticies[vSz];
	verticies[0]=math::vector3f( 1.0f, 0.0f,  0.0f);
	verticies[1]=math::vector3f(-1.0f, 0.0f,  0.0f);
	verticies[2]=math::vector3f(-1.0f, 2.0f,  0.0f);
	verticies[3]=math::vector3f( 1.0f, 2.0f,  0.0f);

	texcoord_t texCoords[vSz];
	texCoords[0]=math::vector2f( 1.0f, 1.0f);
	texCoords[1]=math::vector2f( 0.0f, 1.0f);
	texCoords[2]=math::vector2f( 0.0f, 0.0f);
	texCoords[3]=math::vector2f( 1.0f, 0.0f);

	//vbo::VertexVBO<vertex_t,3> vtxVBO(verticies,vSz,vbo::STATIC_DRAW,prim::TRIANGLE_FAN);
	//vbo::RenderVBO<vbo::render::Vertex3D> vtxVBO;
	//vtxVBO.Upload(verticies,vSz,vbo::STATIC_DRAW,prim::TRIANGLE_FAN);
	vbo::VBOPair<vbo::RenderVBO<vbo::render::Vertex3D>,vbo::RenderVBO<vbo::render::TexCoord> > vtxVBO(vSz,prim::TRIANGLE_FAN);
	vtxVBO.First().Upload(verticies,vSz,vbo::STATIC_DRAW);
	vtxVBO.Second().Upload(texCoords,vSz,vbo::STATIC_DRAW);

    while(running)
    {
        float elapsed=timer.ElapsedSeconds();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
        if (func->IsChanged())
            screen=func->Resolution();
        angle+=elapsed*180.0f;
        if (angle>60.0f)
            angle;
        if (angle>360.0f)
            angle=angle-360.0f;

        t1->Bind();
        glColor4f(1.0f,1.0f,1.0f,1.0f);
        glPushMatrix();
            glTranslatef(0.0f,0.0f,-18.0f);
            glPushMatrix();
                glRotatef(angle,0.0f,0.5f,0.5f);
                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                vtxVBO.Render();
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glDisableClientState(GL_VERTEX_ARRAY);
            glPopMatrix();
		glPopMatrix();
		{// 2D projection mode scope
			renderer::modes::Render2D mode2D(mode3D,screen);

            seconds+=elapsed;
            frame++;
            if (seconds>0.25f)
            {
                seconds=0.0f;
                dTest.Clear();
                dTest.CText()<<"FPS: "<<timing::CalcFPS(frame,elapsed)<<txt::Endl();
                frame=0;
            }
			test.Render();
			dTest.Render();
			glColor3f(1.0f,1.0f,1.0f);
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_LINES);
				glVertex2i(10,screen[math::Y]-650);
				glVertex2i(200,screen[math::Y]-650);
			glEnd();
			glEnable(GL_TEXTURE_2D);
			glColor3f(1.0f,1.0f,1.0f);
			test2.Render();
		}
		glFlush();
        window.SwapBuffers();

        // exit if ESC was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam( GLFW_OPENED);
    }

    return 0;
}
