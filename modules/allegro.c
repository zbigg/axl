/*
* File: 	allegro.c
*
* Id:		$Id: allegro.c 1.2 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl library
*
* Description:	allegro module
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2001,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.

FIXME: rewrite to be more elegant


*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include "axl/axl.h"

#include <allegro.h>
#include <stdlib.h>
#include <string.h>

XLF_DECL(xl_allegro_init);

XLF_DECL(allegro_create_bitmap);
XLF_DECL(allegro_makecol);

ZMOD_IMPLEMENT_MODULE_BEGIN
    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
	ZMOD_ENTRY_A(create_allegro_object,xl_allegro_init)
    ZMOD_ENTRIES_END
    ZMOD_INFO(
	allegro,
	"axlAllegro",
	1,0,
	"Allegro Game Programming Library module for axl"
    )
ZMOD_IMPLEMENT_MODULE_END

#define AF xlo_add_field_func_builtin
#define AI xlo_add_field_int

static int allegro_initialized  = 0;


XLF_DECL(xl_allegro_init)
{
    xl_object aobj;
    if( allegro_initialized == 0 ) {
	/* TODO: allegro initialization always fails because it's not 
	         implemented yet */
	if( install_allegro(SYSTEM_NONE,&errno,atexit) != 0 )
	    XLF_RETURN_NIL();
	allegro_initialized = 1;
	set_color_depth(24);
	set_color_conversion(COLORCONV_TOTAL);
    }
    aobj = xlo_new_map();
    if( !aobj ) XLF_RETURN_FATAL();
    
    strcpy(aobj->name,"allegro");
    AF(aobj,"create_bitmap",allegro_create_bitmap,"allegro.create_bitmap");
    AF(aobj,"makecol",	allegro_makecol,"allegro.makecol");
    XLF_RETURN_OBJ(aobj);
}

#if defined main || defined(unix)
int _mangled_main(int argc,char** argv)
{
    int main(int argc,char** argv);
    return main(argc,argv);
}
void *_mangled_main_address = (void*) _mangled_main;
#endif

static void	destroy_bitmap_wrapper(void* p)
{
    if( p ) destroy_bitmap((BITMAP*)p);
}
static int def_color_depth = 24;
static int def_color = 15;
/* allegro.bitmap members: drawing primitives */
XLF_DECL(bmp_putpixel);
XLF_DECL(bmp_getpixel);
XLF_DECL(bmp_line);
XLF_DECL(bmp_vline);
XLF_DECL(bmp_triangle);
XLF_DECL(bmp_hline);
XLF_DECL(bmp_rect);
XLF_DECL(bmp_rectfill);
XLF_DECL(bmp_circle);
XLF_DECL(bmp_circlefill);
XLF_DECL(bmp_ellipse);
XLF_DECL(bmp_ellipsefill);
XLF_DECL(bmp_arc);
XLF_DECL(bmp_floodfill);
XLF_DECL(bmp_save);

PALETTE pal;

XLF_DECL(allegro_create_bitmap)
{
    xl_object   allegro = (xl_object)XLF_THIS;
    int		width	= XLF_ARG_INT(0,-1);
    int		height	= XLF_ARG_INT(1,-1);
    int		bpp	= XLF_ARG_INT(2,def_color_depth);
    xl_object bmp;
    BITMAP* abmp;
    if( width < 0 || height < 0 )
	XLF_RETURN_NIL();
    set_color_depth(bpp);
    abmp = create_bitmap(width,height);
    
    if( !abmp ) {
	xlo_add_field_str(allegro,"error",allegro_error);
	XLF_RETURN_NIL();
    }
    bmp = xlo_new_res(abmp,NULL,0, destroy_bitmap_wrapper);
    if( !bmp ) XLF_RETURN_FATAL();
    
    sprintf(bmp->name,"al_bmp(%ix%ix%i)",width,height,bpp);
    AI(bmp,"bpp"	,bpp);
    AI(bmp,"width"	,width);
    AI(bmp,"height"	,height);
    
    AF(bmp,"putpixel"	,bmp_putpixel,	"allegro.bitmap.putpixel");
    AF(bmp,"getpixel"	,bmp_getpixel,	"allegro.bitmap.getpixel");
    AF(bmp,"line"	,bmp_line,	"allegro.bitmap.line");
    AF(bmp,"hline"	,bmp_hline,	"allegro.bitmap.hline");
    AF(bmp,"vline"	,bmp_vline,	"allegro.bitmap.vline");
    AF(bmp,"triangle"	,bmp_triangle,	"allegro.bitmap.triangle");
    AF(bmp,"rect"	,bmp_rect,	"allegro.bitmap.rect");
    AF(bmp,"rectfill"	,bmp_rectfill,	"allegro.bitmap.rectfill");
    AF(bmp,"circle"	,bmp_circle,	"allegro.bitmap.circle");
    AF(bmp,"circlefill"	,bmp_circlefill,"allegro.bitmap.circlefill");
    AF(bmp,"ellipse"	,bmp_ellipse,	"allegro.bitmap.ellipse");
    AF(bmp,"ellipsefill",bmp_ellipsefill,"allegro.bitmap.ellipsefill");
    AF(bmp,"arc"	,bmp_arc,	"allegro.bitmap.arc");
    AF(bmp,"floodfill"	,bmp_floodfill,	"allegro.bitmap.floodfill");
    AF(bmp,"save"	,bmp_save,	"allegro.bitmap.save");
/*    AF(bmp,""	,bmp_,	"allego.bitmap.");*/
    
    XLF_RETURN_OBJ(bmp);
}

