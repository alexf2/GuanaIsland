// ==========================================================================
//				Class Implementation : COXBitmapButton
// ==========================================================================

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved                      
			  
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXBitmapButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXBitmapButton, CButton)

#define new DEBUG_NEW

// We define the structs GRPICONDIR, GRPICONDIRENTRY and ICONIMAGE
// which represent an icon directory and an icon from resource
// See also 'Icons in Win32' (John Hornick - Microsoft Corporation)

// ... #pragmas are used here to insure that the structure's
//     packing in memory matches the packing of the EXE or DLL.
#pragma pack( push )
#pragma pack( 2 )
	typedef struct
		{
		BYTE   bWidth;					// Width, in pixels, of the image
		BYTE   bHeight;					// Height, in pixels, of the image
		BYTE   bColorCount;				// Number of colors in image (0 if >=8bpp)
		BYTE   bReserved;				// Reserved
		WORD   wPlanes;					// Color Planes
		WORD   wBitCount;				// Bits per pixel
		DWORD  dwBytesInRes;			// how many bytes in this resource?
		WORD   nID;						// the ID
		} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;
	
	typedef struct 
		{
		WORD            idReserved;		// Reserved (must be 0)
		WORD            idType;			// Resource type (1 for icons)
		WORD            idCount;		// How many images?
		GRPICONDIRENTRY idEntries[1];	// The entries for each image (idCount of 'em)
		} GRPICONDIR, *LPGRPICONDIR;

	typedef struct
		{
		BITMAPINFOHEADER	icHeader;	// DIB header
		RGBQUAD				icColors[1]; // Color table (more than 1!)
		BYTE				icXOR[1];	// DIB bits for XOR mask (more than 1!)
		BYTE				icAND[1];	// DIB bits for AND mask (more than 1!)
		} ICONIMAGE, *LPICONIMAGE;
#pragma pack( pop )

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

//	Button with image (and text) layout

//   +----------------------------------------+
//   |                                        |
//   |                 By                     |
//   |                                        |
//   |         +------------------+           |
//   |         |                  |           |
//   |         |                  |           |
//   |         |                  |           |
//   |         |                  |           |
//   |   Bx    |   B I T M A P    |    Bx     |
//   |         |                  |           |
//   |         |                  |           |
//   |         |                  |           |
//   |         |                  |           |
//   |         +------------------+           |
//   |                                        |
//   |                 By                     |
//   |                                        |
//   |       +----------------------+         |
//   |  Tx   |       T E X T        |   Tx    |
//   |       +----------------------+         |
//   |                                        |
//   |                 Ty                     |
//   |                                        |
//   +----------------------------------------+

// Bx = m_ptImageOffset.x
// By = m_ptImageOffset.y
// Tx = m_ptTextOffset.x
// Ty = m_ptTextOffset.y

// ... The number of pixels the picture is offset from the button border (or the text)
//CPoint COXBitmapButton::m_ptImageOffset = CPoint(-3,-3);
// ... The number of pixels the text is offset from the button border 
CPoint COXBitmapButton::m_ptTextOffset = CPoint(-5,-4);
// ... The number of pixels the outer focus rect is offset from the button border
CPoint COXBitmapButton::m_ptOuterFocusOffset = CPoint(0,0);
// ... The number of pixels the inner focus rect is offset from the button border
CPoint COXBitmapButton::m_ptInnerFocusOffset = CPoint(-3,-3);
// ... The size in pixels of the focus rect when in hyper look mode
CSize COXBitmapButton::m_hyperFocusSize = CPoint(6, 5);
// ... How much the image and the text must move when the button is down
CPoint COXBitmapButton::m_ptDownOffset = CPoint(1,1);
// ... How much the image and the text must move when the mouse
//     is over the button in hyper look (and track look is also enabled)
CPoint COXBitmapButton::m_ptHyperOffset = CPoint(-1,-1);

// Position of the images in the image list
const int COXBitmapButton::m_nNormalImageIndex = 0;
const int COXBitmapButton::m_nInactiveImageIndex = 1;
const int COXBitmapButton::m_nDisabledImageIndex = 2;

// The default color of the face of a button
COLORREF COXBitmapButton::m_defaultButtonColor = ::GetSysColor(COLOR_BTNFACE);

// Data members -------------------------------------------------------------
// protected:
	// CImageList m_imageList;
	// --- Image list containing all the images needed for this button

	// CPalette m_palette;
	// --- The palette of the image, read from resource

	// COLORREF m_textColor;
	// --- The color used to draw text

	// CFont m_textFont;
	// --- The font used to draw text

	// CToolTipCtrl m_toolTip;
	/// --- The tooltip winodw of this button (if created)

	// BOOL m_bTrackLook;
	// --- Whether 'track look' should be used

	// BOOL m_bMouseOverButton;
	// --- Whether the mouse is over the button at the moment

	// BOOL m_bMouseDown;
	// --- Whether the mouse button is down (while mouse is captured)

	// BOOL m_bHyperLook;
	// --- Whether this button is in hyper look mode

	// CImageList m_backgroundImage;
	// --- A grabbed image of the button's background (needed in hyper look mode)

	// BOOL m_bBackgroundGrabbed;
	// --- Whether the background image has been grabbed 
	//     (and thus the contents of m_backgroundImage is valid)

	// UINT m_nDefaultCursorID;
	// --- The ID of the default cursor (or 0 when not et)
	// HCURSOR m_hDefaultCursor;
	// --- The default cursor (or NULL when not et)

	// UINT m_nDisabledCursorID;
	// --- The ID of the disabled cursor (or 0 when not et)
	// HCURSOR m_hDisabledCursor;
	// --- The disabled cursor (or NULL when not et)

	// BOOL m_bPseudoDisableMode;
	// --- Whether this button is in pseudo disabled mode

	// BOOL m_bEnabled;
	// --- Whether this button is enabled.  
	//     This value is only valid in pseudeo disable mode

	// BOOL m_bHasTabStop;
	// --- Whether this button had a tab stop before it was disabled in pseudeo disable mode

// private:
	
// Member functions ---------------------------------------------------------
// public:

BEGIN_MESSAGE_MAP(COXBitmapButton, CButton)
	//{{AFX_MSG_MAP(COXBitmapButton)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONUP()
	ON_WM_SYSKEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_SYSKEYDOWN()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_CHECK_TRACK_LOOK, OnCheckTrackLook)
END_MESSAGE_MAP()

COXBitmapButton::COXBitmapButton()
	:
 m_ptImageOffset( -3, -3 ),
	m_textColor(RGB(0, 0, 0)),
	m_bTrackLook(FALSE),
	m_bMouseOverButton(FALSE),
	m_bMouseDown(FALSE),
	m_bHyperLook(FALSE),
	m_bBackgroundGrabbed(FALSE),
	m_nDefaultCursorID(0),
	m_hDefaultCursor(NULL),
	m_nDisabledCursorID(0),
	m_hDisabledCursor(NULL),
	m_bPseudoDisableMode(FALSE),
	m_bEnabled(FALSE)
	{
	ASSERT_VALID(this);
	}

BOOL COXBitmapButton::LoadBitmap(LPCTSTR lpszBitmapResource, BOOL bResize /* = TRUE */,
	COLORREF crMask /* = CLR_NONE */)
	{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

#ifdef _DEBUG
	if (AfxFindResourceHandle(lpszBitmapResource, RT_BITMAP) == NULL)
		TRACE0("COXBitmapButton::LoadBitmap : Specified bitmap resource not found\n");
#endif // _DEBUG

	// BS_BITMAP style is no longer supported
	// This button must have the BS_OWNERDRAW style
	ASSERT((GetStyle() & BS_BITMAP) != BS_BITMAP);
	ASSERT((GetStyle() & BS_OWNERDRAW) == BS_OWNERDRAW);

	// Clear possible previous contents
	if (m_imageList.m_hImageList != NULL)
		VERIFY(m_imageList.DeleteImageList());

	// Get and compute the necessary bitmaps
	CBitmap bitmap;
	CBitmap grayBitmap;
	CBitmap disabledBitmap;

	// Try to load the new bitmap
	if (!LoadBitmap(lpszBitmapResource, bitmap, m_palette))
		{
		TRACE0("COXBitmapButton::LoadBitmap : Failed to load bitmap \n");
		return FALSE;
		}

	// Build the gray bitmap 
	if (!BuildGrayBitmap(lpszBitmapResource, crMask, &grayBitmap))
		TRACE0("COXBitmapButton::LoadBitmap : Failed to build the gray scale bitmap, continuing\n");

	BITMAP bmInfo;
	ZeroMemory(&bmInfo, sizeof(bmInfo));
	VERIFY(bitmap.GetObject(sizeof(bmInfo), &bmInfo) == sizeof(bmInfo));
	CSize bitmapSize(bmInfo.bmWidth, bmInfo.bmHeight);
	BOOL bMask = crMask != CLR_NONE; 

	// Let the imlage list use a device-dependent bitmap for its internal data structure
	// instead of the default ILC_COLOR4 (4-bit (16 color) device-independent bitmap (DIB) section)
	// Otherwise the colors will shift (e.g. disabled button look)
	VERIFY(m_imageList.Create(bitmapSize.cx, bitmapSize.cy, bMask | ILC_COLORDDB, 0, OX_MAX_IMAGE_COUNT));
	VERIFY(m_imageList.Add(&bitmap, crMask) == m_nNormalImageIndex);
	VERIFY(m_imageList.Add(&grayBitmap, crMask) == m_nInactiveImageIndex);

	// Build the disabled image
	HICON hSourceIcon = m_imageList.ExtractIcon(m_nNormalImageIndex);
	HICON hDestIcon = NULL;
	if (BuildDisabledImage(hSourceIcon, bitmapSize, hDestIcon))
		{
		// Add the disabled image to the image list and use the
		// mask of the normal image
		VERIFY(m_imageList.Add(hDestIcon) == m_nDisabledImageIndex);
		VERIFY(::DestroyIcon(hDestIcon));
		}
	else
		TRACE0("COXBitmapButton::LoadBitmap : Failed to build the disabled image, continuing\n");
	VERIFY(::DestroyIcon(hSourceIcon));


	// Size to content
	if (bResize)
		SizeToContent();

	return TRUE;
	}

BOOL COXBitmapButton::LoadBitmap(UINT nIDBitmapResource, BOOL bResize /* = TRUE */,
	COLORREF crMask /* = CLR_NONE */)
	{ 
	return LoadBitmap(MAKEINTRESOURCE(nIDBitmapResource), bResize, crMask); 
	}

