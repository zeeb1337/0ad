#include "Matrix3D.h"
#include "Renderer.h"
#include "Terrain.h"
#include "LightEnv.h"
#include "TextureManager.h"
#include "Prometheus.h"

#include "time.h"
#include "sdl.h"
#include "res/tex.h"
#include "detect.h"

#include <malloc.h>

// TODO: fix scrolling hack - framerate independent, use SDL
//#include "win.h"	// REMOVEME

void InitScene ();
void InitResources ();
void RenderScene ();

extern bool keys[256];


CMatrix3D			g_WorldMat;
CRenderer			g_Renderer;
CTerrain			g_Terrain;
CCamera				g_Camera;
CLightEnv			g_LightEnv;

int					SelPX, SelPY, SelTX, SelTY;
int					g_BaseTexCounter = 0;
int					g_SecTexCounter = 1;
int					g_TransTexCounter = 0;

int					g_TickCounter = 0;
double				g_LastTime;


const int NUM_ALPHA_MAPS = 13;

int mouse_x=50, mouse_y=50;

void terr_init()
{
	int xres,yres;
	get_cur_resolution(xres,yres);
	g_Renderer.Open(xres,yres,32);

	SViewPort vp;
	vp.m_X=0;
	vp.m_Y=0;
	vp.m_Width=xres;
	vp.m_Height=yres;
	g_Camera.SetViewPort(&vp);

	InitResources ();
	InitScene ();
}

void terr_update()
{
	// start new frame
	g_Renderer.BeginFrame();
	g_Renderer.SetCamera(g_Camera);

	// switch on wireframe for terrain if we want it
	g_Renderer.SetTerrainRenderMode(SOLID);

	/////////////////////////////////////////////
		CVector3D right(1,0,1);
		CVector3D up(1,0,-1);
		right.Normalize ();
		up.Normalize ();
		
		if (mouse_x >= g_xres-2)
			g_Camera.m_Orientation.Translate (right);
		if (mouse_x <= 3)
			g_Camera.m_Orientation.Translate (right*-1);

		if (mouse_y >= g_yres-2)
			g_Camera.m_Orientation.Translate (up);
		if (mouse_y <= 3)
			g_Camera.m_Orientation.Translate (up*-1);



		float fov = g_Camera.GetFOV();
		float d = DEGTORAD(0.4f);
		if(keys[SDLK_KP_MINUS])
			if (fov+d < DEGTORAD(90))
				g_Camera.SetProjection (1, 1000, fov + d);
		if(keys[SDLK_KP_PLUS])
			if (fov-d > DEGTORAD(20))
			g_Camera.SetProjection (1, 1000, fov - d);

		g_Camera.UpdateFrustum ();
/////////////////////////////////////////////


	CFrustum frustum=g_Camera.GetFustum();

	// iterate through patches; cull everything not visible
	for (uint j=0; j<g_Terrain.GetPatchesPerSide(); j++)
	{
		for (uint i=0; i<g_Terrain.GetPatchesPerSide(); i++)
		{
			if (frustum.IsBoxVisible (CVector3D(0,0,0),g_Terrain.GetPatch(j, i)->GetBounds())) {
				g_Renderer.Submit(g_Terrain.GetPatch(j, i));
			}
		}
	}

	// flush the frame to force terrain to be renderered before overlays
	g_Renderer.FlushFrame();
	
		//	g_Renderer.RenderTileOutline (&(g_Terrain.m_Patches[SelPY][SelPX].m_MiniPatches[SelTY][SelTX]));

	g_Renderer.EndFrame();
}






bool terr_handler(const SDL_Event& ev)
{
	switch(ev.type)
	{
	case SDL_MOUSEMOTION:
		mouse_x = ev.motion.x;
		mouse_y = ev.motion.y;
		break;

	case SDL_KEYDOWN:
		switch(ev.key.keysym.sym)
		{
		case 'W':
			if (g_Renderer.GetTerrainRenderMode()==WIREFRAME) {
				g_Renderer.SetTerrainRenderMode(SOLID);
			} else {
				g_Renderer.SetTerrainRenderMode(WIREFRAME);
			}
			break;

		case 'H':
			// quick hack to return camera home, for screenshots (after alt+tabbing)
			g_Camera.SetProjection (1, 1000, DEGTORAD(20));
			g_Camera.m_Orientation.SetXRotation(DEGTORAD(30));
			g_Camera.m_Orientation.RotateY(DEGTORAD(-45));
			g_Camera.m_Orientation.Translate (100, 150, -100);
			break;

/*		case 'L':
			g_HillShading = !g_HillShading;
			break;*/

// tile selection
		case SDLK_DOWN:
			if(++SelTX > 15)
				if(SelPX == g_Terrain.GetPatchesPerSide()-1)
					SelTX = 15;
				else
					SelTX = 0, SelPX++;
			break;

		case SDLK_UP:
			if(--SelTX < 0)
				if(SelPX == 0)
					SelTX = 0;
				else
					SelTX = 15, SelPX--;
			break;
		case SDLK_RIGHT:
			if(++SelTY > 15)
				if(SelPY == g_Terrain.GetPatchesPerSide()-1)
					SelTY = 15;
				else
					SelTY = 0, SelPY++;
			break;

		case SDLK_LEFT:
			if(--SelTY < 0)
				if(SelPY == 0)
					SelTY = 0;
				else
					SelTY = 15, SelPY--;
			break;


		case SDLK_KP0:
				{
					CMiniPatch *MPatch = &g_Terrain.GetPatch(SelPY, SelPX)->m_MiniPatches[SelTY][SelTX];
					/*if (!MPatch->Tex2)
					{
						MPatch->m_AlphaMap = AlphaMaps[g_TransTexCounter];
						MPatch->Tex2 = BaseTexs[g_SecTexCounter];
					}
					else
					{
						MPatch->Tex2 = 0;
						MPatch->m_AlphaMap = 0;
					}*/
					break;
				}

		/*case SDLK_KP1:
				{
					CMiniPatch *MPatch = &g_Terrain.GetPatch(SelPY, SelPX)->m_MiniPatches[SelTY][SelTX];

					g_BaseTexCounter++;
					if (g_BaseTexCounter > 4)
						g_BaseTexCounter = 0;
					
					MPatch->Tex1 = BaseTexs[g_BaseTexCounter];
					break;
				}

		case SDLK_KP2:
				{
					CMiniPatch *MPatch = &g_Terrain.m_Patches[SelPY][SelPX].m_MiniPatches[SelTY][SelTX];
					
					if (MPatch->Tex2)
					{
						g_SecTexCounter++;
						if (g_SecTexCounter > 4)
							g_SecTexCounter = 0;

						MPatch->Tex2 = BaseTexs[g_SecTexCounter];
					}

					break;
				}
						
		case SDLK_KP3:
				{
					CMiniPatch *MPatch = &g_Terrain.m_Patches[SelPY][SelPX].m_MiniPatches[SelTY][SelTX];
					
					if (MPatch->m_AlphaMap)
					{
						g_TransTexCounter++;
						if (g_TransTexCounter >= NUM_ALPHA_MAPS)
							g_TransTexCounter = 0;

						MPatch->m_AlphaMap = AlphaMaps[g_TransTexCounter];
					}

					break;
				}*/

		}
	}

	return false;
}




