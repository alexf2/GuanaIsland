#if !defined(_MY_LOOKUP_)
#define _MY_LOOKUP_


class MyLB: public CListBox
	{
public:
		MyLB( CDialog* pL ): CListBox(), m_pL( pL )
			{
			}

		afx_msg void OnLButtonUp( UINT nFlags, CPoint point );

		CDialog* m_pL;

		DECLARE_MESSAGE_MAP()
	};

class TLookUp: public CDialog
 {

struct TLookSlot
	{
	  TLookSlot(): key(0)
				{
				}
			TLookSlot( const TLookSlot& rS )
				{
				  this->operator=( rS );
				}
			TLookSlot& operator=( const TLookSlot& rS )
				{
				  key = rS.key, val = rS.val;
				}

	  int key;
			CString val;
	};
typedef auto_ptr<TLookSlot> TAP_LookSlot;
typedef list<TAP_LookSlot>  TLST_LookSlot;

public: 
  TLookUp( CWnd *pPar, CDataGrid *pGr, _ConnectionPtr pCnct ):
    CDialog( IDD_DIALOG_LOOKUP, pPar ),
				m_pGr( pGr ),
				m_pCnct( pCnct ),
				m_bPassed( false ), m_bUnboundMode( false ),
				m_lBox( this )
   { 			  				
   }
		virtual ~TLookUp();

		void Ass( _RecordsetPtr pRecMain, _RecordsetPtr pRecLook,
			 LPCTSTR lpKey, LPCTSTR lpFieldLook, LPCTSTR lpFieldReplicat );
		void Ass( _RecordsetPtr pRecMain, LPCTSTR lpFieldReplicat,
			 LPCTSTR* );
		
  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );
		int OnCreate( LPCREATESTRUCT lpC );
		void OnShowWindow( BOOL bShow, UINT nStatus );

  virtual void OnOK();
		virtual void OnCancel();		

		void ReFill();
		
		afx_msg void OnDestroy();
		afx_msg void OnLbSel();

		afx_msg void OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized );		
		
				
private:       
		_ConnectionPtr m_pCnct;
		CDataGrid *m_pGr;										

		bool m_bUnboundMode;

		_RecordsetPtr m_pRecMain, m_pRecLook;
		CString m_sKey, m_sFieldLook, m_sFieldReplicat;

		TLST_LookSlot m_lst;
		MyLB m_lBox;
		bool m_bHandle;

		bool m_bPassed;
		
  DECLARE_MESSAGE_MAP()
 };




#endif