BOOL COXBitmapButton::LoadIcon(LPCTSTR lpszIconResource, BOOL bResize /* = TRUE */)
	{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

#ifdef _DEBUG
	if (AfxFindResourceHandle(lpszIconResource, RT_GROUP_ICON) == NULL)
		TRACE0("COXBitmapButton::LoadIcon : Specified icon resource not found\n");
#endif // _DEBUG

	// BS_BITMAP style is no longer supported
	// This button must have the BS_OWNERDRAW style
	ASSERT((GetStyle() & BS_BITMAP) != BS_BITMAP);
	ASSERT((GetStyle() & BS_OWNERDRAW) == BS_OWNERDRAW);

	// Clear possible previous contents
	if (m_imageList.m_hImageList != NULL)
		VERIFY(m_imageList.DeleteImageList());
	
	// Get and compute the necessary bitmaps
	HICON hIcon;
	CBitmap grayBitmap;
	CBitmap disabledBitmap;

	// Try to load the new bitmap
	hIcon = AfxGetApp()->LoadIcon(lpszIconResource);
	if (hIcon == NULL)
		{
		TRACE0("COXBitmapButton::LoadIcon : Failed to load icon\n");
		return FALSE;
		}

	// Get the icon's palette
	if (!GetIconPalette(lpszIconResource, m_palette))
		TRACE0("COXBitmapButton::LoadIcon : Failed to get the palette, continuing\n");

	// Build the gray bitmap and gray palette
	HICON hGrayIcon = NULL;
	if (!BuildGrayIcon(lpszIconResource, &hGrayIcon))
		TRACE0("COXBitmapButton::LoadIcon : Failed to build the gray scale icon, continuing\n");

	ICONINFO iconInfo;
	ZeroMemory(&iconInfo, sizeof(iconInfo));
	VERIFY(GetIconInfo(hIcon, &iconInfo));

	BITMAP bmInfo;
	ZeroMemory(&bmInfo, sizeof(bmInfo));
	VERIFY(::GetObject(iconInfo.hbmColor, sizeof(bmInfo), &bmInfo) == sizeof(bmInfo));
	CSize iconSize(bmInfo.bmWidth, bmInfo.bmHeight);

	// Let the imlage list use a device-dependent bitmap for its internal data structure
	// instaed of the default ILC_COLOR4 (4-bit (16 color) device-independent bitmap (DIB) section)
	// Otherwise the colors will shift (e.g. disabled button look)
	VERIFY(m_imageList.Create(iconSize.cx, iconSize.cy, TRUE | ILC_COLORDDB, 0, OX_MAX_IMAGE_COUNT));
	VERIFY(m_imageList.Add(hIcon) == m_nNormalImageIndex);
	VERIFY(m_imageList.Add(hGrayIcon) == m_nInactiveImageIndex);

	// Build the disabled bitmap
	HICON hDestIcon = NULL;
	if (BuildDisabledImage(hIcon, iconSize, hDestIcon))
		{
		// Add the disabled image to the image list and use the
		// mask of the normal image
		VERIFY(m_imageList.Add(hDestIcon) == m_nDisabledImageIndex);
		VERIFY(::DestroyIcon(hDestIcon));
		}
	else
		TRACE0("COXBitmapButton::LoadIcon : Failed to build the disabled image, continuing\n");

	// Size to content
	if (bResize)
		SizeToContent();

	return TRUE;
	}

BOOL COXBitmapButton::LoadIcon(UINT nIDIconResource, BOOL bResize /* = TRUE */)
	{
	return LoadIcon(MAKEINTRESOURCE(nIDIconResource), bResize); 
	}

CPalette* COXBitmapButton::GetPalette()
	{
	return &m_palette;
	}

void COXBitmapButton::SizeToContent()
	{
	ASSERT(::IsWindow(m_hWnd));

	CSize oldButtonSize = GetButtonSize();
	CSize newButtonSize = GetFitButtonSize();
	VERIFY(SetWindowPos(NULL, -1, -1, newButtonSize.cx, newButtonSize.cy,
		SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE));

	// Invalidate the union of the old and the new button rect
	CRect invalidRect;
	invalidRect.UnionRect(CRect(CPoint(0, 0), oldButtonSize), CRect(CPoint(0, 0), newButtonSize));
	// ... Invalidate through the parent window, because this button may have shrunk
	//     and thus part of the parent window has to be invalidated as well
	ClientToScreen(invalidRect);
	ASSERT(GetParent() != NULL);
	GetParent()->ScreenToClient(invalidRect);
	GetParent()->InvalidateRect(invalidRect);
	}

BOOL COXBitmapButton::GetTrackLook() const
	{
	return m_bTrackLook;
	}

BOOL COXBitmapButton::SetTrackLook(BOOL bTrackLook /* = TRUE */)
	{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

	if ((GetStyle() & BS_OWNERDRAW) != BS_OWNERDRAW)
		{
		TRACE0("COXBitmapButton::SetTrackLook : Can only set track look for owner draw buttons, failing\n");
		return FALSE;
		}

	if (bTrackLook != m_bTrackLook)
		{
		m_bTrackLook = bTrackLook;
		Invalidate();
		}
	return TRUE;
	}

void COXBitmapButton::SetHorizontalAlignment(DWORD nAlignment /* = BS_CENTER */)
	{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT((nAlignment == BS_LEFT) || (nAlignment == BS_CENTER) || (nAlignment == BS_RIGHT));

	if (GetHorizontalAlignment() != nAlignment)
		{
		ModifyStyle((BS_LEFT | BS_CENTER | BS_RIGHT), nAlignment);
		// ... Alignment changed, redraw button
		Invalidate();
		}
	}

DWORD COXBitmapButton::GetHorizontalAlignment() const
	{
	ASSERT(::IsWindow(m_hWnd));
	return GetStyle() & (BS_LEFT | BS_CENTER | BS_RIGHT);
	}

void COXBitmapButton::SetVerticalAlignment(DWORD nAlignment /* = BS_VCENTER */)
	{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT((nAlignment == BS_TOP) || (nAlignment == BS_VCENTER) || (nAlignment == BS_BOTTOM));

	if (GetVerticalAlignment() != nAlignment)
		{
		ModifyStyle((BS_TOP | BS_VCENTER | BS_BOTTOM), nAlignment);
		// ... Alignment changed, redraw button
		Invalidate();
		}
	}

DWORD COXBitmapButton::GetVerticalAlignment() const
	{
	ASSERT(::IsWindow(m_hWnd));
	return GetStyle() & (BS_TOP | BS_VCENTER | BS_BOTTOM);
	}

COLORREF COXBitmapButton::GetTextColor() const
	{
	return m_textColor;
	}

void COXBitmapButton::SetTextColor(COLORREF textColor)
	{
	if (textColor != m_textColor)
		{
		m_textColor = textColor;
		// ... Text color changed, redraw the button
		Invalidate();
		}
	}

CFont* COXBitmapButton::GetTextFont()
	{
	ASSERT(::IsWindow(m_hWnd));

	// Get the font associated with this control 
	CFont* pFont = GetFont();
	if (pFont == NULL)
		{
		TRACE0("COXBitmapButton::GetTextFont : Failed to get the font\n");
		// ... Return an empty font
		m_textFont.DeleteObject();
		}
	else if (pFont->GetSafeHandle() == (HFONT)m_textFont.m_hObject)
		{
		// If the handles are the same, the wrapping object must be the same as well
		ASSERT(pFont == &m_textFont);
		}
	else
		{
		// The window font is different from the font we have
		// (This means the font has not been set with SetTextFont() but with WM_SETFONT)
		// Make a copy of the window font
		LOGFONT logFont;
		m_textFont.DeleteObject();
		VERIFY(pFont->GetLogFont(&logFont));
		VERIFY(m_textFont.CreateFontIndirect(&logFont));
		}

	return &m_textFont;
	}

void COXBitmapButton::SetTextFont(CFont* pTextFont)
	{
	ASSERT(pTextFont != NULL);
	ASSERT(::IsWindow(m_hWnd));

	// Make a copy of the supplied font if necessary
	BOOL bChanged = FALSE;
	if (pTextFont->GetSafeHandle() != (HFONT)m_textFont.m_hObject)
		{
		LOGFONT logFont;
		m_textFont.DeleteObject();
		VERIFY(pTextFont->GetLogFont(&logFont));
		VERIFY(m_textFont.CreateFontIndirect(&logFont));
		bChanged = TRUE;
		}

	// ... Set the window font
	SetFont(&m_textFont, bChanged);
	}

CString COXBitmapButton::GetText() const
	{
	ASSERT(::IsWindow(m_hWnd));

	CString sText;
	GetWindowText(sText);
	// Get everything in front of first EndOfLine char
	return GetSubString(sText, 1, _T('\n'));
	}

void COXBitmapButton::SetText(LPCTSTR pszText)
	{
	ASSERT(::IsWindow(m_hWnd));

	CString sWindowText(pszText);
	// ... Get the tooltip text so it can be used again
	CString sToolTipText = GetToolTipText();
	if (!sToolTipText.IsEmpty())
		sWindowText += _T("\n") + sToolTipText;
	SetWindowText(sWindowText);
	}

CString COXBitmapButton::GetToolTipText() const
	{
	ASSERT(::IsWindow(m_hWnd));

	CString sText;
	GetWindowText(sText);
	// Get everything after the first EndOfLine char and before te second
	return GetSubString(sText, 2, _T('\n'));
	}

void COXBitmapButton::SetToolTipText(LPCTSTR pszToolTipText)
	{
	ASSERT(::IsWindow(m_hWnd));

	CString sToolTipText(pszToolTipText);
	// ... Get the text so it can be used again
	CString sWindowText = GetText();
	if (!sToolTipText.IsEmpty())
		sWindowText += _T("\n") + sToolTipText;

	// A tool will be added to (or deleted from) the tool tip control 
	// in the WM_SETTEXT handler

	SetWindowText(sWindowText);
	}

BOOL COXBitmapButton::GetToolTip() const
	{
	ASSERT(::IsWindow(m_hWnd));

	// Tooltip is enabled if the tool tip control is creted
	return (m_toolTip.m_hWnd != NULL);
	}

