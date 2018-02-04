
package G_APPL;

import borland.sql.dataset.*;
import borland.jbcl.dataset.*;
import borland.jbcl.util.*;
import borland.jbcl.control.*;
import java.rmi.*;
import java.net.InetAddress;
import java.util.*;
import java.awt.*;
import java.awt.event.*;

public class TAsyncAllProvider extends Provider implements LoadCancel
{
  private StorageDataSet m_sDS = null;
  LoadCancel cancelDataSetData = null;
  private IMDataControl dataCtrlIntf = null;
  private String nameQryOnServer = null;

  private Component m_cPar = null;

  public IMDataControl getDataCtrlIntf() { return dataCtrlIntf; }
  public String getNameQryOnServer() { return nameQryOnServer; }
  public void setDataCtrlIntf( IMDataControl dc ) throws RemoteException, DataSetException
   {
     dataCtrlIntf = dc;
   }
  public void setNameQryOnServer( String s ) { nameQryOnServer = s; }


  public TAsyncAllProvider( Component c ) { super(); m_cPar = c; }

  public void cancelLoad() {
    if( cancelDataSetData != null ) cancelDataSetData.cancelLoad();
  }

  public void provideMoreData( StorageDataSet dataSet ) throws DataSetException {
   }

  public void ifBusy( StorageDataSet dataSet ) throws DataSetException {
   }

  public boolean hasMoreData(StorageDataSet dataSet) {
    return false;
  }

  public void close( StorageDataSet dataSet, boolean loadRemainingRows ) {
  }

  public void provideData( StorageDataSet dataSet, boolean toOpen ) throws DataSetException
  {
    m_sDS = dataSet;

    Cursor csrOld = m_cPar.getCursor();
    m_cPar.setCursor( new Cursor(Cursor.WAIT_CURSOR) );
    try {
      if( dataCtrlIntf == null ) return;

      dataSet.empty();
      DataSetData dsNew = dataCtrlIntf.IMD_ProvideDataAll( nameQryOnServer );

      if( dsNew != null )
       {
         dataSet.enableDataSetEvents( false );
         try {
           cancelDataSetData = dsNew;
           try { dsNew.loadDataSet( dataSet ); }
           finally { cancelDataSetData = null; }
         }
         finally {
           dataSet.enableDataSetEvents( true );
         }
       }
    }
    catch( Exception ex ) {
      dataSet.close();
      throw new DataSetException( ex.getMessage() );
    }
   finally
    {
      m_cPar.setCursor( csrOld );
    }
  }
}

