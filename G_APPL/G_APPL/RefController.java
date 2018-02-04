package G_APPL;

import java.rmi.*;
import java.rmi.server.*;
import borland.jbcl.dataset.DataSetException;

import borland.sql.dataset.*;
import borland.jbcl.dataset.*;
import borland.jbcl.util.*;


public class RefController extends RemoteObject
  implements IRefController, Unreferenced, IMDataControl, IAccount {

  public RefController( IRef_Response resp, java.rmi.dgc.VMID vmVMID, String sServer ) throws RemoteException {
    UnicastRemoteObject.exportObject( this );

    m_VMID = vmVMID;
    m_iNum = resp.ResponseMakeRef( this );
    m_respToRef = resp;
    m_bFl = true;

    m_dc = new TMDataControl( sServer );
  }

  public void ClearRef() throws RemoteException
   {
     if( m_bFl == false ) return;
     m_respToRef.ResponseClearRef( this );
     m_respToRef = null;
     m_bFl = false;

     try {
       m_dc.CloseAll_DS();
      }
     catch( DataSetException ex )
      {
        ex.printStackTrace();
      }
     m_dc = null;
   }
  public  void unreferenced()
   {
     try {
       if( m_bFl == true ) ClearRef();
     }
     catch( RemoteException ex )
      {
        ex.printStackTrace();
      }
   }

  protected void finalize() throws Throwable
   {
     m_bFl = false;
     m_respToRef = null;
   }

  public int getNum()
   {
     return m_iNum;
   }
  public java.rmi.dgc.VMID getVMID()
   {
     return m_VMID;
   }

  public void IMD_OpenCnct( String sName, String sPwd, String sObj  ) throws RemoteException, DataSetException
   {
     if( m_dc != null ) m_dc.IMD_OpenCnct( sName, sPwd, sObj );
   }
  public void IMD_CloseCnct( String sObj ) throws RemoteException, DataSetException
   {
     if( m_dc != null ) m_dc.IMD_CloseCnct( sObj );
   }
  public Column[] IMD_GetColumns( String sObj )  throws RemoteException, DataSetException
   {
     if( m_dc != null ) return m_dc.IMD_GetColumns( sObj );
     else return null;
   }
  public TDataRet IMD_ProvideData( String sQryName, int iPageSize, int iPage, boolean bCalcPages, TParameterRow_ValTransfer tpvPR ) throws RemoteException, DataSetException
   {
     if( m_dc != null ) return m_dc.IMD_ProvideData( sQryName, iPageSize, iPage, bCalcPages, tpvPR );
     else return null;
   }
  public DataSetData[] IMD_ResolveChanges( String sQryName, DataSetData changes, boolean bIdentityMAX ) throws RemoteException, DataSetException
   {
     if( m_dc != null ) return m_dc.IMD_ResolveChanges( sQryName, changes, bIdentityMAX );
     else return null;
   }
  public ParameterRow IMD_GetParameterRow( String sStorageName ) throws RemoteException, DataSetException
   {
     if( m_dc != null ) return m_dc.IMD_GetParameterRow( sStorageName );
     else return null;
   }
  public DataSetData IMD_ProvideDataAll( String sQryName )  throws RemoteException, DataSetException
   {
     if( m_dc != null ) return m_dc.IMD_ProvideDataAll( sQryName );
     else return null;
   }
  public void IMD_SetQueryText( String sQryName, String sDBName, String sQryText )  throws RemoteException, DataSetException
   {
     if( m_dc != null ) m_dc.IMD_SetQueryText( sQryName, sDBName, sQryText );
     else return;
   }

  public void IAcc_PayEnrol( int iBILL_NO, boolean bFlEnrol ) throws RemoteException, DataSetException
   {
     if( m_dc != null ) m_dc.IAcc_PayEnrol( iBILL_NO, bFlEnrol );
     else return;
   }

  private boolean m_bFl = false;
  private IRef_Response m_respToRef = null;
  private int m_iNum = 0;
  private java.rmi.dgc.VMID m_VMID;

  private TMDataControl m_dc = null;
}