BOOL COXBitmapButton::SetToolTip(BOOL bEnable /* = TRUE */)
	{
	ASSERT(::IsWindow(m_hWnd));

	if (bEnable)
		{
		if (m_toolTip.m_hWnd != NULL)
			{
			::IsWindow(m_toolTip.m_hWnd);
			TRACE0("COXBitmapButton::SetToolTip : Tooltip window already exists, continuing\n");
			return TRUE;
			}
		if (!m_toolTip.Create(this))
			{
			TRACE0("COXBitmapButton::SetToolTip : Failed to create tool tip control\n");
			return FALSE;
			}
		m_toolTip.Activate(TRUE);

		// ... Set the initial tool tip text
		CString sToolTipText = GetToolTipText();
		if (!sToolTipText.IsEmpty())
			VERIFY(m_toolTip.AddTool(this, sToolTipText));
		}
	else if (m_toolTip.m_hWnd != NULL)
		m_toolTip.DestroyWindow();

	return TRUE;
	}

CSize COXBitmapButton::GetFitButtonSize()
	{
	CSize imageAndGapSize;
	CSize textAndGapSize;
	CSize totalSize;

	// Get the needed sizes for image and text
	imageAndGapSize = GetImageSize() + CSize(2 * -m_ptImageOffset.x, 2 * -m_ptImageOffset.y);
	textAndGapSize = GetTextSize();
	if ((textAndGapSize.cx != 0) && (textAndGapSize.cy != 0))
		// ... Only add text gap if text is non-empty
		textAndGapSize += CSize(2 * -m_ptTextOffset.x, -m_ptTextOffset.y);

	// Use the sum for the height and the largest for the width 
	if (imageAndGapSize.cx < textAndGapSize.cx)
		totalSize.cx = textAndGapSize.cx;
	else
		totalSize.cx = imageAndGapSize.cx;
	totalSize.cy = imageAndGapSize.cy + textAndGapSize.cy;

	return totalSize;
	}

CSize COXBitmapButton::GetButtonSize() const
	{
	ASSERT(::IsWindow(m_hWnd));

	CRect clientRect;
	GetClientRect(clientRect);
	return clientRect.Size();
	}

CSize COXBitmapButton::GetImageSize() const
	{
	ASSERT(::IsWindow(m_hWnd));

	if (m_imageList.m_hImageList == NULL)
		return CSize(0, 0);

	IMAGEINFO imageInfo;
	::ZeroMemory(&imageInfo, sizeof(imageInfo));
	VERIFY(m_imageList.GetImageInfo(m_nNormalImageIndex, &imageInfo));
	return CRect(imageInfo.rcImage).Size();
	}

CSize COXBitmapButton::GetTextSize()
	{
	ASSERT(::IsWindow(m_hWnd));

	CWindowDC dc(this);
	CRect textRect(0, 0, 0, 0);
	CString sText = GetText();
	if (!sText.IsEmpty())
		{
		// ... Use correct font
		CFont* pOldFont = dc.SelectObject(GetTextFont());
		VERIFY(dc.DrawText(GetText(), textRect, DT_CALCRECT | DT_LEFT | DT_SINGLELINE | DT_TOP) != 0);
		dc.SelectObject(pOldFont);
		}
	return textRect.Size();
	}

BOOL COXBitmapButton::GetHyperLook() const
	{
	return m_bHyperLook;
	}

BOOL COXBitmapButton::SetHyperLook(BOOL bHyperLook /* = TRUE */)
	{
	// ... Set the mode state
	m_bHyperLook = bHyperLook;
	// ... Change the cursors by default
	SetDefaultCursor(bHyperLook ? IDC_OX_HAND_CURSOR : 0);
	SetDisabledCursor(bHyperLook ? IDC_OX_NO_HAND_CURSOR : 0);
	if (bHyperLook)
		{
		// ... Use the pseuco-disable mode (necessary for cursors)
		SetPseudoDisableMode(TRUE);
		// ... Regrab the background 
		m_bBackgroundGrabbed = FALSE;
		}

	CRect rect;
	GetWindowRect(rect);
	CWnd* pParent = GetParent();
	ASSERT(pParent != NULL);
	pParent->ScreenToClient(rect);
	pParent->RedrawWindow(rect, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_ERASE);

	return TRUE;
	}

BOOL COXBitmapButton::RegrabBackground()
	{
	// ... Check whether we have the focus
	BOOL bFocus = (GetFocus() == this);

	// Hide and show the window (parent background will be repainted
	// and grabbed during OnEraseBkgnd()
	// ... Discard a possible previous copy
	m_bBackgroundGrabbed = FALSE;
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOWNA);

	// ... Restore the focus if necessary
	if (bFocus)
		SetFocus();

	return m_bBackgroundGrabbed;
	}

BOOL COXBitmapButton::SetDefaultCursor(UINT nCursorID /* = 0 */)
	{
	// Check whether we are reseting the cursor
	if (nCursorID == 0)
		{
		m_nDefaultCursorID = 0;
		m_hDefaultCursor = NULL;
		return TRUE;
		}

	// The cursor is read from resource now
	//     (Make sure OXBitmapButton.rc is included in your resource file)
	HCURSOR hNewCursor = AfxGetApp()->LoadCursor(nCursorID);
	if (hNewCursor != NULL)
		{
		m_nDefaultCursorID = nCursorID;
		m_hDefaultCursor = hNewCursor;
		return TRUE;
		}
	else
		{
		TRACE1("COXBitmapButton::SetDefaultCursor : Failed to load cursor with ID %i\n",
			nCursorID);
		return FALSE;
		}
	}

UINT COXBitmapButton::GetDefaultCursor() const
	{
	return m_nDefaultCursorID;
	}

BOOL COXBitmapButton::SetDisabledCursor(UINT nCursorID /* = 0 */)
	{
	// Check whether we are reseting the cursor
	if (nCursorID == 0)
		{
		m_nDisabledCursorID = 0;
		m_hDisabledCursor = NULL;
		return TRUE;
		}

	// The cursor is read from resource now
	//     (Make sure OXBitmapButton.rc is included in your resource file)
	HCURSOR hNewCursor = AfxGetApp()->LoadCursor(nCursorID);
	if (hNewCursor != NULL)
		{
		m_nDisabledCursorID = nCursorID;
		m_hDisabledCursor = hNewCursor;
		return TRUE;
		}
	else
		{
		TRACE1("COXBitmapButton::SetDisabledCursor : Failed to load cursor with ID %i\n",
			nCursorID);
		return FALSE;
		}
	}

UINT COXBitmapButton::GetDisabledCursor() const
	{
	return m_nDisabledCursorID;
	}

BOOL COXBitmapButton::GetPseudoDisableMode() const
	{
	return m_bPseudoDisableMode;
	}

BOOL COXBitmapButton::SetPseudoDisableMode(BOOL bPseudoDisableMode /* = TRUE */)
	{
	if (m_bPseudoDisableMode != bPseudoDisableMode)
		{
		if (bPseudoDisableMode)
			{
			// ... Changing from normal mode to pseudo-disable mode 
			m_bEnabled = IsWindowEnabled();
			m_bPseudoDisableMode = TRUE;
			m_bHasTabStop = ((GetStyle() & WS_TABSTOP) == WS_TABSTOP);
			// ... Always enable the window (in real Windows sense)
			//     and remove tabstop if necessary
			CButton::EnableWindow(TRUE);
			EnableWindow(m_bEnabled);
			}
		else
			{
			// ... Changing from pseudo-disable mode to normal mode
			m_bPseudoDisableMode = FALSE;
			if (m_bHasTabStop)
				ModifyStyle(0, WS_TABSTOP);
			// ... Call the base class version
			CButton::EnableWindow(m_bEnabled);
			}
		}
	return TRUE;
	}

BOOL COXBitmapButton::IsWindowEnabled() const
	{
	if (!m_bPseudoDisableMode)
		return CButton::IsWindowEnabled();
	else
		// ... In pseudo-disable mode we return the stored state,
		//     not the actual window state (WS_DISABLED)
		return m_bEnabled;
	}

BOOL COXBitmapButton::EnableWindow(BOOL bEnable /* = TRUE */)
	{
	BOOL bOldEnabled = FALSE;
	if (!m_bPseudoDisableMode)
		{
		// ... Just pass it on to the base class
		bOldEnabled = CButton::EnableWindow(bEnable);
		}
	else
		{
		// Store the requested state, but reset the style
		bOldEnabled = m_bEnabled;
		m_bEnabled = bEnable;
		// ... If the window had the tab stop style : add or remove it
		if (m_bHasTabStop)
			ModifyStyle(!bEnable ? WS_TABSTOP : 0, bEnable ? WS_TABSTOP : 0);
		if (bOldEnabled != bEnable)
			//... Redraw the window after state change
			Invalidate();
		}
	return bOldEnabled;
	}

BOOL COXBitmapButton::AdjustBackground()
	{
	// Get the window rect 
	// ... The button rect expressed in screen coordinates
	CRect buttonInScreenRect;
	GetWindowRect(buttonInScreenRect);
	// ... The button rect expressed in button client coordinates
	CRect buttonInClientRect;
	buttonInClientRect = buttonInScreenRect;
	ScreenToClient(buttonInClientRect);
	// ... Make sure the border is grabbed as well
	buttonInScreenRect.right++;
	buttonInScreenRect.bottom++;

	// Convert it to parent coordinates
	CWnd* pParent = GetParent();
	ASSERT(pParent != NULL);

	// ... The parent rect expressed in screen coordinates
	CRect parentInScreenRect;
	pParent->GetWindowRect(parentInScreenRect);
	// ... The button rect expressed in parent coordinates
	CRect buttonInParentRect = buttonInClientRect;
	buttonInParentRect += buttonInScreenRect.TopLeft() - parentInScreenRect.TopLeft();

	// ... Get the parent's window DC
	CWindowDC windowDC(pParent);

	// Copy the image from the screen DC to the memory DC
	CDC memDC;
	CBitmap bitmap;
	VERIFY(memDC.CreateCompatibleDC(&windowDC));
	VERIFY(bitmap.CreateCompatibleBitmap(&windowDC, buttonInClientRect.Width(), buttonInClientRect.Height()));
    // ... Select new bitmap into memory DC
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
    // ... Copy the bitmap from the window DC to memory DC
    memDC.BitBlt(0, 0, buttonInClientRect.Width(), buttonInClientRect.Height(), &windowDC, 
		buttonInParentRect.left, buttonInParentRect.top, SRCCOPY);

    // ... Restore the old bitmap of the memory DC 
	memDC.SelectObject(pOldBitmap);

	// ... First delete an image that may be present from previous grabs
	m_backgroundImage.DeleteImageList();
	VERIFY(m_backgroundImage.Create(buttonInClientRect.Width(), buttonInClientRect.Height(), 
		ILC_COLORDDB, 0, 1));
	VERIFY(m_backgroundImage.Add(&bitmap, CLR_NONE) == 0);

	return TRUE;
	}

