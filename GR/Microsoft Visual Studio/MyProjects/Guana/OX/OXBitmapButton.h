// ==========================================================================
// 					Class Specification : COXBitmapButton
// ==========================================================================

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CButton

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	YES	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class implements an owner drawn button for which you only have to 
//	 provide one bitmap or icon.  The other bitmaps and icons are computed dynamically
//	 (pressed, inactive (grayed), disabled etc).

//	To use it just link your button (BS_OWNERDRAW) with an COXBitmapButton object
//	 and call the function LoadBitmap() to supply a bitmap or icon from resource.

//	A special look ('track look') is provided as well.  This looks like the
//	 buttons in the toolbar of the MS Internet Explorer 3.0.
//	When the mouse is not over the button, the button looks flat and the
//	 image is shown inactively (in gray scale).
//	When the user moves the mouse over the button it shows a normal 3D button
//	 and the image has its original colors.
//  A disabled button, always has a flat, disabled look (even when the mouse is over
//	 the button)

//	A second special look ('hyper look') is provided as well.  
//  In this mode the original image is shown always.  So this look does not have
//	 a button look : just a (transparant) image.
//	By default it uses a cursor in the form of a hand.  
//	It also uses the pseudo disable mode.  This means that the button
//   is not actually disabled, but the user cannot click it.
//	This gives the possibility to use another cursor when the button is
//	 (pseudo) disabled : a hand with a stop sign.

//	You can add text to the image and set its alignment
//	Support for tooltips is also available.  The text can be get and set by using
//	GetText(), SetText(), GetToolTipText(), SetToolTipText()

// Remark:
//	The buttons used with this class must have the BS_OWNERDRAW style.
//	The BS_BITMAP style is not supported,

//	Windows version previous than 4.0 do not support the disabled look.

//  This class has been reworked so that it works with 256-color bitmaps 
//   (not 256-color icons)
//	The images are internally stored as DDB (device dependant bitmaps)
//   together with the palette (of the original resource)
//	The class uses the one palette when drawing itself.
//	To assure nice cooperation with other controls or graphic elements
//	 you should use one palette for all items within a dialog, formview etc.

//	This file uses resources. (E.g. Bitmap IDC_OX_HAND_CURSOR)
//	The reserved ID ranges are : 23460 -> 23479 and 53460 -> 53479

// Prerequisites (necessary conditions):
/////////////////////////////////////////////////////////////////////////////

#ifndef __OXBITMAPBUTTON_H__
#define __OXBITMAPBUTTON_H__

#include "OXBitmapButtonRes.h"

// The maximum number of images we have to build per bitmap button
#define OX_MAX_IMAGE_COUNT 5

#ifndef WM_OX_APP
// You can redefine WM_OX_APP by defining it before including this file
#define WM_OX_APP WM_APP + 100
#endif
#define WM_CHECK_TRACK_LOOK		(WM_OX_APP + 113)

class COXBitmapButton : public CButton
{
DECLARE_DYNAMIC(COXBitmapButton);

friend class TMyDBNav;

// Data members -------------------------------------------------------------
public:

protected:
	CImageList		m_imageList;
	CPalette		m_palette;
	COLORREF		m_textColor;
	CFont			m_textFont;
	CToolTipCtrl	m_toolTip;

	BOOL	m_bTrackLook;
	BOOL	m_bMouseOverButton;
	BOOL	m_bMouseDown;

	BOOL	m_bHyperLook;
	CImageList m_backgroundImage;
	BOOL	m_bBackgroundGrabbed;
	UINT	m_nDefaultCursorID;
	HCURSOR m_hDefaultCursor;
	UINT	m_nDisabledCursorID;
	HCURSOR m_hDisabledCursor;

	BOOL	m_bPseudoDisableMode;
	BOOL	m_bEnabled;
	BOOL	m_bHasTabStop;

public:
	//static CPoint m_ptImageOffset;
	CPoint m_ptImageOffset;
protected:

	static CPoint m_ptTextOffset;
	static CPoint m_ptOuterFocusOffset;
	static CPoint m_ptInnerFocusOffset;
	static CSize  m_hyperFocusSize;
	static CPoint m_ptDownOffset;
	static CPoint m_ptHyperOffset;

	static const int m_nNormalImageIndex;
	static const int m_nInactiveImageIndex;
	static const int m_nDisabledImageIndex;

	static COLORREF m_defaultButtonColor;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXBitmapButton();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	BOOL LoadBitmap(LPCTSTR lpszBitmapResource, BOOL bResize = TRUE, COLORREF crMask = CLR_NONE);
	// --- In  : lpszBitmapResource : The bitmap resource
	//			 bResize : Whether to resize the button around the bitmap
	//			 crMask : Mask color to use for the bitmap
	//					  (This color of the bitmap will be shown transparantly)
	// --- Out : 
	// --- Returns : Whether the loading was successful
	// --- Effect : Loads the bitmap resource

	BOOL LoadBitmap(UINT nIDBitmapResource, BOOL bResize = TRUE, COLORREF crMask = CLR_NONE);
	// --- In  : nIDBitmapResource : The bitmap resource
	//			 bResize : Whether to resize the button around the bitmap
	//			 crMask : Mask color to use for the bitmap
	//					  (This color of the bitmap will be shown transparantly)
	// --- Out : 
	// --- Returns : Whether the loading was successful
	// --- Effect : Loads the bitmap resource

	BOOL LoadIcon(LPCTSTR lpszIconResource, BOOL bResize = TRUE);
	// --- In  : lpszIconResource : The icon  resource
	//			 bResize : Whether to resize the button around the icon
	// --- Out : 
	// --- Returns : Whether the loading was successful
	// --- Effect : Loads the icon resource

	BOOL LoadIcon(UINT nIDIconResource, BOOL bResize = TRUE);
	// --- In  : nIDIconResource : The icon  resource
	//			 bResize : Whether to resize the button around the icon
	// --- Out : 
	// --- Returns : Whether the loading was successful
	// --- Effect : Loads the icon resource

	CPalette* GetPalette();
	// --- In  : 
	// --- Out : 
	// --- Returns : The palette of the bitmap button
	//				 This object will valid after a successful LoadBitmap or LoadIcon
	// --- Effect : 

	virtual void SizeToContent();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Sizes the button according to its contents

	BOOL GetTrackLook() const;
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the track look mode is enabled
	// --- Effect : 

	BOOL SetTrackLook(BOOL bTrackLook = TRUE);
	// --- In  : bTrackLook : Whether enable or disable the track look mode
	// --- Out : 
	// --- Returns : Whether is was successful or not
	// --- Effect : 

	void SetHorizontalAlignment(DWORD nAlignment = BS_CENTER);
	// --- In  : nAlignment : The horizontal alignment of the image
	//						  (BS_LEFT, BS_CENTER or BS_RIGHT)
	// --- Out : 
	// --- Returns : 
	// --- Effect : 

	DWORD GetHorizontalAlignment() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The horizontal alignment of the image (BS_LEFT, BS_CENTER or BS_RIGHT)
	// --- Effect : 

	void SetVerticalAlignment(DWORD nAlignment = BS_VCENTER);
	// --- In  : nAlignment : The vertical alignment of the image
	//						  (BS_TOP, BS_VCENTER or BS_BOTTOM)
	// --- Out : 
	// --- Returns : 
	// --- Effect : 

	DWORD GetVerticalAlignment() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The vertical alignment of the image (BS_TOP, BS_VCENTER or BS_BOTTOM)
	// --- Effect : 

	COLORREF GetTextColor() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The current text color
	// --- Effect : 

	void SetTextColor(COLORREF textColor);
	// --- In  : textColor : The new text color
	// --- Out : 
	// --- Returns : 
	// --- Effect : 

	CFont* GetTextFont();
	// --- In  : 
	// --- Out : 
	// --- Returns : The current text font
	// --- Effect : 

