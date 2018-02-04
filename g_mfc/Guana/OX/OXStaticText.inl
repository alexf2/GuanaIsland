// ==========================================================================
// 					Inline functions : COXStaticText
// ==========================================================================
// inline file :		OXStaticText.inl

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved                      
                          
// //////////////////////////////////////////////////////////////////////////

inline COLORREF COXStaticText::GetBkColor() const
	{
	return m_dwBkColor;
	}

inline COLORREF COXStaticText::GetTextColor() const
	{
	return m_dwTextColor;
	}

inline BOOL COXStaticText::GetEmboss() const
	{
	return m_bEmbossText;
	}

inline LPCTSTR COXStaticText::GetText() const
	{
	return (LPCTSTR)m_sText;
	}

inline int COXStaticText::GetGraphicsMode() const
	{
	return m_nGraphicsMode;
	}

inline int COXStaticText::GetEllipseMode() const
	{
	return  m_nEllipseMode;
	}

inline DWORD COXStaticText::Get3Doffset() const
	{
	return m_dwOffset;
	}

inline BOOL COXStaticText::IsItalic() const
	{
	return m_LogFont.lfItalic;
	}

inline BOOL COXStaticText::IsUnderlined() const
	{
	return m_LogFont.lfUnderline;
	}

inline BOOL COXStaticText::IsStrikedOut() const
	{
	return m_LogFont.lfStrikeOut;
	}

inline int COXStaticText::GetStringAngle() const
	{
	return m_LogFont.lfEscapement;
	}

inline int COXStaticText::GetCharAngle() const
	{
	return m_LogFont.lfOrientation;
	}

inline int COXStaticText::GetCharSet() const
	{
	return m_LogFont.lfCharSet;
	}

inline int COXStaticText::GetFontHeight() const
	{
	return m_LogFont.lfHeight;
	}

inline int COXStaticText::GetFontWidth() const
	{
	return m_LogFont.lfWidth;
	}

inline CString COXStaticText::GetFontName() const
	{
	return m_LogFont.lfFaceName;
	}

inline int COXStaticText::GetScrollDirection() const
	{
	return m_nScrollDirection;
	}

inline DWORD COXStaticText::GetScrollSpeed() const
	{
	return m_dwScrollSpeed;
	}

inline DWORD COXStaticText::GetMinTimeOut() const
	{
	return m_dwMinTimeOut;
	}

// end of OXStaticText.inl