#ifdef _DEBUG
void COXBitmapButton::AssertValid() const
	{
	CButton::AssertValid();
	}

void COXBitmapButton::Dump(CDumpContext& dc) const
	{
	CButton::Dump(dc);
	}
#endif //_DEBUG

COXBitmapButton::~COXBitmapButton()
	{
	}

void COXBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
	{
	ASSERT(lpDrawItemStruct != NULL);

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	UINT nState = lpDrawItemStruct->itemState;
	CRect itemRect = lpDrawItemStruct->rcItem;
	CRect buttonRect;
	CRect imageRect;
	CRect textRect;

	// Adjust the state for a possible pseudo-disable mode
	if (IsWindowEnabled())
		nState &= ~ODS_DISABLED;
	else
		nState |= ODS_DISABLED;

	// First distribute the available space (image etc)
	DistributeSpace(nState, itemRect, buttonRect, imageRect, textRect);

	// ... Select and realize palette
	CPalette* pOldPalette = NULL;
	SelectPalette(pDC, nState, buttonRect, pOldPalette);

	// ... Draw the button borders 
	DrawButton(pDC, nState, buttonRect);
	// ... Draw the image in its correct state
	DrawImage(pDC, nState, &m_imageList, imageRect);
	DrawText(pDC, nState, GetText(), textRect);
	if (nState & ODS_FOCUS)
		// ... Draw a focus rectangle on top of the button and image
		DrawFocusRectangle(pDC, nState, buttonRect, imageRect);

	// ... Deselect palette
	RestorePalette(pDC, nState, buttonRect, pOldPalette);
	}

// protected:
void COXBitmapButton::DistributeSpace(UINT nState, CRect itemRect, CRect& buttonRect, CRect& imageRect, CRect& textRect)
	// --- In  : nState : The state of the button
	//			 itemRect : The avaliable space of the entire button
	// --- Out : buttonRect : Space used for the button
	//			 imageRect : Spece used for the image
	// --- Returns :
	// --- Effect : Distributes the avaliable space between the button, the image atc.
	{
	// Let the button use the entire space
	buttonRect = itemRect;

	// ... Outer allowable position for image and text
	const CRect rectLimit = buttonRect;
	CPoint originLimit = rectLimit.TopLeft();
	CSize sizeLimit = rectLimit.Size();

	// Calculate the text size
	CSize textSize = GetTextSize();
	// ... Set text and gap to CSize(0,0) if no text
	CSize textAndGapSize = CSize(0, 0);
	if ((textSize.cx != 0) && (textSize.cy != 0))
		{
		// ... Set text and gap to correct value if text exists
		textAndGapSize = textSize + CSize(-m_ptTextOffset.x * 2, -m_ptTextOffset.y);
		// ... Adjust size if already to big
		if (sizeLimit.cx < textAndGapSize.cx)
			{
			textAndGapSize.cx = sizeLimit.cx;
			textSize.cx = textAndGapSize.cx - 2 * (-m_ptTextOffset.x);
			if (textSize.cx < 0)
				textSize.cx = 0;
			}
		if (sizeLimit.cy < textAndGapSize.cy)
			{
			textAndGapSize.cy = sizeLimit.cy;
			textSize.cy = textAndGapSize.cy - (-m_ptTextOffset.y);
			if (textSize.cy < 0)
				textSize.cy = 0;
			}

		// ... Subtract text and space size from limit size (only height)
		sizeLimit.cy -= textAndGapSize.cy;
		}
	// ... May not have used more size than available
	ASSERT((0 <= sizeLimit.cx) || (0 <= sizeLimit.cy));

	// Calculate the image size
	CSize imageSize = GetImageSize();
	CSize imageAndGapSize = imageSize + CSize(2 * -m_ptImageOffset.x, 2 * -m_ptImageOffset.y);
	// ... Adjust size if already to big
	if (sizeLimit.cx < imageAndGapSize.cx)
		{
		imageAndGapSize.cx = sizeLimit.cx;
		imageSize.cx = imageAndGapSize.cx - 2 * (-m_ptImageOffset.x);
		if (imageSize.cx < 0)
			imageSize.cx = 0;
		}
	if (sizeLimit.cy < imageAndGapSize.cy)
		{
		imageAndGapSize.cy = sizeLimit.cy;
		imageSize.cy = imageAndGapSize.cy - 2 * (-m_ptImageOffset.y);
		if (imageSize.cy < 0)
			imageSize.cy = 0;
		}
	// ... Subtract image and space size from limit size (only height)
	sizeLimit.cy -= imageAndGapSize.cy;

	// ... May not have used more size than available
	//     The width that is still left will be used between the image and the border
	//     or the text and the border (not between the text and the image)
	ASSERT((0 <= sizeLimit.cx) || (0 <= sizeLimit.cy));
	// ... The width shouldn't have changed
	ASSERT(sizeLimit.cx == rectLimit.Size().cx);

	// Calculate the image and text position
	CPoint textOrigin = originLimit + -m_ptTextOffset;
	CPoint imageOrigin = originLimit + -m_ptImageOffset;

	// Take horizontal alignment into account
	// ... Get the alignment
	DWORD nHorizontalAlignment = GetHorizontalAlignment();
	DWORD nVerticalAlignment = GetVerticalAlignment();

	if (nHorizontalAlignment == BS_LEFT)
		{
		ASSERT(textOrigin.x == originLimit.x + -m_ptTextOffset.x);
		ASSERT(imageOrigin.x == originLimit.x + -m_ptImageOffset.x);
		}
	else if (nHorizontalAlignment == BS_RIGHT)
		{
		textOrigin.x = sizeLimit.cx - textAndGapSize.cx + -m_ptTextOffset.x;
		imageOrigin.x = sizeLimit.cx - imageAndGapSize.cx + -m_ptImageOffset.x;
		}
	else // (nHorizontalAlignment == BS_CENTER) or other
		{
		textOrigin.x = (sizeLimit.cx - textAndGapSize.cx) / 2 + -m_ptTextOffset.x;
		imageOrigin.x = (sizeLimit.cx - imageAndGapSize.cx) / 2 + -m_ptImageOffset.x;
		}

	// Take vertical alignment into account
	if (nVerticalAlignment == BS_TOP)
		{
		// ... Image at top, text underneath
		ASSERT(imageOrigin.y == originLimit.y + -m_ptImageOffset.y);
		textOrigin.y = originLimit.y + imageAndGapSize.cy;
		}
	else if (nVerticalAlignment == BS_BOTTOM)
		{
		// ... Image at bottom, text above it
		imageOrigin.y = rectLimit.Height() - (imageSize.cy + -m_ptImageOffset.y);
		textOrigin.y = imageOrigin.y - (-m_ptImageOffset.y) - textSize.cy;
		}
	else // (nVerticalAlignment == BS_VCENTER) or other
		{
		ASSERT(textOrigin.y == originLimit.y + -m_ptTextOffset.y);
		ASSERT(imageOrigin.y == originLimit.y + -m_ptImageOffset.y);
		// ... Image and text vertically centered (text under image)
		int nImageAndAndTextAndAllGaps = imageAndGapSize.cy + textAndGapSize.cy;
		int nTopBottomSpace = (rectLimit.Height() - nImageAndAndTextAndAllGaps) / 2 ;
		imageOrigin.y = nTopBottomSpace + -m_ptImageOffset.y;
		textOrigin.y = nTopBottomSpace + imageAndGapSize.cy;
		}

	// Assign image result
	imageRect = CRect(imageOrigin, imageSize);

	// Assign text result
	if ((textSize.cx != 0) && (textSize.cy != 0))
		textRect = CRect(textOrigin, textSize);
	else
		// ... No text to show
		textRect.SetRectEmpty();

	// Add small adjustments if necessary
	if ((nState & ODS_SELECTED) == ODS_SELECTED)
		{
		// ... If button is pressed : move the image and text a little bit
		//     to bottom right
		imageRect += m_ptDownOffset;
		textRect += m_ptDownOffset;
		}
	else if (((nState & ODS_DISABLED) != ODS_DISABLED) && m_bHyperLook && m_bMouseOverButton)
		{
		// ... Hyper look with mouse over button and not disabled : 
		//     move the image and text a little bit to top left
		imageRect += m_ptHyperOffset;
		textRect += m_ptHyperOffset;
		}

#ifdef _DEBUG
	// Calculated image rect should not be outside outer limit rect for image
	CRect imageDiffRect;
	imageDiffRect.SubtractRect(imageRect, rectLimit);
	//ASSERT(imageDiffRect.IsRectEmpty());

	// Calculated text rect should not be outside outer limit rect for text
	CRect textDiffRect;
	textDiffRect.SubtractRect(textRect, rectLimit);
	ASSERT(textDiffRect.IsRectEmpty());
#endif //_DEBUG
	}

void COXBitmapButton::SelectPalette(CDC* pDC, UINT /* nState */, CRect /* buttonRect */, CPalette*& pOldPalette)
	// --- In  : pDC : Device context to draw on
	//			 nState : The state of the button
	//			 buttonRect : The place where to draw the button
	// --- Out : pOldPalette : The previous (old) palette
	// --- Returns : 
	// --- Effect : This function may select a palette into the DC and realize it
	//				It may also use the oldPalette parameter to store the old palette
	{
	pOldPalette = NULL;
	if ((HPALETTE)m_palette != NULL)
		{
		pOldPalette = pDC->SelectPalette(&m_palette, FALSE);
		pDC->RealizePalette();
		}
	}

void COXBitmapButton::DrawButton(CDC* pDC, UINT nState, CRect buttonRect)
	// --- In  : pDC : Device context to draw on
	//			 nState : The state of the button
	//			 buttonRect : The place where to draw the button
	// --- Out : 
	// --- Returns : 
	// --- Effect : Draw the button look (with image etc.) in the correct state (pressed etc)
	{
	if (m_bHyperLook)
		{
		// Draw our own background while in hyper look mode
		// ... Background should have been grabbed already
		// ASSERT(m_bBackgroundGrabbed);
		VERIFY(m_backgroundImage.Draw(pDC, 0, buttonRect.TopLeft(), ILD_TRANSPARENT));
		return;
		}

	if ((nState & ODS_SELECTED) == ODS_SELECTED)
		{
		// ... Pressed in or checked
		pDC->DrawFrameControl(buttonRect, DFC_BUTTON , DFCS_BUTTONPUSH | DFCS_PUSHED);
		}
	else if (m_bTrackLook && (!m_bMouseOverButton || ((nState & ODS_DISABLED) == ODS_DISABLED)))
		{
		// ... Track look with mouse not over button or disabled state
		CBrush buttonBrush(m_defaultButtonColor);
		pDC->FillRect(buttonRect, &buttonBrush);
		}
	else if ((nState & ODS_DISABLED) == ODS_DISABLED)
		{
		// ... Disabled
		pDC->DrawFrameControl(buttonRect, DFC_BUTTON , DFCS_BUTTONPUSH | DFCS_INACTIVE);
		}
	else
		{
		// ... Normal look or Track look with mouse over button
		pDC->DrawFrameControl(buttonRect, DFC_BUTTON , DFCS_BUTTONPUSH);
		}
	}

