#if !defined(_MY_EDIT_)
#define _MY_EDIT_


class TMyEdit: public CEdit
	{
public:
	 TMyEdit(): CEdit()
			{
			}

		void OnSetFocus( CWnd* pOldWnd );

		DECLARE_MESSAGE_MAP()
	};


class TMyDateTime: public CDateTimeCtrl
	{
public:
	 TMyDateTime(): CDateTimeCtrl()
			{
			}

		void OnSetFocus( CWnd* pOldWnd );

		DECLARE_MESSAGE_MAP()
	};


#endif

