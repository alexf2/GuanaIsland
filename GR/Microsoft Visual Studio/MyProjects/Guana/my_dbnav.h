#if !defined(_MY_DBNAV_)
#define _MY_DBNAV_

struct TCreBmpDta
	{
	  int id;
			LPCTSTR lp;
	};


/*class MyCOXBtn: public COXBitmapButton
	{
public:
	MyCOXBtn(): COXBitmapButton()
		{
		}
	virtual ~MyCOXBtn()
		{
		}

	virtual CSize GetFitButtonSize()
		{
    return GetImageSize();
		}
	};*/

class TMyDBNav: public CStatic
	{
public:
	 TMyDBNav(): CStatic(), m_vecBtn( 10 )
			{
			  m_pWGrid = NULL; m_pWAdc = NULL;
			}

		virtual ~TMyDBNav()
			{
			}

		void Assign( CWnd* pWGrid, CAdodc* pWAdc );
		void TestState( CWnd* );
		void GridEdit( CWnd*, bool );

		CDataGrid* GetGridFor( CAdodc* pWAdc );

		CPoint GetBtnSize();
		CPoint GetRecSize();

protected:
		typedef auto_ptr<COXBitmapButton> TAP_COXBtn;
		vector<TAP_COXBtn> m_vecBtn;
		map<int, COXBitmapButton*> m_map;

#define IDC_NAVBTN_STA 101
#define BMPNAV_WIDTH 21
#define BMPNAV_HEIGHT 19
		//0 - begin        101
		//1 - priv         102
		//2 - next         103
		//3 - end          104
		//4 - add          105
		//5 - remov        106
		//6 - edit         107
		//7 - update       108
		//8 - cancel       109
		//9 - refresh      110

#define IDC_NAVBTN_BEGIN     101
#define IDC_NAVBTN_PRIV      102
#define IDC_NAVBTN_NEXT      103
#define IDC_NAVBTN_END       104 
#define IDC_NAVBTN_ADD       105
#define IDC_NAVBTN_REMOV     106
#define IDC_NAVBTN_EDIT      107
#define IDC_NAVBTN_UPDATE    108
#define IDC_NAVBTN_CANCEL    109
#define IDC_NAVBTN_REFRESH   110

		afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
		afx_msg BOOL OnEraseBkgnd( CDC *pDC );

		CDataGrid* m_pWGrid;
		CAdodc* m_pWAdc;
		

		void OnNav_Begin();
	 void OnNav_Priv();
	 void OnNav_Next();
	 void OnNav_End();
	 void OnNav_Add();
	 void OnNav_Remov();
	 void OnNav_Edit();
	 void OnNav_Update();
	 void OnNav_Cancel();
	 void OnNav_Refresh();

		void SetBtnState( bool, int id );

		DECLARE_MESSAGE_MAP()	

		static TCreBmpDta bmps[];
	};


#endif