void COXBitmapButton::DrawImage(CDC* pDC, UINT nState, CImageList* pImageList, CRect imageRect)
	// --- In  : pDC : Device context to draw on
	//			 nState : The state of the button
	//			 pImageList : The image list containing the images to draws
	//			 imageRect : The place where to draw the image
	// --- Out : 
	// --- Returns : 
	// --- Effect : Draw the correct image according to the state (disabled etc)
	{
	if ((pImageList == NULL) || (pImageList->m_hImageList == NULL))
		// ... Must have the image list created before we can draw something
		return;

	// Select the image to draw
	// ... Other scenarios
	int nImageIndex = m_nNormalImageIndex;
	if (!m_bHyperLook && ((nState & ODS_DISABLED) == ODS_DISABLED))
		// .. Disabled look (never used in hyper look)
		nImageIndex = m_nDisabledImageIndex;
	else if (m_bTrackLook && !m_bHyperLook && !m_bMouseOverButton && ((nState & ODS_SELECTED) != ODS_SELECTED))
		// ... Track look with mouse not over button and button not pressed
		//     and not in hyper look mode
		nImageIndex = m_nInactiveImageIndex;

	// Check whether image index is valid
	if (pImageList->GetImageCount() <= nImageIndex)
		{
		// ... Should have at least one image
		ASSERT(1 <= pImageList->GetImageCount());
		// ... The needed image is not available, use the normal image (best we can do)
		nImageIndex = m_nNormalImageIndex;
		}

	// Draw the image
	VERIFY(pImageList->Draw(pDC, nImageIndex, imageRect.TopLeft(), ILD_TRANSPARENT));
	}

void COXBitmapButton::DrawText(CDC* pDC, UINT nState, CString sText, CRect textRect)
	// --- In  : pDC : Device context to draw on
	//			 nState : The state of the button
	//			 sText : The text to draws
	//			 textRect : The place where to draw the text
	// --- Out : 
	// --- Returns : 
	// --- Effect : Draw the text in the correct state (disabled etc)
	{
	if (sText.IsEmpty())
		// ... Nothing to do : return immediately
		return;

	// ... Set the correct text color
	COLORREF oldColor = pDC->SetTextColor(GetTextColor());

	// Draw the text
	if (!m_bHyperLook && ((nState & ODS_DISABLED) == ODS_DISABLED))
		{
		// ... Disabled look
		if (!pDC->DrawState(textRect.TopLeft(), textRect.Size(), (LPCTSTR)sText,
			DSS_DISABLED, TRUE, 0, (HBRUSH)NULL))
			{
			// DrawState may fail on some platforms (Win NT 3.51)
			// Than we draw a gray string (better than nothing)
			CBrush brush;
			brush.CreateStockObject(::GetSysColor(COLOR_GRAYTEXT));
			sText = RemoveAmpersand(sText);

			VERIFY(pDC->GrayString(&brush, NULL, (LPARAM)(LPCTSTR)sText, 0, textRect.left, textRect.top, textRect.Width(), textRect.Height()));
			}
		}
	else if (!m_bHyperLook && m_bTrackLook && !m_bMouseOverButton)
		{
		// ... Track look with mouse not over button : Use black text color
		pDC->SetTextColor(oldColor);
		oldColor = pDC->SetTextColor(RGB(0, 0, 0));
		VERIFY(pDC->DrawText(sText, textRect, DT_LEFT | DT_SINGLELINE | DT_TOP) != 0);
		}
	else
		// ... Normal look
		VERIFY(pDC->DrawText(sText, textRect, DT_LEFT | DT_SINGLELINE | DT_TOP) != 0);

	// ... Reset the color
	pDC->SetTextColor(oldColor);
	}

void COXBitmapButton::DrawFocusRectangle(CDC* pDC, UINT nState, CRect buttonRect, CRect imageRect)
	// --- In  : pDC : Device context to draw on
	//			 buttonRect : The place where the button was drawn
	//			 imageRect : The place where the image was drawn
	// --- Out : 
	// --- Returns : 
	// --- Effect : Draw the focus rectangles
	{
	// Handle hyper look mode seperately
	if (m_bHyperLook)
		{
		// Center the focus mark in the image (not in the button)
	   	CRect innerFocusRect = imageRect;
		CSize deltaSize = innerFocusRect.Size() - m_hyperFocusSize;
		deltaSize.cx = -deltaSize.cx / 2;
		deltaSize.cy = -deltaSize.cy / 2;
		innerFocusRect.InflateRect(deltaSize);

		// Draw a focus circle
		CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);
		pDC->Ellipse(innerFocusRect);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
		return;
		}

	// Draw inner focus rect
   	CRect innerFocusRect = buttonRect;
	innerFocusRect.InflateRect(m_ptInnerFocusOffset.x, m_ptInnerFocusOffset.y);
	if ((nState & ODS_SELECTED) == ODS_SELECTED)
		innerFocusRect += m_ptDownOffset;
	pDC->DrawFocusRect(&innerFocusRect);

	// Draw outer focus rect (if not in track look mode)
	if (!m_bTrackLook)
		{
		CBrush blackBrush;
		VERIFY(blackBrush.CreateStockObject(BLACK_BRUSH));
		CRect outerFocusRect = buttonRect;
		outerFocusRect.InflateRect(m_ptOuterFocusOffset.x, m_ptOuterFocusOffset.y);
		pDC->FrameRect(outerFocusRect, &blackBrush);
		}
	}

void COXBitmapButton::RestorePalette(CDC* pDC, UINT /* nState */, CRect /* buttonRect */, CPalette* pOldPalette)
	// --- In  : pDC : Device context to draw on
	//			 nState : The state of the button
	//			 buttonRect : The place where to draw the button
	//			 pOldPalette : The previous (old) palette (may be NULL)
	// --- Out : 
	// --- Returns : 
	// --- Effect : This function may deselect a palette from the DC
	{
	if (pOldPalette != NULL)
		pDC->SelectPalette(pOldPalette, TRUE);
	}

BOOL COXBitmapButton::BuildGrayBitmap(LPCTSTR lpszBitmapResource, COLORREF crMask, CBitmap* pGrayBitmap)
	// --- In  : lpszBitmapResource : The bitmap resource to use
	//			 crMask : Mask color
	//			 pGrayBitmap : A (not yet attached) bitmap object
	// --- Out : pGrayBitmap : The attached bitmap object
	// --- Returns : Whether it was successful or not
	// --- Effect : Converts the supplied bitmap to a gray scale bitmap
	//				The color crMask is not converted
	{
	// Bitmap object should exist but not yet be attached
	ASSERT(pGrayBitmap != NULL);
	ASSERT(pGrayBitmap->m_hObject == NULL);

	// Get the bitmap resource data
	HINSTANCE hInstance = NULL;
	HRSRC hResourceInfoBlock = NULL;
	HGLOBAL hBitmapGlobal = NULL;
	HGLOBAL hBitmapCopy = NULL;
	DWORD nResourceSize = 0;
	LPBITMAPINFOHEADER  pBitmapInfoHeader = NULL;

	hInstance = AfxFindResourceHandle(lpszBitmapResource, RT_BITMAP);
	hResourceInfoBlock = ::FindResource(hInstance, lpszBitmapResource, RT_BITMAP);
	hBitmapGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if (hBitmapGlobal == NULL)
		{
		TRACE0("COXBitmapButton::BuildGrayBitmap : Failed to load bitmap resource, failing\n");
		return FALSE;
		}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(BITMAPINFOHEADER) <= nResourceSize);

	// Memory may be read only, make a copy 
	void* pOldBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hBitmapGlobal);
	hBitmapCopy = ::GlobalAlloc(GMEM_MOVEABLE, nResourceSize);
	pBitmapInfoHeader = (LPBITMAPINFOHEADER)::GlobalLock(hBitmapCopy);
	ASSERT(pBitmapInfoHeader != NULL);
	::CopyMemory(pBitmapInfoHeader, pOldBitmapInfoHeader, nResourceSize);
	
	// Change the bitmap color table to gray scale
	BOOL bGraySuccess = MakeGray(pBitmapInfoHeader, crMask);

	// Build the bitmap
	HBITMAP hBitmap = NULL;
	if (bGraySuccess)
		{
		int nNumColors = (1 << pBitmapInfoHeader->biBitCount);
		CClientDC dc(NULL);
		hBitmap = ::CreateDIBitmap(dc.m_hDC, pBitmapInfoHeader,	CBM_INIT,
			(LPBYTE)pBitmapInfoHeader + pBitmapInfoHeader->biSize + nNumColors * sizeof(RGBQUAD),
			(LPBITMAPINFO)pBitmapInfoHeader, DIB_RGB_COLORS);
		pGrayBitmap->Attach(hBitmap);
		}
	BOOL bBitmapSuccess = (hBitmap != NULL);
#ifdef _DEBUG
	if (!bBitmapSuccess)
		TRACE0("COXBitmapButton::BuildGrayBitmap : Failed to create the bitmap\n");
#endif // _DEBUG
	
	// Clean up
	::UnlockResource(hBitmapGlobal);
	if ((::GlobalUnlock(hBitmapCopy) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hBitmapCopy) == NULL);

	return bGraySuccess && bBitmapSuccess;
	}

