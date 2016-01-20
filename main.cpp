
#include "sfwdraw.h"
#include "Plotter.h"
#include <cmath>
//quadBezier(3 vectors)
//cubicBezier(4 vectors)
//hermite(4 vectors)
//quaddratic Bezier: (1-t)^2*a + t*(1-t)*B + t^2*c

float lerp(float a, float b, float t)
{
	return (b - a) * t + a;
}

class Lerp : public Plotter
{
protected:
	void step(float t)
	{
		x = (xe - xs)*t + xs;// lerp(xs, xe, t);
		y = (ye - ys)*t + ys;// lerp(ys, ye, t);
	}
public:
	float xs, ys;
	float xe, ye;

	Lerp(float x, float y, float x1, float y1) : Plotter(),
		xs(x), ys(y), xe(x1), ye(y1)
	{ }
};

class QuadraticBezier : public Plotter
{
protected:
	void step(float t)
	{
		x = (1 - t) * (1 - t) * xs + 2 * (1 - t) * t * xm + t * t * xe;
		y = (1 - t) * (1 - t) * ys + 2 * (1 - t) * t * ym + t * t * ye;		
	}
public:
	float xs, ys;
	float xe, ye;
	float xm, ym;

	QuadraticBezier(float x, float y, float x1, float y1, float x2, float y2) 
		: Plotter() ,xs(x), ys(y), xe(x2), ye(y2), xm(x1), ym(x1)
	{ }
};

class Quad : public Plotter
{
protected:
	void step(float t) 
	{
		x = t;
		y = t*t;

	}

public:
	Quad()
	{
		rangeStart = -1;
	}
};

void main()
{
	sfw::initContext(400,400,"NSFW Draw");

	float range = 1;
	float step = .1f;
	float wing = step / 5;

	float r = range,
		  t = range,
		  l = -r,
		  b = -t,
		  f = 1,
		  n = -1;

	float ortho[16] = {2/(r-l),       0,        0, -(r+l)/(r-l),
					         0, 2/(t-b),        0, -(t+b)/(t-b),
					         0,       0, -2/(f-n), -(f+n)/(f-n), 
							 0,       0,        0,            1};

	sfw::setProjectionMatrix(ortho);
	
	
	/////////////////////
	Lerp temp(.2,0,.4,1);
	Quad quad;
	QuadraticBezier qbTest(.7f, .2f, .7f, .3f, .1f, .9f);


	while (sfw::stepContext())
	{	

		//////////////////////////////////////////////////////
		//	Draw Grid
		#pragma region Grid
		for (float x = l; x <= r; x += step)
		{
			sfw::drawLine(x, b, x, t, 0x88888844);
			sfw::drawLine(x,-wing,x,wing,0x00000088);
		}

		for (float y = b; y <= t; y += step)
		{
			sfw::drawLine(l, y, r, y, 0x88888844);
			sfw::drawLine(-wing, y, wing, y, 0x00000088);
		}
		sfw::drawLine(0, t, 0, b, 0x000000cc);
		sfw::drawLine(l, 0, r, 0, 0x000000cc);
		#pragma endregion
		/////////////////////////////////////////////////////

		temp.draw();
		quad.draw();
		qbTest.draw();
	}

	sfw::termContext();
}