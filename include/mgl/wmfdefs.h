/* $Id: wmfdefs.h,v 1.1.1.1 2002/12/31 01:26:22 pavlovskii Exp $ */

WMF_FUNCTION(SETBKCOLOR, 0x0201)
WMF_FUNCTION(SETBKMODE, 0x0102)
WMF_FUNCTION(SETMAPMODE, 0x0103)
WMF_FUNCTION(SETROP2, 0x0104)
WMF_FUNCTION(SETRELABS, 0x0105)
WMF_FUNCTION(SETPOLYFILLMODE, 0x0106)
WMF_FUNCTION(SETSTRETCHBLTMODE, 0x0107)
WMF_FUNCTION(SETTEXTCHAREXTRA, 0x0108)
WMF_FUNCTION(SETTEXTCOLOR, 0x0209)
WMF_FUNCTION(SETTEXTJUSTIFICATION, 0x020A)
WMF_FUNCTION(SETWINDOWORG, 0x020B)
WMF_FUNCTION(SETWINDOWEXT, 0x020C)
WMF_FUNCTION(SETVIEWPORTORG, 0x020D)
WMF_FUNCTION(SETVIEWPORTEXT, 0x020E)
WMF_FUNCTION(OFFSETWINDOWORG, 0x020F)
WMF_FUNCTION(SCALEWINDOWEXT, 0x0410)
WMF_FUNCTION(OFFSETVIEWPORTORG, 0x0211)
WMF_FUNCTION(SCALEVIEWPORTEXT, 0x0412)
WMF_FUNCTION(LINETO, 0x0213)
WMF_FUNCTION(MOVETO, 0x0214)
WMF_FUNCTION(EXCLUDECLIPRECT, 0x0415)
WMF_FUNCTION(INTERSECTCLIPRECT, 0x0416)
WMF_FUNCTION(ARC, 0x0817)
WMF_FUNCTION(ELLIPSE, 0x0418)
WMF_FUNCTION(FLOODFILL, 0x0419)
WMF_FUNCTION(PIE, 0x081A)
WMF_FUNCTION(RECTANGLE, 0x041B)
WMF_FUNCTION(ROUNDRECT, 0x061C)
WMF_FUNCTION(PATBLT, 0x061D)
WMF_FUNCTION(SAVEDC, 0x001E)
WMF_FUNCTION(SETPIXEL, 0x041F)
WMF_FUNCTION(OFFSETCLIPRGN, 0x0220)
WMF_FUNCTION(TEXTOUT, 0x0521)
WMF_FUNCTION(BITBLT, 0x0922)
WMF_FUNCTION(STRETCHBLT, 0x0B23)
WMF_FUNCTION(POLYGON, 0x0324)
WMF_FUNCTION(POLYLINE, 0x0325)
WMF_FUNCTION(ESCAPE, 0x0626)
WMF_FUNCTION(RESTOREDC, 0x0127)
WMF_FUNCTION(FILLREGION, 0x0228)
WMF_FUNCTION(FRAMEREGION, 0x0429)
WMF_FUNCTION(INVERTREGION, 0x012A)
WMF_FUNCTION(PAINTREGION, 0x012B)
WMF_FUNCTION(SELECTCLIPREGION, 0x012C)
WMF_FUNCTION(SELECTOBJECT, 0x012D)
WMF_FUNCTION(SETTEXTALIGN, 0x012E)
WMF_FUNCTION(CHORD, 0x0830)
WMF_FUNCTION(SETMAPPERFLAGS, 0x0231)
WMF_FUNCTION(EXTTEXTOUT, 0x0a32)
WMF_FUNCTION(SETDIBTODEV, 0x0d33)
WMF_FUNCTION(SELECTPALETTE, 0x0234)
WMF_FUNCTION(REALIZEPALETTE, 0x0035)
WMF_FUNCTION(ANIMATEPALETTE, 0x0436)
WMF_FUNCTION(SETPALENTRIES, 0x0037)
WMF_FUNCTION(POLYPOLYGON, 0x0538)
WMF_FUNCTION(RESIZEPALETTE, 0x0139)
WMF_FUNCTION(DIBBITBLT, 0x0940)
WMF_FUNCTION(DIBSTRETCHBLT, 0x0b41)
WMF_FUNCTION(DIBCREATEPATTERNBRUSH, 0x0142)
WMF_FUNCTION(STRETCHDIB, 0x0f43)
WMF_FUNCTION(EXTFLOODFILL, 0x0548)
WMF_FUNCTION(SETLAYOUT, 0x0149)
WMF_FUNCTION(DELETEOBJECT, 0x01f0)
WMF_FUNCTION(CREATEPALETTE, 0x00f7)
WMF_FUNCTION(CREATEPATTERNBRUSH, 0x01F9)
WMF_FUNCTION(CREATEPENINDIRECT, 0x02FA)
WMF_FUNCTION(CREATEFONTINDIRECT, 0x02FB)
WMF_FUNCTION(CREATEBRUSHINDIRECT, 0x02FC)
WMF_FUNCTION(CREATEREGION, 0x06FF)