BOOL COXBitmapButton::BuildGrayIcon(LPCTSTR lpszIconResource, HICON* phGrayIcon)
	// --- In  : lpszBitmapResource : The bitmap resource to use
	//			 crMask : Mask color
	//			 pGrayBitmap : A (not yet attached) bitmap object
	// --- Out : pGrayBitmap : The attached bitmap object
	// --- Returns : Whether it was successful or not
	// --- Effect : Converts the supplied icon to a gray scale icon
	{
	// Icon object should exist but not yet be attached
	ASSERT(phGrayIcon != NULL);
	ASSERT(AfxIsValidAddress(phGrayIcon, sizeof(HICON*)));
	ASSERT(*phGrayIcon == NULL);

	// Get the icon  resource data
	HINSTANCE hInstance = NULL;
	HRSRC hResourceInfoBlock = NULL;
	HGLOBAL hGroupIconGlobal = NULL;
	HGLOBAL hIconGlobal = NULL;
	HGLOBAL hIconCopy = NULL;
	DWORD nResourceSize = 0;
	LPGRPICONDIR  pGroupIconDir = NULL;
	LPICONIMAGE pIconImage = NULL;
	LPBITMAPINFOHEADER  pBitmapInfoHeader = NULL;
	WORD nIconID = 0;

	// First get the icon directory
	hInstance = AfxFindResourceHandle(lpszIconResource, RT_GROUP_ICON);
	hResourceInfoBlock = ::FindResource(hInstance, lpszIconResource, RT_GROUP_ICON);
	hGroupIconGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if (hGroupIconGlobal == NULL)
		{
		TRACE0("COXBitmapButton::BuildGrayIcon : Failed to load icon group resource, failing\n");
		return FALSE;
		}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(GRPICONDIR) <= nResourceSize);

	pGroupIconDir = (LPGRPICONDIR)::LockResource(hGroupIconGlobal);
	ASSERT(pGroupIconDir != NULL);
	ASSERT(pGroupIconDir->idType == 1);
	ASSERT(0 < pGroupIconDir->idCount);

	nIconID = pGroupIconDir->idEntries[0].nID;
	if ((::GlobalUnlock(hGroupIconGlobal) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hGroupIconGlobal) == NULL);

	// Then get the icon itself
	hResourceInfoBlock = ::FindResource(hInstance, MAKEINTRESOURCE(nIconID), RT_ICON);
	hIconGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if (hIconGlobal == NULL)
		{
		TRACE0("COXBitmapButton::BuildGrayIcon : Failed to load icon resource, failing\n");
		return FALSE;
		}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(ICONIMAGE) <= nResourceSize);

	// Memory may be read only, make a copy 
	void* pOldBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hIconGlobal);
	ASSERT(pOldBitmapInfoHeader != NULL);
	hIconCopy = ::GlobalAlloc(GMEM_MOVEABLE, nResourceSize);
	pBitmapInfoHeader = (LPBITMAPINFOHEADER)::GlobalLock(hIconCopy);
	ASSERT(pBitmapInfoHeader != NULL);
	::CopyMemory(pBitmapInfoHeader, pOldBitmapInfoHeader, nResourceSize);
	pIconImage = (LPICONIMAGE)pBitmapInfoHeader;
	
	// Change the bitmap color table to gray scale
	BOOL bGraySuccess = MakeGray(pBitmapInfoHeader);

	// Build the icon
	HBITMAP hBitmap = NULL;
	if (bGraySuccess)
		*phGrayIcon = ::CreateIconFromResource((PBYTE)pIconImage, nResourceSize, TRUE, 	0x00030000);
	BOOL bIconSuccess = (*phGrayIcon != NULL);
#ifdef _DEBUG
	if (!bIconSuccess)
		TRACE0("COXBitmapButton::BuildGrayBitmap : Failed to create the icon\n");
#endif // _DEBUG
	
	// Clean up
	::UnlockResource(hIconGlobal);
	if ((::GlobalUnlock(hIconCopy) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hIconCopy) == NULL);

	return bGraySuccess && bIconSuccess;
	}

BOOL COXBitmapButton::MakeGray(LPBITMAPINFOHEADER pBitmapInfoHeader, COLORREF crMask /* = CLR_NONE */)
	// --- In  : pBitmapInfoHeader : The bitmap data to convert
	//			 crMask : The color to skip during conversion
	// --- Out : 
	// --- Returns : Whether the gray bitmap could be created
	// --- Effect : Chenges to color entries of the specified bitmap to gray scale
	{
	// See also 'Retrieving Palette Information from a Bitmap Resource'
	// MSDN PSS ID Number: Q124947

	ASSERT(pBitmapInfoHeader != NULL);
	ASSERT(AfxIsValidAddress(pBitmapInfoHeader, sizeof(BITMAPINFOHEADER)));

	// Check palette existance
	int nNumColors = 0;
	if (pBitmapInfoHeader->biBitCount <= 8)
		nNumColors = (1 << pBitmapInfoHeader->biBitCount);
	else
		// ... No palette used for 24 BPP DIB
		nNumColors = 0;  
	if (nNumColors == 0)
		{
		TRACE0("COXBitmapButton::MakeGray : Resource does not have a palette, failing\n");
		return FALSE;
		}

	// Change the colors to gray scale
	int nColor;
	int nWhite;
	LPBITMAPINFO pBitmapInfo = (LPBITMAPINFO)pBitmapInfoHeader;
	ASSERT(AfxIsValidAddress(pBitmapInfo, sizeof(BITMAPINFO)));
	COLORREF orgColor;
	for (nColor = 0;  nColor < nNumColors;  nColor++)
			{
			// ... This formula will compute the 'white factor' of a color
			//     It translates an RGB value to a gray scale value
			orgColor = RGB(pBitmapInfo->bmiColors[nColor].rgbRed, 
				pBitmapInfo->bmiColors[nColor].rgbGreen, 
				pBitmapInfo->bmiColors[nColor].rgbBlue);

			nWhite = (pBitmapInfo->bmiColors[nColor].rgbRed *    77 +
					  pBitmapInfo->bmiColors[nColor].rgbGreen * 153 +
					  pBitmapInfo->bmiColors[nColor].rgbBlue *   25 ) / 255;

			// Round the value to 0, 128, 192 or 255 (gray scale colors which are always available)
			if (nWhite < 64)
				nWhite = 0;			// ... [0, 63]
			else if (nWhite < 160)
				nWhite = 128;		// ... [64, 159]
			else if (nWhite < 224)
				nWhite = 192;		// ... [160, 223]
			else 
				nWhite = 255;		// ... [224, 255]
			
			if (orgColor != crMask)
				{
				// Change the color entries in the bitmap
				pBitmapInfo->bmiColors[nColor].rgbRed	= (BYTE)nWhite;
				pBitmapInfo->bmiColors[nColor].rgbGreen = (BYTE)nWhite;
				pBitmapInfo->bmiColors[nColor].rgbBlue	= (BYTE)nWhite;
				}
			}

	return TRUE;
	}

BOOL COXBitmapButton::BuildDisabledImage(HICON hSourceIcon, CSize imageSize, HICON& hDestIcon)
	// --- In  : hSourceIcon : Icon conating the image
	//			 imageSize : Size of the image
	// --- Out : hDestIcon : An icon containing the disabled image
	// --- Returns : Whether the disabled icon could be created
	// --- Effect : Creates an icon that contains the disabled look of the supplied icon
	{
	CBitmap helperBitmap;

	CClientDC dc(NULL);
	// Create a mem DC with a new bitmap in it
	CDC memDC;
	VERIFY(memDC.CreateCompatibleDC(&dc));
	VERIFY(helperBitmap.CreateCompatibleBitmap(&dc, imageSize.cx, imageSize.cy));
	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&helperBitmap);
	// ... Default background is the button color
	memDC.FillSolidRect(CRect(CPoint(0,0), imageSize), m_defaultButtonColor);

	// Draw the source icon on the mem DC in a disabled state
	BOOL bSuccess = memDC.DrawState(CPoint(0, 0), imageSize, hSourceIcon, 
		DSS_DISABLED, (HBRUSH)NULL);
	memDC.SelectObject(pOldBitmap);


	// Convert the disabled bitmap to an icon by using the same mask as the source icon
	ICONINFO iconInfo;
	::ZeroMemory(&iconInfo, sizeof(iconInfo));
	// ... ::GetIconInfo craetes two new bitmaps (color and mask)
	BOOL bSuccess2 = ::GetIconInfo(hSourceIcon, &iconInfo);
	// ... Delete the color bitmap
	::DeleteObject(iconInfo.hbmColor);
	iconInfo.hbmColor = NULL;
	iconInfo.hbmColor = helperBitmap;
	hDestIcon = ::CreateIconIndirect(&iconInfo);
	// ... Delete the mask bitmap
	::DeleteObject(iconInfo.hbmMask);
	iconInfo.hbmMask = NULL;

	return bSuccess;
	}

BOOL COXBitmapButton::GetBitmapPalette(LPCTSTR lpszBitmapResource, CPalette& palette)
	// --- In  : lpszBitmapResource : The bitmap resource to use
	// --- Out : palette : The palette of the bitmap
	// --- Returns : Whether it was successful or not
	// --- Effect : Extracts the palette of a bitmap resource
	{
	// Get the bitmap resource data
	HINSTANCE hInstance = NULL;
	HRSRC hResourceInfoBlock = NULL;
	HGLOBAL hBitmapGlobal = NULL;
	HGLOBAL hBitmapCopy = NULL;
	DWORD nResourceSize = 0;
	LPBITMAPINFOHEADER  pBitmapInfoHeader = NULL;

	hInstance = AfxFindResourceHandle(lpszBitmapResource, RT_BITMAP);
	hResourceInfoBlock = ::FindResource(hInstance, lpszBitmapResource, RT_BITMAP);
	hBitmapGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if (hBitmapGlobal == NULL)
		{
		TRACE0("COXBitmapButton::GetBitmapPalette : Failed to load bitmap resource, failing\n");
		return FALSE;
		}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(BITMAPINFOHEADER) <= nResourceSize);

	// Memory may be read only, make a copy 
	void* pOldBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hBitmapGlobal);
	hBitmapCopy = ::GlobalAlloc(GMEM_MOVEABLE, nResourceSize);
	pBitmapInfoHeader = (LPBITMAPINFOHEADER)::GlobalLock(hBitmapCopy);
	ASSERT(pBitmapInfoHeader != NULL);
	::CopyMemory(pBitmapInfoHeader, pOldBitmapInfoHeader, nResourceSize);
	
	// Change the bitmap color table to gray scale
	BOOL bPaletteSuccess = GetImagePalette(pBitmapInfoHeader, palette);

	// Clean up
	::UnlockResource(hBitmapGlobal);
	if ((::GlobalUnlock(hBitmapCopy) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hBitmapCopy) == NULL);

	return bPaletteSuccess;
	}