	void SetTextFont(CFont* pTextFont);
	// --- In  : pTextFont : The new text font
	// --- Out : 
	// --- Returns : 
	// --- Effect : A copy of the font will be made and stored internally

	CString GetText() const;
	// --- In  :
	// --- Out : 
	// --- Returns : The text of this button 
	//				 (window text upto but not including the first '\n")
	// --- Effect : 

	void SetText(LPCTSTR pszText);
	// --- In  : pszText : The new button text
	// --- Out : 
	// --- Returns : 
	// --- Effect : Sets the new text of this button 
	//				(window text upto but not including the first '\n")

	CString GetToolTipText() const;
	// --- In  :
	// --- Out : 
	// --- Returns : The tool tip text of this button 
	//				 (window text starting from but not including the first '\n")
	// --- Effect : 

	void SetToolTipText(LPCTSTR pszToolTipText);
	// --- In  : pszToolTipText : The new tool tip text
	// --- Out : 
	// --- Returns : 
	// --- Effect : Sets the new text of the tool tip
	//				(window text starting from but not including the first '\n")

	BOOL GetToolTip() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether tooltip is enabled for this window
	// --- Effect : 

	BOOL SetToolTip(BOOL bEnable = TRUE);
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether the function was successful
	// --- Effect : Enable tooltip for this window

	virtual CSize GetFitButtonSize();
	// --- In  :
	// --- Out : 
	// --- Returns : The size of button in pixels when it fits its entire contents
	// --- Effect : 

	CSize GetButtonSize() const;
	// --- In  :
	// --- Out : 
	// --- Returns : The present size of the button in pixels
	// --- Effect : 

	CSize GetImageSize() const;
	// --- In  :
	// --- Out : 
	// --- Returns : The present size of the image in pixels
	// --- Effect : 

	CSize GetTextSize();
	// --- In  :
	// --- Out : 
	// --- Returns : The size of the text in pixels
	// --- Effect : 

	BOOL GetHyperLook() const;
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the hyper look mode is enabled
	// --- Effect : 

	BOOL SetHyperLook(BOOL bHyperLook = TRUE);
	// --- In  : bHyperLook : Whether enable or disable the hyper look mode
	// --- Out : 
	// --- Returns : Whether is was successful or not
	// --- Effect : 

	BOOL RegrabBackground();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether is was successful or not
	// --- Effect : This functions deletes an old copy of the button background 
	//				and grabs a new one.
	//				This function can be useful when you change the background
	//				during the lifetime of a bitmap button

	BOOL SetDefaultCursor(UINT nCursorID = 0);
	// --- In  : nCursorID : The new cursor ID to use (0 resets to default cursor)
	// --- Out : 
	// --- Returns : Whether the function was successful (the resource was found)
	// --- Effect : Sets the default cursor of this window
	//				Possible values are : IDC_OX_HAND_CURSOR and IDC_OX_NO_HAND_CURSOR

	UINT GetDefaultCursor() const;
	// --- In  :
	// --- Out : 
	// --- Returns : The ID of the current default cursor (0 when none is set)
	// --- Effect : 

	BOOL SetDisabledCursor(UINT nCursorID = 0);
	// --- In  : nCursorID : The new cursor ID to use (0 resets to default cursor)
	// --- Out : 
	// --- Returns : Whether the function was successful (the resource was found)
	// --- Effect : Sets the default cursor of this window
	//				Possible values are : IDC_OX_HAND_CURSOR and IDC_OX_NO_HAND_CURSOR
	//				The pseudo-disable mode must be activated before the
	//				disabled cursor will be shown

	UINT GetDisabledCursor() const;
	// --- In  :
	// --- Out : 
	// --- Returns : The ID of the current default cursor (0 when none is set)
	// --- Effect : 

	BOOL GetPseudoDisableMode() const;
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the pseudo-disable mode is active
	// --- Effect : 

