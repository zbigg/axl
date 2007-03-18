/*
* File: 	gd.c
*
* Id:		$Id: gd.c 1.3 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl library
*
* Description:	AXL gd module
*
* Author Zbigniew Zagorski <zzagorski@wi.ps.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2002,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include "axl/axl.h"
#include <string.h>
#include <gd.h>

XLF_DECL(xl_gd_init);

ZMOD_IMPLEMENT_MODULE_BEGIN
    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
	ZMOD_ENTRY_A(gd_init,xl_gd_init)
    ZMOD_ENTRIES_END

    ZMOD_INFO(
	gd,
	"axlGD",
	1,0,
	"AXL gd interface"
    )
ZMOD_IMPLEMENT_MODULE_END

#define AF xlo_add_field_func_builtin
#define AI xlo_add_field_int
#define AS xlo_add_field_str

XLF_DECL(gd_ImageCreate);
XLF_DECL(gd_ImageCreateTrueColor);
XLF_DECL(gd_ImageCreateFromJpeg);
XLF_DECL(gd_ImageCreateFromPng);

XLF_DECL(gd_Image_SaveJpeg);
XLF_DECL(gd_Image_SavePng);

XLF_DECL(gd_create_image);
XLF_DECL(gd_load);

XLF_DECL(img_putpixel);
XLF_DECL(img_getpixel);
XLF_DECL(img_line);
XLF_DECL(img_vline);
XLF_DECL(img_triangle);
XLF_DECL(img_hline);
XLF_DECL(img_rect);
XLF_DECL(img_rectfill);
XLF_DECL(img_circle);
XLF_DECL(img_circlefill);
XLF_DECL(img_ellipse);
XLF_DECL(img_ellipsefill);
XLF_DECL(img_arc);
XLF_DECL(img_floodfill);
XLF_DECL(img_save);

int	axl_io_ZSTREAM_type = -1;
XLF_DECL(xl_gd_init)
{
    xl_object obj = xlo_new_map();

    AF(obj,"ImageCreate",gd_ImageCreate,"gd.ImageCreate");
    AF(obj,"ImageCreateTrueColor",gd_ImageCreateTrueColor,"gd.ImageCreateTrueColor");
    AF(obj,"ImageCreateFromJpeg",gd_ImageCreateFromJpeg,"gd.ImageCreateFromJpeg");
    AF(obj,"ImageCreateFromPng",gd_ImageCreateFromPng,"gd.ImageCreateFromPng");

    AF(obj,"create_image",gd_create_image,"gd.create_image");
    AF(obj,"load",gd_load,"gd.load");

    strcpy(obj->name,".gd");

    axl_io_ZSTREAM_type = xlv_ptr_type("axliostream");

    XLF_RETURN_OBJ(obj);
}

static void gdImageDestroy_wrapper(void* p)
{
    if( p ) gdImageDestroy((gdImagePtr)p);
}
static int gd_get_color_depth(gdImagePtr gdimg)
{
#ifdef GD_TRUECOLOR
    return gdImageTrueColor(gdimg) ? 32 : 8;
#else
    return 8;
#endif
}
static void gd_add_Image_methods(xl_object img)
{
    gdImagePtr x = xlo_get_res(img);
    AF(img,"SavePng",gd_Image_SavePng,"gd.Image.SavePng");
    AF(img,"SaveJpeg",gd_Image_SaveJpeg,"gd.Image.SaveJpeg");
    AI(img,"width",x->sx);
    AI(img,"height",x->sy);
    AI(img,"bpp",gd_get_color_depth(x));
    AF(img,"putpixel"	,img_putpixel,	"gd.image.putpixel");
    AF(img,"getpixel"	,img_getpixel,	"gd.image.getpixel");
    AF(img,"line"	,img_line,	"gd.image.line");
    AF(img,"hline"	,img_hline,	"gd.image.hline");
    AF(img,"vline"	,img_vline,	"gd.image.vline");
    AF(img,"triangle"	,img_triangle,	"gd.image.triangle");
    AF(img,"rect"	,img_rect,	"gd.image.rect");
    AF(img,"rectfill"	,img_rectfill,	"gd.image.rectfill");
    AF(img,"circle"	,img_circle,	"gd.image.circle");
    AF(img,"circlefill"	,img_circlefill,"gd.image.circlefill");
    AF(img,"ellipse"	,img_ellipse,	"gd.image.ellipse");
    AF(img,"ellipsefill",img_ellipsefill,"gd.image.ellipsefill");
    AF(img,"arc"	,img_arc,	"gd.image.arc");
    AF(img,"floodfill"	,img_floodfill,	"gd.image.floodfill");
    AF(img,"save"	,img_save,	"gd.image.save");
}
static xl_object xlo_new_gdImage(gdImagePtr gdimg)
{
    xl_object img = xlo_new_res(gdimg,NULL,0, gdImageDestroy_wrapper);

#ifdef GD_TRUECOLOR
    sprintf(img->name,"gd.Image(%ix%ix%i)",
	gdimg->sx,gdimg->sy,gd_get_color_depth(gdimg));
#else
    sprintf(img->name,"gd.Image(%ix%ix8)",gdimg->sx,gdimg->sy);
#endif
    gd_add_Image_methods(img);

    return img;
}

typedef struct _ZSTREAM_IOCtx {
    gdIOCtx ctx;
    ZSTREAM s;
} ZSTREAM_IOCtx;

static void	ZSTREAM_IOCtx_init(ZSTREAM_IOCtx* c,ZSTREAM s);

XLF_DECL(gd_create_image)
{
    int		width	= XLF_ARG_INT(0,-1);
    int		height	= XLF_ARG_INT(1,-1);
    int		bpp	= XLF_ARG_INT(2,8);
    gdImagePtr  gdimg;
    if( width <= 0 || height <= 0 )
	XLF_RETURN_NIL();

    if( bpp > 0 && bpp <= 8 ) {
	gdimg = gdImageCreate(width,height);
    } else
	if( bpp >= 15 && bpp <= 32 ) {
#ifdef GD_TRUECOLOR
	    gdimg = gdImageCreateTrueColor(width,height);
#else
	    gdimg = NULL;
#endif
	} else
	    gdimg = NULL;
    if( ! gdimg ) XLF_RETURN_NIL();

    XLF_RETURN_OBJ( xlo_new_gdImage(gdimg) );
}

XLF_DECL(gd_load)
{
    const char*	filename= XLF_ARG_STR(0,NULL);
    gdImagePtr  gdimg = NULL;
    ZSTREAM_IOCtx ctx;
    ZSTREAM file;
    if( !filename ||
	(file = zopen(filename,ZO_READ | ZO_BINARY)) == NULL )
	    XLF_RETURN_NIL();

    ZSTREAM_IOCtx_init(&ctx,file);
    {
	if( strstr(filename,".jpg") ||
	    strstr(filename,".jpeg") ) {
	    gdimg = gdImageCreateFromJpegCtx((gdIOCtx*)&ctx);
	} else
	if( strstr(filename,".png") ) {
	    gdimg = gdImageCreateFromPngCtx((gdIOCtx*)&ctx);
	}
    }
    zclose(file);
    if( ! gdimg ) XLF_RETURN_NIL();

    XLF_RETURN_OBJ(xlo_new_gdImage (gdimg));
}

XLF_DECL(gd_ImageCreate)
{
    int		width	= XLF_ARG_INT(0,0);
    int		height	= XLF_ARG_INT(1,0);
    gdImagePtr  gdimg;
    if( width <= 0 || height <= 0 )
	XLF_RETURN_NIL();
    gdimg = gdImageCreate(width,height);
    if( !gdimg ) XLF_RETURN_NIL();

    XLF_RETURN_OBJ( xlo_new_gdImage (gdimg) );
}

XLF_DECL(gd_ImageCreateTrueColor)
{
    int		width	= XLF_ARG_INT(0,0);
    int		height	= XLF_ARG_INT(1,0);
    gdImagePtr  gdimg;
    if( width < 0 || height < 0 )
	XLF_RETURN_NIL();
#ifdef GD_TRUECOLOR
    gdimg = gdImageCreateTrueColor(width,height);
#else
    gdimg = NULL;
#endif
    if( !gdimg ) XLF_RETURN_NIL();

    XLF_RETURN_OBJ( xlo_new_gdImage (gdimg) );
}

XLF_DECL(gd_ImageCreateFromJpeg)
{
    const char*	filename= XLF_ARG_STR(0,NULL);
    ZSTREAM 	xfile	= XLF_ARG_PTR(0,axl_io_ZSTREAM_type,NULL);
    gdImagePtr  gdimg;
    ZSTREAM_IOCtx ctx;
    ZSTREAM file;
    if( xfile )
	file = xfile;
    else if( filename == NULL ||
	(file = zopen(filename,ZO_READ | ZO_BINARY)) == NULL)
	XLF_RETURN_NIL();
    ZSTREAM_IOCtx_init(&ctx,file);
    gdimg = gdImageCreateFromJpegCtx((gdIOCtx*)&ctx);
    if( !xfile ) zclose(file);
    if( !gdimg ) {
	XLF_RETURN_NIL();
    }

    XLF_RETURN_OBJ(xlo_new_gdImage (gdimg));
}

XLF_DECL(gd_ImageCreateFromPng)
{
    const char*	filename= XLF_ARG_STR(0,NULL);
    ZSTREAM 	xfile	= XLF_ARG_PTR(0,axl_io_ZSTREAM_type,NULL);
    gdImagePtr  gdimg;
    ZSTREAM_IOCtx ctx;
    ZSTREAM file;
    if( xfile )
	file = xfile;
    else if( filename == NULL ||
	(file = zopen(filename,ZO_READ | ZO_BINARY)) == NULL)
	XLF_RETURN_NIL();
    ZSTREAM_IOCtx_init(&ctx,file);
    gdimg = gdImageCreateFromPngCtx((gdIOCtx*)&ctx);
    if( !xfile ) zclose(file);
    if( !gdimg ) {
	XLF_RETURN_NIL();
    }
    XLF_RETURN_OBJ(xlo_new_gdImage (gdimg));
}

XLF_DECL(gd_Image_SavePng)
{
    xl_object athis	= (xl_object)XLF_THIS;
    const char*	filename= XLF_ARG_STR(0,NULL);
    ZSTREAM 	xfile	= XLF_ARG_PTR(0,axl_io_ZSTREAM_type,NULL);
    gdImagePtr  gdimg	= (gdImagePtr)xlo_get_res(athis);
    ZSTREAM_IOCtx ctx;
    ZSTREAM file;
    if( xfile )
	file = xfile;
    else if( filename == NULL ||
	(file = zopen(filename,ZO_NEW | ZO_TRUNC | ZO_BINARY)) == NULL)
	XLF_RETURN_NIL();
    ZSTREAM_IOCtx_init(&ctx,file);
    gdImagePngCtx(gdimg,(gdIOCtx*)&ctx);
    if( !xfile ) zclose(file);
    XLF_RETURN_REF((xl_value)athis);
}

XLF_DECL(gd_Image_SaveJpeg)
{
    xl_object athis	= (xl_object)XLF_THIS;
    const char*	filename= XLF_ARG_STR(0,NULL);
    ZSTREAM 	xfile	= XLF_ARG_PTR(0,axl_io_ZSTREAM_type,NULL);
    gdImagePtr  gdimg	= (gdImagePtr)xlo_get_res(athis);
    int		quality = XLF_ARG_INT(1,-1);
    ZSTREAM_IOCtx ctx;
    ZSTREAM file;
    if( xfile )
	file = xfile;
    else if( filename == NULL ||
	(file = zopen(filename,ZO_NEW | ZO_TRUNC | ZO_BINARY)) == NULL)
	XLF_RETURN_NIL();
    ZSTREAM_IOCtx_init(&ctx,file);
    gdImageJpegCtx(gdimg,(gdIOCtx*)&ctx,quality);
    if( !xfile ) zclose(file);
    XLF_RETURN_REF((xl_value)athis);
}

static int _zpio_Getbuf (gdIOCtx *c, void *b, int l)
{
    ZSTREAM_IOCtx* x = (ZSTREAM_IOCtx*)c;
    return zread(x->s,b,l);
}
static int _zpio_Putbuf (gdIOCtx * c, const void * b, int l)
{
    ZSTREAM_IOCtx* x = (ZSTREAM_IOCtx*)c;
    return zwrite(x->s,b,l);
}
static void _zpio_Putchar (gdIOCtx *c, int ch)
{
    ZSTREAM_IOCtx* x = (ZSTREAM_IOCtx*)c;
    zfputc(x->s,ch);
}
static int _zpio_Getchar (gdIOCtx * c)
{
    ZSTREAM_IOCtx* x = (ZSTREAM_IOCtx*)c;
    return zfgetc(x->s);
}

static int _zpio_Seek (struct gdIOCtx *c, const int ofs)
{
    ZSTREAM_IOCtx* x = (ZSTREAM_IOCtx*)c;
    return ( zseek(x->s,  ofs, ZSEEK_SET) == 0 );
}
static long _zpio_Tell (struct gdIOCtx *c)
{
    ZSTREAM_IOCtx* x = (ZSTREAM_IOCtx*)c;
    return zseek(x->s,0,ZSEEK_CUR);
}
static void gdFreeZSTREAMCtx (gdIOCtx * ctx)
{
}

static void	ZSTREAM_IOCtx_init(ZSTREAM_IOCtx* c,ZSTREAM s)
{
#ifdef GD_TRUECOLOR
    c->ctx.gd_free = gdFreeZSTREAMCtx;
#else
    c->ctx.free = gdFreeZSTREAMCtx;
#endif
    c->ctx.getC = _zpio_Getchar;
    c->ctx.getBuf = _zpio_Getbuf;
    c->ctx.putC = _zpio_Putchar;
    c->ctx.putBuf = _zpio_Putbuf;
    c->ctx.seek = _zpio_Seek;
    c->ctx.tell = _zpio_Tell;
    c->s = s;
}
int	def_color = 1;
XLF_DECL(img_putpixel)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x		= XLF_ARG_INT(0,0);
    int	y		= XLF_ARG_INT(1,0);
    int c		= XLF_ARG_INT(2,def_color);
    gdImageSetPixel(img,x,y,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_getpixel)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x		= XLF_ARG_INT(0,0);
    int	y		= XLF_ARG_INT(1,0);
    XLF_RETURN_INT(gdImageGetPixel(img,x,y));
}
XLF_DECL(img_line)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x1	= XLF_ARG_INT(0,0);
    int	y1	= XLF_ARG_INT(1,0);
    int	x2	= XLF_ARG_INT(2,1);
    int	y2	= XLF_ARG_INT(3,1);
    int	c	= XLF_ARG_INT(4,def_color);
    gdImageLine(img,x1,y1,x2,y2,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_vline)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x		= XLF_ARG_INT(0,0);
    int	y1		= XLF_ARG_INT(1,0);
    int y2		= XLF_ARG_INT(2,0);
    int	c		= XLF_ARG_INT(3,def_color);
    gdImageLine(img,x,y1,x,y2,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_hline)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x1		= XLF_ARG_INT(0,0);
    int	y		= XLF_ARG_INT(1,0);
    int x2		= XLF_ARG_INT(2,0);
    int	c		= XLF_ARG_INT(3,def_color);
    gdImageLine(img,x1,y,x2,y,c);
    XLF_RETURN_REF((xl_value)this);
}

XLF_DECL(img_triangle)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x1	= XLF_ARG_INT(0,0);
    int	y1	= XLF_ARG_INT(1,0);
    int	x2	= XLF_ARG_INT(2,1);
    int	y2	= XLF_ARG_INT(3,1);
    int	x3	= XLF_ARG_INT(4,1);
    int	y3	= XLF_ARG_INT(5,1);
    int	c	= XLF_ARG_INT(6,def_color);
    gdImageLine(img,x1,y1,x2,y2,c);
    gdImageLine(img,x2,y2,x3,y3,c);
    gdImageLine(img,x3,y3,x1,y1,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_rect)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x1	= XLF_ARG_INT(0,0);
    int	y1	= XLF_ARG_INT(1,0);
    int	x2	= XLF_ARG_INT(2,1);
    int	y2	= XLF_ARG_INT(3,1);
    int	c	= XLF_ARG_INT(4,def_color);
    gdImageRectangle(img,x1,y1,x2,y2,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_rectfill)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x1	= XLF_ARG_INT(0,0);
    int	y1	= XLF_ARG_INT(1,0);
    int	x2	= XLF_ARG_INT(2,1);
    int	y2	= XLF_ARG_INT(3,1);
    int	c	= XLF_ARG_INT(4,def_color);
    gdImageFilledRectangle(img,x1,y1,x2,y2,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_circle)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x	= XLF_ARG_INT(0,0);
    int	y	= XLF_ARG_INT(1,0);
    int	r	= XLF_ARG_INT(2,0);
    int	c	= XLF_ARG_INT(3,def_color);
    gdImageArc(img,x,y,r*2,r*2,0,360,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_circlefill)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x	= XLF_ARG_INT(0,0);
    int	y	= XLF_ARG_INT(1,0);
    int	r	= XLF_ARG_INT(2,0);
    int	c	= XLF_ARG_INT(3,def_color);
    gdImageArc(img,x,y,r*2,r*2,0,360,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_ellipse)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x	= XLF_ARG_INT(0,0);
    int	y	= XLF_ARG_INT(1,0);
    int	rx	= XLF_ARG_INT(2,1);
    int	ry	= XLF_ARG_INT(3,1);
    int	c	= XLF_ARG_INT(4,def_color);
    gdImageArc(img,x,y,rx*2,ry*2,0,360,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_ellipsefill)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x	= XLF_ARG_INT(0,0);
    int	y	= XLF_ARG_INT(1,0);
    int	rx	= XLF_ARG_INT(2,1);
    int	ry	= XLF_ARG_INT(3,1);
    int	c	= XLF_ARG_INT(4,def_color);
    gdImageArc(img,x,y,rx*2,ry*2,0,360,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_arc)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x	= XLF_ARG_INT(0,0);
    int	y	= XLF_ARG_INT(1,0);
    double a1	= XLF_ARG_FLOAT(2,1);
    double a2	= XLF_ARG_FLOAT(3,1);
    int	r	= XLF_ARG_INT(4,0);
    int	c	= XLF_ARG_INT(5,def_color);
    gdImageArc(img,x,y,r*2,r*2,a1 < a2 ? a1 : a2,a2 > a1 ? a2 : a1,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_floodfill)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr img	= xlo_get_res(this);
    int	x		= XLF_ARG_INT(0,0);
    int	y		= XLF_ARG_INT(1,0);
    int c		= XLF_ARG_INT(2,def_color);
    gdImageFill(img,x,y,c);
    XLF_RETURN_REF((xl_value)this);
}
XLF_DECL(img_save)
{
    xl_object this	= (xl_object)XLF_THIS;
    gdImagePtr gdimg	= xlo_get_res(this);
    const char* filename	= XLF_ARG_STR(0,NULL);
    int		q	= XLF_ARG_INT(1,-1);
    int result = 0;
    ZSTREAM_IOCtx ctx;
    ZSTREAM file;
    if( !filename || !gdimg )
	XLF_RETURN_NIL();
    file = zopen(filename,ZO_NEW | ZO_TRUNC | ZO_BINARY);
    if( !file )
	XLF_RETURN_NIL();

    ZSTREAM_IOCtx_init(&ctx,file);
    {
	if( strstr(filename,".jpg") ||
	    strstr(filename,".jpeg") ) {
	    gdImageJpegCtx(gdimg,(gdIOCtx*)&ctx,q);
	    result = 1;
	} else
	if( strstr(filename,".png") ) {
	    gdImagePngCtx(gdimg,(gdIOCtx*)&ctx);
	    result = 1;
	}
    }
    zclose(file);
    XLF_RETURN_INT( result );
}