BOOL COXBitmapButton::GetIconPalette(LPCTSTR lpszIconResource, CPalette& palette)
	// --- In  : lpszIconResource : The icon resource to use
	// --- Out : palette : The palette of the icon
	// --- Returns : Whether it was successful or not
	// --- Effect : Extracts the palette of an icon resource
	{
	// Get the icon  resource data
	HINSTANCE hInstance = NULL;
	HRSRC hResourceInfoBlock = NULL;
	HGLOBAL hGroupIconGlobal = NULL;
	HGLOBAL hIconGlobal = NULL;
	HGLOBAL hIconCopy = NULL;
	DWORD nResourceSize = 0;
	LPGRPICONDIR  pGroupIconDir = NULL;
	LPICONIMAGE pIconImage = NULL;
	LPBITMAPINFOHEADER  pBitmapInfoHeader = NULL;
	WORD nIconID = 0;

	// First get the icon directory
	hInstance = AfxFindResourceHandle(lpszIconResource, RT_GROUP_ICON);
	hResourceInfoBlock = ::FindResource(hInstance, lpszIconResource, RT_GROUP_ICON);
	hGroupIconGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if (hGroupIconGlobal == NULL)
		{
		TRACE0("COXBitmapButton::GetIconPalette : Failed to load icon group resource, failing\n");
		return FALSE;
		}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(GRPICONDIR) <= nResourceSize);

	pGroupIconDir = (LPGRPICONDIR)::LockResource(hGroupIconGlobal);
	ASSERT(pGroupIconDir != NULL);
	ASSERT(pGroupIconDir->idType == 1);
	ASSERT(0 < pGroupIconDir->idCount);

	nIconID = pGroupIconDir->idEntries[0].nID;
	if ((::GlobalUnlock(hGroupIconGlobal) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hGroupIconGlobal) == NULL);

	// Then get the icon itself
	hResourceInfoBlock = ::FindResource(hInstance, MAKEINTRESOURCE(nIconID), RT_ICON);
	hIconGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if (hIconGlobal == NULL)
		{
		TRACE0("COXBitmapButton::GetIconPalette : Failed to load icon resource, failing\n");
		return FALSE;
		}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(ICONIMAGE) <= nResourceSize);

	// Memory may be read only, make a copy 
	void* pOldBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hIconGlobal);
	ASSERT(pOldBitmapInfoHeader != NULL);
	hIconCopy = ::GlobalAlloc(GMEM_MOVEABLE, nResourceSize);
	pBitmapInfoHeader = (LPBITMAPINFOHEADER)::GlobalLock(hIconCopy);
	ASSERT(pBitmapInfoHeader != NULL);
	::CopyMemory(pBitmapInfoHeader, pOldBitmapInfoHeader, nResourceSize);
	pIconImage = (LPICONIMAGE)pBitmapInfoHeader;
	
	// Change the bitmap color table to gray scale
	BOOL bPaletteSuccess = GetImagePalette(pBitmapInfoHeader, palette);

	// Clean up
	::UnlockResource(hIconGlobal);
	if ((::GlobalUnlock(hIconCopy) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hIconCopy) == NULL);

	return bPaletteSuccess;
	}

BOOL COXBitmapButton::GetImagePalette(LPBITMAPINFOHEADER pBitmapInfoHeader, CPalette& palette)
	// --- In  : pBitmapInfoHeader : The image data
	// --- Out : palette : The palette of the image
	// --- Returns : Whether it was successful or not
	// --- Effect : Extracts the palette of an image
	{
	// ... Initialize output parameter
	if (palette.m_hObject != NULL)
		palette.DeleteObject();

	// Check the palette existance
	int nNumColors = 0;
	if (pBitmapInfoHeader->biBitCount <= 8)
		nNumColors = (1 << pBitmapInfoHeader->biBitCount);
	else
		// ... No palette used for 24 BPP DIB
		nNumColors = 0;  
	if (nNumColors == 0)
		{
		TRACE0("COXBitmapButton::GetImagePalette : Resource does not have a palette, failing\n");
		return FALSE;
		}

	// Create the logical palette	
	BYTE* pBuffer = NULL;
	LPLOGPALETTE pLogPalette = NULL;
	const int nBufferSize = sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nNumColors;
	pBuffer = new BYTE[nBufferSize];
	::ZeroMemory(pBuffer, nBufferSize);
	pLogPalette = (LPLOGPALETTE)pBuffer;

	// Initialize the palette
	pLogPalette->palVersion = 0x300;
	pLogPalette->palNumEntries = (unsigned short)nNumColors;

	// Get the palette colors 
	LPBITMAPINFO pBitmapInfo = (LPBITMAPINFO)pBitmapInfoHeader;
	ASSERT(AfxIsValidAddress(pBitmapInfo, sizeof(BITMAPINFO)));
	int nColor;
	for (nColor = 0;  nColor < nNumColors;  nColor++)
		{
		pLogPalette->palPalEntry[nColor].peRed   = pBitmapInfo->bmiColors[nColor].rgbRed;
		pLogPalette->palPalEntry[nColor].peGreen = pBitmapInfo->bmiColors[nColor].rgbGreen;
		pLogPalette->palPalEntry[nColor].peBlue  = pBitmapInfo->bmiColors[nColor].rgbBlue;
		pLogPalette->palPalEntry[nColor].peFlags = 0;
		}

	// Build the palette from the logical palette
	BOOL bPaletteSuccess = palette.CreatePalette(pLogPalette);

#ifdef _DEBUG
	if (!bPaletteSuccess)
		TRACE0("COXBitmapButton::GetImagePalette : Failed to create the palette\n");
#endif // _DEBUG

	// Clean up
	delete[] pBuffer;

	return TRUE;
	}

//########################
static CWnd* FindActiveParent( CWnd* pW )
	{
   CWnd *pww = CWnd::GetActiveWindow();
			if( !pww ) return NULL;

	  CWnd *p = pW;			
			HWND hw = pww->m_hWnd; 
			while( p )
				{
      if( p->m_hWnd == hw ) return p;
						p = p->GetParent();
				}

			return NULL;
	}

void COXBitmapButton::CheckTrackLook(CPoint point)
	// --- In  : point : The current position of the mouse in screen coordinates
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets the correct look if in track look mode
	{
	if (!m_bTrackLook)
		// Only do something when we are in track look mode
		return;

	// Check whether the mouse is over the button now
	CRect rcWindow;
	GetWindowRect(rcWindow);
	BOOL bNewMouseOverButton = rcWindow.PtInRect(point);

	if (bNewMouseOverButton && IsWindowEnabled())
		{
		// Moved inside this control window
		// ... Do capture the mouse input while the mouse is still down
		//     (may have triggered an action, e.g. show msgbox)
		if (!m_bMouseDown)
			SetCapture();
		// ... When we changed state invalidate the window and get the focus
		//	   so that pressing a key will have the right effect
		if (!m_bMouseOverButton)
			{//####################
			  CWnd *pPar_ = GetParent();
					if( pPar_ && FindActiveParent(pPar_) )						
							 SetFocus();								  
						
					Invalidate();
			}
		m_bMouseOverButton = TRUE;
		}
	else 
		{
		// ... We release the mouse capture (may have already lost it)
		if (!m_bMouseDown)
			ReleaseCapture();
		// ... When we changed state invalidate the window
		if (m_bMouseOverButton)
			Invalidate();
		m_bMouseOverButton = FALSE;
		}
	}

void COXBitmapButton::PostCheckTrackLook() 
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Checks the current state of the track look
	//				Should be called by function which might release the mouse capture
	{
	if (m_bTrackLook && m_bMouseOverButton && (GetCapture() != this))
		{
		// We have lost the capture
		m_bMouseOverButton = FALSE;
		Invalidate();
		// Recheck the mouse position as soon as possible
		PostMessage(WM_CHECK_TRACK_LOOK);
		}
	}

CString COXBitmapButton::GetSubString(LPCTSTR pszFullString, int nSubIndex, 
	TCHAR cDelimiter)
	// --- In  : pszFullString : The full string
	//			 nSubIndex : The ONE-based index of the substring requested
	//			 cDelimiter : Delimiter character used between all substrings
	// --- Out : 
	// --- Returns : The requested substring or an empty string otherwise
	// --- Effect : 
	{
	ASSERT(0 < nSubIndex);

	CString sSubString;
	if (pszFullString == NULL)
		{
		// Nothing to search : nothing to find
		ASSERT(sSubString.IsEmpty());
		return sSubString;
		}

	// Set pszStart to first charecter and pszEnd after last charecter
	LPCTSTR pszBegin = pszFullString;
	LPCTSTR pszEnd = pszFullString + _tcslen(pszFullString);
	LPCTSTR pszDelimiter = _tcschr(pszBegin, cDelimiter); 
	if (pszDelimiter == NULL)
		pszDelimiter = pszEnd;

	ASSERT((*pszDelimiter == cDelimiter) || (*pszDelimiter == _T('\0')));
	ASSERT(pszBegin <= pszDelimiter);

	while (--nSubIndex != 0)
		{
		if (*pszDelimiter == _T('\0'))
			{
			// Search to end of string and not found
			ASSERT(sSubString.IsEmpty());
			return sSubString;
			}
		pszBegin = pszDelimiter + 1;
		pszDelimiter = _tcschr(pszBegin, cDelimiter); 
		if (pszDelimiter == NULL)
			pszDelimiter = pszEnd;
		ASSERT((*pszDelimiter == cDelimiter) || (*pszDelimiter == _T('\0')));
		ASSERT(pszBegin <= pszDelimiter);
		}

	int nLen = pszDelimiter - pszBegin;
	ASSERT(0 <= nLen);
	LPTSTR pszSubString = sSubString.GetBufferSetLength(nLen);
	::CopyMemory(pszSubString, pszBegin, nLen * sizeof(TCHAR));
	return sSubString;
	}


CString COXBitmapButton::RemoveAmpersand(LPCTSTR pszText)
	// --- In  : pszText : The input string 
	// --- Out : 
	// --- Returns : The input string but with all single ampersands removed and
	//               all double ampersands replaced by a single
	// --- Effect : 
	{
	if (pszText == NULL)
		return _T("");

	CString sResult;
	BOOL bPreviousWasAmpersand = FALSE;
	BOOL bThisIsAmpersand = FALSE;
	LPTSTR pszResult = sResult.GetBuffer(_tcslen(pszText));
	while (*pszText != _T('\0'))
		{
		bThisIsAmpersand = (*pszText == _T('&'));
		if (bPreviousWasAmpersand || !bThisIsAmpersand)
			{
			// ... Prev was ampersand or this one is not (or both), copy
			*(pszResult++) = *pszText;
			}
		bPreviousWasAmpersand = bThisIsAmpersand && !bPreviousWasAmpersand;
		pszText++;
		}
	*pszResult = _T('\0');
	sResult.ReleaseBuffer();
	return sResult;
	}