void InitScene ()
{
	// setup default lighting environment
	g_LightEnv.m_SunColor=RGBColor(1,1,1);
	g_LightEnv.m_Rotation=DEGTORAD(270);
	g_LightEnv.m_Elevation=DEGTORAD(45);
	g_LightEnv.m_TerrainAmbientColor=RGBColor(0,0,0);
	g_LightEnv.m_UnitsAmbientColor=RGBColor(0.4f,0.4f,0.4f);
	g_Renderer.SetLightEnv(&g_LightEnv);

	// load terrain
	Handle ht = tex_load("terrain.raw");
	if(ht > 0)
	{
		const u8* p;
		int w;
		int h;

		tex_info(ht, &w, &h, NULL, NULL, (void **)&p);

		printf("terrain.raw: %dx%d\n", w, h);

		u16 *p16=new u16[w*h];
		u16 *p16p=p16;
		while (p16p < p16+(w*h))
			*p16p++ = (*p++) << 8;

		g_Terrain.Resize(20);
		g_Terrain.SetHeightMap(p16);

		delete[] p16;
		
		tex_free(ht);
	}

	// get default texture to apply to terrain
	CTextureEntry* texture=0;
	if (g_TexMan.m_TerrainTextures.size()>0) {
		if (g_TexMan.m_TerrainTextures[0].m_Textures.size()) {
			texture=g_TexMan.m_TerrainTextures[0].m_Textures[0];
		}
	}

	// cover entire terrain with default texture
	u32 patchesPerSide=g_Terrain.GetPatchesPerSide();
	for (uint pj=0; pj<patchesPerSide; pj++) {
		for (uint pi=0; pi<patchesPerSide; pi++) {
			
			CPatch* patch=g_Terrain.GetPatch(pi,pj);
			
			for (int j=0;j<16;j++) {
				for (int i=0;i<16;i++) {
					patch->m_MiniPatches[j][i].Tex1=texture ? texture->m_Handle :0;
				}
			}
		}
	}

	g_Camera.SetProjection (1, 1000, DEGTORAD(20));
	g_Camera.m_Orientation.SetXRotation(DEGTORAD(30));
	g_Camera.m_Orientation.RotateY(DEGTORAD(-45));

	g_Camera.m_Orientation.Translate (100, 150, -100);

	SelPX = SelPY = SelTX = SelTY = 0;
}

void InitResources()
{
#ifndef _WIN32
	g_TexMan.AddTextureType("grass");
	g_TexMan.AddTexture("Base1.tga", 0);
#else
	g_TexMan.LoadTerrainTextures();
#endif

	const char* fns[CRenderer::NumAlphaMaps] = {
		"art/textures/terrain/alphamaps/special/blendcircle.png",
		"art/textures/terrain/alphamaps/special/blendlshape.png",
		"art/textures/terrain/alphamaps/special/blendedge.png",
		"art/textures/terrain/alphamaps/special/blendedgecorner.png",
		"art/textures/terrain/alphamaps/special/blendedgetwocorners.png",
		"art/textures/terrain/alphamaps/special/blendfourcorners.png",
		"art/textures/terrain/alphamaps/special/blendtwooppositecorners.png",
		"art/textures/terrain/alphamaps/special/blendlshapecorner.png",
		"art/textures/terrain/alphamaps/special/blendtwocorners.png",
		"art/textures/terrain/alphamaps/special/blendcorner.png",
		"art/textures/terrain/alphamaps/special/blendtwoedges.png",
		"art/textures/terrain/alphamaps/special/blendthreecorners.png",
		"art/textures/terrain/alphamaps/special/blendushape.png",
		"art/textures/terrain/alphamaps/special/blendbad.png"
	};

	assert(g_Renderer.LoadAlphaMaps(fns));
}
