#pragma once
#include "renwin.h"
#include "primatives2d.h"

namespace renderer
{
	typedef math::matrix44f_c projMat_t;
	// sets projection matrix based on screen resolution, near, far, and feild of view
	inline void Projection(projMat_t &projection,math::vector2i &displayXY,float feildOfView,float nearClip,float farClip)
	{
		math::matrix_perspective_xfov_RH(projection,
			feildOfView,
			(float)displayXY[math::X]/(float)displayXY[math::Y],
			nearClip,
			farClip,
			math::z_clip_neg_one);
	};
	namespace modes // rendering modes
	{
		// all modes call outerMode.Restore() on close to restore the outer rendering mode

		// render mode base class, use this as starting mode
		class Mode
		{
		public:
			virtual ~Mode() {};
			virtual void Restore() {};
		};
		// initiates 2D render mode for screen dimensions screenRes_
		class Render2D : public Mode
		{
		public:
			Render2D(Mode &outerMode_,const math::vector2i &screenRes_)
				: outerMode(outerMode_),screenRes(screenRes_) { Restore(); };
			~Render2D(void) { outerMode.Restore(); };
			// restores render mode after gl state has been modified
			void Restore();
		private:
			const math::vector2i &screenRes; // screen resolution
			Mode &outerMode; // reference to outer mode
		};
		// initiates 3D render for projection matrix projection_
		class Render3D : public Mode
		{
		public:
			Render3D(Mode &outerMode_,const projMat_t &projection_)
				: outerMode(outerMode_),projection(projection_) { Restore(); };
			~Render3D(void) { outerMode.Restore(); };
			// restores render mode after gl state has been modified
			void Restore();
		private:
			const projMat_t &projection; // projection
			Mode &outerMode; // reference to outer mode
		};
	}
}
