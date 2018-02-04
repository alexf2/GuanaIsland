#if !defined(_FLD_BINDER_)
#define _FLD_BINDER_

class TFieldBind
	{

friend class TFieldsMgr;

public:
	TFieldBind(): m_bChanged( false ), m_bNull( true ), m_iIDCtrl(0)
		{
		}
	virtual ~TFieldBind()
		{
		}

	virtual void Bind( _RecordsetPtr ptr, LPCTSTR lpNam, int id, CWnd* pD, bool bReadOnly = false )
		{
		  m_pRec = ptr; m_csFieldName = lpNam; m_iIDCtrl = id;
				m_bChanged = false;
				m_bRdOnly = bReadOnly;
		}

protected:
		virtual void LoadFromControl() = 0;
		virtual void LoadToControl() = 0;

		virtual void Fetch() = 0;
		virtual bool Store( bool bNoCheck = false ) = 0;

		virtual void Clear() = 0;

	 bool m_bNull;
		int m_iIDCtrl;
		bool m_bChanged;

		bool m_bRdOnly;

		_RecordsetPtr m_pRec;
		CString m_csFieldName;		
	};

class TFieldBind_Int: public TFieldBind
	{

friend class TFieldsMgr;

public:
	  TFieldBind_Int(): TFieldBind()
				{
				  m_iValue = 0;
				}
			virtual ~TFieldBind_Int()
				{
				}

			virtual void Bind( _RecordsetPtr ptr, LPCTSTR lpNam, int id, CWnd* pD, bool bReadOnly = false )
				{		  
				  TFieldBind::Bind( ptr, lpNam, id, pD, bReadOnly );
				  m_ed = (CEdit*)pD;
				}

protected:
			virtual void LoadFromControl();
			virtual void LoadToControl();

			virtual void Fetch();
			virtual bool Store( bool bNoCheck = false );

			virtual void Clear();

	  int m_iValue;
			CEdit* m_ed;
	};

class TFieldBind_Str: public TFieldBind
	{

friend class TFieldsMgr;

public:
	  TFieldBind_Str(): TFieldBind()
				{
				}
			virtual ~TFieldBind_Str()
				{
				}

			virtual void Bind( _RecordsetPtr ptr, LPCTSTR lpNam, int id, CWnd* pD, bool bReadOnly = false )
				{		  
				  TFieldBind::Bind( ptr, lpNam, id, pD, bReadOnly );
				  m_ed = (CEdit*)pD;
				}

protected:
			virtual void LoadFromControl();
			virtual void LoadToControl();

			virtual void Fetch();
			virtual bool Store( bool bNoCheck = false );

			virtual void Clear();

	  CString m_sValue;

			CEdit* m_ed;
	};

class TFieldBind_Money: public TFieldBind
	{

friend class TFieldsMgr;

public:
	  TFieldBind_Money(): TFieldBind()
				{
				  m_currValue.SetCurrency( 0, 0 );
				}
			virtual ~TFieldBind_Money()
				{
				}

			virtual void Bind( _RecordsetPtr ptr, LPCTSTR lpNam, int id, CWnd* pD, bool bReadOnly = false )
				{		  
				  TFieldBind::Bind( ptr, lpNam, id, pD, bReadOnly );
				  m_ed = (CEdit*)pD;
				}

protected:
			virtual void LoadFromControl();
			virtual void LoadToControl();

			virtual void Fetch();
			virtual bool Store( bool bNoCheck = false );

			virtual void Clear();

	  COleCurrency m_currValue;
			CEdit* m_ed;
	};


class TFieldBind_Date: public TFieldBind
	{

friend class TFieldsMgr;

public:
	  TFieldBind_Date(): TFieldBind()
				{
				  m_odtValue = COleDateTime::GetCurrentTime();
						m_odtValue.SetDateTime( m_odtValue.GetYear(), 
				    m_odtValue.GetMonth(), m_odtValue.GetDay(), 0, 0, 0 );
				}
			virtual ~TFieldBind_Date()
				{
				}

			virtual void Bind( _RecordsetPtr ptr, LPCTSTR lpNam, int id, CWnd* pD, bool bReadOnly = false )
				{		  
				  TFieldBind::Bind( ptr, lpNam, id, pD, bReadOnly );
				  m_date = (CDateTimeCtrl*)pD;
				}

protected:
			virtual void LoadFromControl();
			virtual void LoadToControl();

			virtual void Fetch();
			virtual bool Store( bool bNoCheck = false );

			virtual void Clear();

	  COleDateTime m_odtValue;
			CDateTimeCtrl* m_date;
	};

typedef auto_ptr<TFieldBind> TAP_FieldBind;
typedef list<TAP_FieldBind> TVEC_FieldBindAP;

class TFieldsMgr
	{
public:
	  TFieldsMgr()
				{
				}

			void LoadFromControl( bool bOne = false, int id = -1 );
			void LoadToControl( bool bOne = false, int id = -1 );

			void Fetch( bool bOne = false, int id = -1 );
			int Store( bool bNoCheck = false, bool bOne = false, int id = -1 );

   TFieldsMgr& operator,( TAP_FieldBind& rAp )
				{
				  m_vec.push_back( rAp );
						return *this;
				}

			void Clear( bool bOne = false, int id = -1 );

protected:
	 TVEC_FieldBindAP m_vec;
	};

#endif