BOOL COXBitmapButton::LoadBitmap(LPCTSTR lpszBitmapResource, CBitmap& bitmap, CPalette& palette)
	// --- In  : lpszBitmapResource : The bitmap resource
	// --- Out : bitmap : The bitmap loaded (if successful)
	//			 palette : The bitmap loaded (if successful)
	// --- Returns : Whether the function could load the bitmap successsfully
	// --- Effect : Loads a bitmap and its palette from resource
	//				This function also works for 256-color bitmaps
	//				(which ::LoadBitmpa does not)
	// See also "How to Use a DIB Stored as a Windows Resource"
	// Article ID: Q67883 :
	//  When the display is a 256-color 8514 unit, the same action [LoadBitmap()] 
	//  will map the 256 bitmap colors into the 20 reserved system colors, 
	//  and an 8 bits-per-pixel bitmap will be returned. 
	{
	// ... Assume failure
	BOOL bPaletteSuccess = FALSE;
	BOOL bBitmapSuccess = FALSE;

	// ... First delete a possible previous bitmap contents
	bitmap.DeleteObject();

	// ... Without deleting the palette, reloading the bitmap would fail
	// Thanks to Ali
	palette.DeleteObject();

	// Load bitmap
	// ... Bitmap and palette object should not yet be attached
	ASSERT(bitmap.m_hObject == NULL);
	ASSERT(palette.m_hObject == NULL);

	// ... Get the bitmap data
	HINSTANCE hInstance = NULL;
	HRSRC hResourceInfoBlock = NULL;
	HGLOBAL hBitmapGlobal = NULL;
	HGLOBAL hBitmapCopy = NULL;
	DWORD nResourceSize = 0;
	LPBITMAPINFOHEADER  pBitmapInfoHeader = NULL;
	LPBITMAPINFOHEADER  pCopyBitmapInfoHeader = NULL;

	hInstance = AfxFindResourceHandle(lpszBitmapResource, RT_BITMAP);
	hResourceInfoBlock = ::FindResource(hInstance, lpszBitmapResource, RT_BITMAP);
	hBitmapGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if (hBitmapGlobal == NULL)
		{
		TRACE0("COXBitmapButton::LoadBitmap : Failed to load bitmap resource, failing\n");
		return FALSE;
		}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(BITMAPINFOHEADER) <= nResourceSize);

	// Memory may be read only, make a copy 
	pBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hBitmapGlobal);
	hBitmapCopy = ::GlobalAlloc(GMEM_MOVEABLE, nResourceSize);
	pCopyBitmapInfoHeader = (LPBITMAPINFOHEADER)::GlobalLock(hBitmapCopy);
	ASSERT(pCopyBitmapInfoHeader != NULL);
	::CopyMemory(pCopyBitmapInfoHeader, pBitmapInfoHeader, nResourceSize);

	// Get the bitmap color table 
	bPaletteSuccess = GetImagePalette(pCopyBitmapInfoHeader, palette);
	if (!bPaletteSuccess)
		TRACE0("COXBitmapButton::LoadBitmap : Failed to load palette, continuing\n");

	// Get the bitmap bits itself
	DWORD nPaletteByteSize = 0;
	LPVOID pBitmapBits = NULL;
	if (pCopyBitmapInfoHeader->biSize == sizeof(BITMAPINFOHEADER))
		{
		if (pCopyBitmapInfoHeader->biBitCount <= 8)
			nPaletteByteSize = (1 << pCopyBitmapInfoHeader->biBitCount) * sizeof(RGBQUAD);
		}
   	else
		{
		if (((LPBITMAPCOREHEADER)pCopyBitmapInfoHeader)->bcBitCount <= 8)
			nPaletteByteSize = (1 << ((LPBITMAPCOREHEADER)pCopyBitmapInfoHeader)->bcBitCount) * sizeof(RGBTRIPLE);
		}
	pBitmapBits = (LPBYTE)pCopyBitmapInfoHeader + pCopyBitmapInfoHeader->biSize + nPaletteByteSize;
		{
		CWindowDC dc(NULL);
		CPalette* pOldPalette = NULL;
		if (bPaletteSuccess)
			{
			dc.SelectPalette(&palette, FALSE);
			dc.RealizePalette();
			}
		HBITMAP hBitmap = ::CreateDIBitmap (dc.GetSafeHdc(), pCopyBitmapInfoHeader,
			CBM_INIT, pBitmapBits, (LPBITMAPINFO)pCopyBitmapInfoHeader, DIB_RGB_COLORS);
		bBitmapSuccess = (hBitmap != NULL);
		if (bBitmapSuccess)
			bitmap.Attach(hBitmap);
		else
			TRACE0("COXBitmapButton::LoadBitmap : Failed to load bitmap, failing\n");

		if (pOldPalette != NULL)
			dc.SelectPalette (pOldPalette, FALSE);
		}

	::UnlockResource(hBitmapGlobal);
	if ((::GlobalUnlock(hBitmapCopy) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hBitmapCopy) == NULL);

	// ... Return whether successful or not
	return bBitmapSuccess;
	}

// private:

// ==========================================================================

void COXBitmapButton::OnSysColorChange() 
	{
	CButton::OnSysColorChange();
	
	// Get the new button color
	m_defaultButtonColor = ::GetSysColor(COLOR_BTNFACE);

	// Redraw the button
	Invalidate();
	}

void COXBitmapButton::OnMouseMove(UINT nFlags, CPoint point) 
	{
	if (!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	// Check whether the mouse is over the button now
	ClientToScreen(&point);
	CheckTrackLook(point);

	CButton::OnMouseMove(nFlags, point);
	}

void COXBitmapButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
	{
	if (!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;
	
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
	}

void COXBitmapButton::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
	{
	if (!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	CButton::OnKeyUp(nChar, nRepCnt, nFlags);
	// The previous function may have released the mouse capture,
	// so recheck the track look
	PostCheckTrackLook();
	}

void COXBitmapButton::OnKillFocus(CWnd* pNewWnd) 
	{
	if (!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	CButton::OnKillFocus(pNewWnd);
	// The previous function may have released the mouse capture,
	// so recheck the track look
	PostCheckTrackLook();
	}

void COXBitmapButton::OnMButtonUp(UINT nFlags, CPoint point) 
	{
	if (!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	CButton::OnMButtonUp(nFlags, point);
	// The previous function may have released the mouse capture,
	// so recheck the track look
	PostCheckTrackLook();
	}

void COXBitmapButton::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
	{
	if (!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;
	
	CButton::OnSysKeyDown(nChar, nRepCnt, nFlags);
	}

void COXBitmapButton::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
	{
	if (!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	CButton::OnSysKeyUp(nChar, nRepCnt, nFlags);
	// The previous function may have released the mouse capture,
	// so recheck the track look
	PostCheckTrackLook();
	}

void COXBitmapButton::OnLButtonDown(UINT nFlags, CPoint point) 
	{
	if (!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	m_bMouseDown = TRUE;
	CButton::OnLButtonDown(nFlags, point);
	// The previous function may have released the mouse capture,
	// so recheck the track look
	PostCheckTrackLook();
	}

void COXBitmapButton::OnLButtonUp(UINT nFlags, CPoint point) 
	{
	if (!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	CButton::OnLButtonUp(nFlags, point);
	m_bMouseDown = FALSE;
	// The previous function may have released the mouse capture,
	// so recheck the track look
	PostCheckTrackLook();
	}


LRESULT COXBitmapButton::OnSetText(WPARAM wParam, LPARAM lParam)
	{
	LRESULT result = DefWindowProc(WM_SETTEXT, wParam, lParam);

	// Adjust a possible tool tip control's text
	if (m_toolTip.m_hWnd != 0)
		{
		// Get everything after the first EndOfLine char and before te second
		CString sToolTipText = GetSubString((LPCTSTR)lParam, 2, _T('\n'));
		if (!sToolTipText.IsEmpty())
			VERIFY(m_toolTip.AddTool(this, sToolTipText));
		else
			m_toolTip.DelTool(this);
		}

	return result;
	}

BOOL COXBitmapButton::PreTranslateMessage(MSG* pMsg) 
	{
	// ... Pass the message on to the tool tip
	if (m_toolTip.m_hWnd != NULL)
		m_toolTip.RelayEvent(pMsg);
 
	return CButton::PreTranslateMessage(pMsg);
	}

LRESULT COXBitmapButton::OnCheckTrackLook(WPARAM wParam, LPARAM lParam)
	{
	ASSERT(wParam == 0);
	ASSERT(lParam == 0);
	UNUSED(wParam);
	UNUSED(lParam);

	// Check whether the mouse is over the button now
	CPoint ptCursor;
	VERIFY(::GetCursorPos(&ptCursor));
	CheckTrackLook(ptCursor);

	return TRUE;
	}


BOOL COXBitmapButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
	{
	// Use our own cursor if we have one
	if (IsWindowEnabled())
		{
		if (m_hDefaultCursor != NULL)
			{
			::SetCursor(m_hDefaultCursor);
			// ... We handled the message
			return TRUE;
			}
		}
	else
		{
		if (m_hDisabledCursor != NULL)
			{
			::SetCursor(m_hDisabledCursor);
			// ... We handled the message
			return TRUE;
			}
		}

	// ... Call base class implementation
	return CButton::OnSetCursor(pWnd, nHitTest, message);
	}

BOOL COXBitmapButton::OnEraseBkgnd(CDC* pDC) 
	{
	if (m_bHyperLook)
		{
		// Do not fill the background when in hyper look mode
		if (!m_bBackgroundGrabbed)
			{
			// Grab the backround on the first OnEraseBackground() call
			m_bBackgroundGrabbed = AdjustBackground();	
			}
		return TRUE;
		}
	else
		return CButton::OnEraseBkgnd(pDC);
	}


void COXBitmapButton::OnSize(UINT nType, int cx, int cy) 
	{
	CButton::OnSize(nType, cx, cy);
	
	// ... Because the button changed size, the background image should be grabbed again
	if (m_bHyperLook)
		RegrabBackground();	
	}

void COXBitmapButton::OnMove(int x, int y) 
	{
	CButton::OnMove(x, y);
	
	// ... Because the button moved, the background image should be grabbed again
	if (m_bHyperLook)
		RegrabBackground();	
	}


void COXBitmapButton::OnEnable(BOOL bEnable) 
	{
	CButton::OnEnable(bEnable);
	
	// The previous function may have changed the enable state of the button
	// so recheck the track look
	PostCheckTrackLook();
	}
