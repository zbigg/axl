/*
* File: 	wxaxl/wxaxl.cpp
*
* Id:		$Id: wxaxl.cpp 1.1 04/02/07 22:24:32+01:00 nikt@nowy.dom $
*
* Project:	wxAXL
*
* Description:	wxAXL - AXL to wxWindows binding 
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2004,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#   include "wx/wx.h"
#endif

#include "wx/app.h"

#ifdef HAVE_CONFIG
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include "axl/axl.h"


extern "C" XLF_EDECL( wxAxl_main )
{
   // wxString a;
    return 0;
}
extern "C" XLF_EDECL( wxAxl_Factory )
{
    return 0;
}