	BOOL SetPseudoDisableMode(BOOL bPseudoDisableMode = TRUE);
	// --- In  : bPseudoDisableMode : Whether use pseudo-disable mode or not
	// --- Out : 
	// --- Returns : Whether is was successful or not
	// --- Effect : In pseudo disable mode the control is visually disabled
	//				but not really disabled according to Windows.
	//				This provides the opportunity to receive messages
	//				even when the window is (psudo)disabled.
	//				This mode must be set for the disabled cursor to work.

	BOOL IsWindowEnabled() const;
	// --- In  :
	// --- Out : 
	// --- Returns : Whether this window is enabled or not
	// --- Effect : This function also works when in pseudo-disable mode 

	BOOL EnableWindow(BOOL bEnable = TRUE);
	// --- In  : bEnable : Whether to enable (TRUE) or disable (FALSE) this window
	// --- Out : 
	// --- Returns : The old state before the requested state was set
	// --- Effect : This function also works when in pseudo-disable mode 

	BOOL AdjustBackground();
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the background was grabbed again
	// --- Effect : This functions grabs the image that is currently painted
	//              on the rect used by the bitmap button
	//				When this function is called during OnEraseBkgnd() it will
	//				grab the background of the button

#ifdef _DEBUG
	virtual void AssertValid() const;
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : AssertValid performs a validity check on this object 
	//				by checking its internal state. 
	//				In the Debug version of the library, AssertValid may assert and 
	//				thus terminate the program.
	
	virtual void Dump(CDumpContext& dc) const;
	// --- In  : dc : The diagnostic dump context for dumping, usually afxDump.
	// --- Out : 
	// --- Returns :
	// --- Effect : Dumps the contents of the object to a CDumpContext object. 
	//				It provides diagnostic services for yourself and 
	//				 other users of your class. 
	//				Note  The Dump function does not print a newline character
	//				 at the end of its output. 
#endif

	virtual ~COXBitmapButton();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXBitmapButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	void DistributeSpace(UINT nState, CRect itemRect, CRect& buttonRect, CRect& imageRect, CRect& textRect);
	virtual void SelectPalette(CDC* pDC, UINT nState, CRect buttonRect, CPalette*& pOldPalette);
	virtual void DrawButton(CDC* pDC, UINT nState, CRect buttonRect);
	virtual void DrawImage(CDC* pDC, UINT nState, CImageList* pImageList, CRect imageRect);
	virtual void DrawText(CDC* pDC, UINT nState, CString sText, CRect textRect);
	virtual void DrawFocusRectangle(CDC* pDC, UINT nState, CRect buttonRect, CRect bitmapRect);
	virtual void RestorePalette(CDC* pDC, UINT nState, CRect buttonRect, CPalette* pOldPalette);

	BOOL BuildGrayBitmap(LPCTSTR lpszBitmapResource, COLORREF crMask, CBitmap* pGrayBitmap);
	BOOL BuildGrayIcon(LPCTSTR lpszIconResource, HICON* phIcon);
	BOOL MakeGray(LPBITMAPINFOHEADER pBitmapInfoHeader, COLORREF crMask = CLR_NONE);
	BOOL BuildDisabledImage(HICON hSourceIcon, CSize imageSize, HICON& hDestIcon);

	static BOOL GetBitmapPalette(LPCTSTR lpszBitmapResource, CPalette& palette);
	static BOOL GetIconPalette(LPCTSTR lpszIconResource, CPalette& palette);
	static BOOL GetImagePalette(LPBITMAPINFOHEADER pBitmapInfoHeader, CPalette& palette);

	void CheckTrackLook(CPoint point);
	void PostCheckTrackLook();

	static CString GetSubString(LPCTSTR pszFullString, int nSubIndex, TCHAR cDelimiter);
	static CString RemoveAmpersand(LPCTSTR pszText);
	static BOOL LoadBitmap(LPCTSTR lpszBitmapResource, CBitmap& bitmap, CPalette& palette);

	//{{AFX_MSG(COXBitmapButton)
	afx_msg void OnSysColorChange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG

	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCheckTrackLook(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
                   
};

#endif // __OXBITMAPBUTTON_H__
// ==========================================================================