XLF_DECL(allegro_makecol)
{
    int	r		= XLF_ARG_INT(0,0);
    int	g		= XLF_ARG_INT(1,0);
    int b		= XLF_ARG_INT(2,0);
    XLF_RETURN_INT(makecol24(r,g,b));
}

XLF_DECL(bmp_putpixel)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x		= XLF_ARG_INT(0,0);
    int	y		= XLF_ARG_INT(1,0);
    int c		= XLF_ARG_INT(2,def_color);
    putpixel(bmp,x,y,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_getpixel)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x		= XLF_ARG_INT(0,0);
    int	y		= XLF_ARG_INT(1,0);
    XLF_RETURN_INT(getpixel(bmp,x,y));
}
XLF_DECL(bmp_line)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x1	= XLF_ARG_INT(0,0);
    int	y1	= XLF_ARG_INT(1,0);
    int	x2	= XLF_ARG_INT(2,1);
    int	y2	= XLF_ARG_INT(3,1);
    int	c	= XLF_ARG_INT(4,def_color);
    line(bmp,x1,y1,x2,y2,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_vline)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x		= XLF_ARG_INT(0,0);
    int	y1		= XLF_ARG_INT(1,0);
    int y2		= XLF_ARG_INT(2,0);
    int	c		= XLF_ARG_INT(3,def_color);
    vline(bmp,x,y1,y2,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_hline)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x1		= XLF_ARG_INT(0,0);
    int	y		= XLF_ARG_INT(1,0);
    int x2		= XLF_ARG_INT(2,0);
    int	c		= XLF_ARG_INT(3,def_color);
    hline(bmp,x1,y,x2,c);
    XLF_RETURN_REF((xl_value)this);
}

XLF_DECL(bmp_triangle)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x1	= XLF_ARG_INT(0,0);
    int	y1	= XLF_ARG_INT(1,0);
    int	x2	= XLF_ARG_INT(2,1);
    int	y2	= XLF_ARG_INT(3,1);
    int	x3	= XLF_ARG_INT(4,1);
    int	y3	= XLF_ARG_INT(5,1);
    int	c	= XLF_ARG_INT(6,def_color);
    triangle(bmp,x1,y1,x2,y2,x3,y3,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_rect)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x1	= XLF_ARG_INT(0,0);
    int	y1	= XLF_ARG_INT(1,0);
    int	x2	= XLF_ARG_INT(2,1);
    int	y2	= XLF_ARG_INT(3,1);
    int	c	= XLF_ARG_INT(4,def_color);
    rect(bmp,x1,y1,x2,y2,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_rectfill)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x1	= XLF_ARG_INT(0,0);
    int	y1	= XLF_ARG_INT(1,0);
    int	x2	= XLF_ARG_INT(2,1);
    int	y2	= XLF_ARG_INT(3,1);
    int	c	= XLF_ARG_INT(4,def_color);
    rectfill(bmp,x1,y1,x2,y2,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_circle)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x	= XLF_ARG_INT(0,0);
    int	y	= XLF_ARG_INT(1,0);
    int	r	= XLF_ARG_INT(2,0);
    int	c	= XLF_ARG_INT(3,def_color);
    circle(bmp,x,y,r,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_circlefill)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x	= XLF_ARG_INT(0,0);
    int	y	= XLF_ARG_INT(1,0);
    int	r	= XLF_ARG_INT(2,0);
    int	c	= XLF_ARG_INT(3,def_color);
    circlefill(bmp,x,y,r,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_ellipse)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x	= XLF_ARG_INT(0,0);
    int	y	= XLF_ARG_INT(1,0);
    int	rx	= XLF_ARG_INT(2,1);
    int	ry	= XLF_ARG_INT(3,1);
    int	c	= XLF_ARG_INT(4,def_color);
    ellipse(bmp,x,y,rx,ry,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_ellipsefill)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x	= XLF_ARG_INT(0,0);
    int	y	= XLF_ARG_INT(1,0);
    int	rx	= XLF_ARG_INT(2,1);
    int	ry	= XLF_ARG_INT(3,1);
    int	c	= XLF_ARG_INT(4,def_color);
    ellipsefill(bmp,x,y,rx,ry,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_arc)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x	= XLF_ARG_INT(0,0);
    int	y	= XLF_ARG_INT(1,0);
    double a1	= XLF_ARG_FLOAT(2,0)*256.0/360.0;
    double a2	= XLF_ARG_FLOAT(3,260)*256.0/360.0;
    int	r	= XLF_ARG_INT(4,0);
    int	c	= XLF_ARG_INT(5,def_color);
    fixed a1f = ftofix(a1);
    fixed a2f = ftofix(a2);
    arc(bmp,x,y,a1f,a2f,r,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_floodfill)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    int	x		= XLF_ARG_INT(0,0);
    int	y		= XLF_ARG_INT(1,0);
    int c		= XLF_ARG_INT(2,def_color);
    floodfill(bmp,x,y,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(bmp_save)
{
    xl_object this	= (xl_object)XLF_THIS;
    BITMAP* bmp		= xlo_get_res(this);
    const char* fname	= XLF_ARG_STR(0,NULL);
    if( !fname || !bmp ) XLF_RETURN_NIL();
    
    XLF_RETURN_INT( save_bitmap(fname,bmp,pal) == 0);
}
