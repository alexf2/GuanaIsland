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
import G_APPL.*;


public class TPgProvider extends Provider implements LoadCancel{
  static final  boolean B_TRACE = GApplet.B_TRACE;
  private TIntervalTimer m_tim = B_TRACE ? new TIntervalTimer():null;
  
  DataSetException asyncThreadException = null;
  LoadCancel cancelDataSetData = null;
  StorageDataSet m_sDS = null;
  private boolean m_bPageChanfed = false;
  private boolean m_bNeedRecalcPages = true;
  private boolean m_bFirstCall = true;
  private Component m_cPar = null;

  private TParameterRow_ValTransfer m_tvp = null;

  public TPgProvider() { super(); }
  public TPgProvider( Component c ) { super(); m_cPar = c; }

  public void cancelLoad() {
    if( cancelDataSetData != null ) cancelDataSetData.cancelLoad();
    System.out.println("Stopping");
  }

  public void provideMoreData( StorageDataSet dataSet ) throws DataSetException {
    //System.out.println( "provideMoreData" );
    throwAsyncException();
  }

  public void ifBusy( StorageDataSet dataSet ) throws DataSetException {
    throwAsyncException();
  }

  public void throwAsyncException() throws DataSetException {
    if( asyncThreadException != null ) {
       DataSetException temp = asyncThreadException;
       asyncThreadException = null;
       throw temp;
    }
  }

  public boolean hasMoreData(StorageDataSet dataSet) {
    //System.out.println("hasMoreData");
    //return !asynchronous && !allDataReceived;
    return false;
  }


  public void close( StorageDataSet dataSet, boolean loadRemainingRows ) {
  }
  private void PrepareParams() throws DataSetException
   {
     if( m_tvp != null && paramRow != null )
       m_tvp.Prepare( paramRow, false );
   }
  public void provideData( StorageDataSet dataSet, boolean toOpen ) throws DataSetException
  {
    if( toOpen ) return;

    try { if( dataSet.isEditing() ) dataSet.cancel(); }
    catch( Exception exx ) {}

    if( B_TRACE ) m_tim.start();

    m_sDS = dataSet;
    //if( toOpen ) m_bNeedRecalcPages = true;
    Cursor csrOld = m_cPar.getCursor();
    m_cPar.setCursor( new Cursor(Cursor.WAIT_CURSOR) );
    try {
      if( dataCtrlIntf == null ) return;

      PrepareParams();

      boolean bRecalc = m_bNeedRecalcPages;
      m_bNeedRecalcPages = true;
      dataSet.empty();

      firePageLoaded( new PgProvEvent(this) );

      TDataRet dsRet = dataCtrlIntf.IMD_ProvideData( nameQryOnServer, sizePage, curPage, bRecalc, m_tvp );

      if( B_TRACE )
        System.out.println( "Load page #" + Integer.toString(curPage) + " from server: " + m_tim.startF() );

      if( dsRet == null ) return;
      if( dsRet.m_tpvPR != null ) dsRet.m_tpvPR.Load( paramRow );
      DataSetData  dsNew = dsRet.m_ds;
      if( bRecalc || dsRet.m_iReturnedPage != curPage )
       {
         curPage =  dsRet.m_iReturnedPage;
         //totalPages =  dsRet.m_iPages;
         SetTotalPages_Internal( dsRet.m_iPages, true );
       }

      //int row = -1;
      //if( !dataSet.isEmpty() ) row = dataSet.getRow();

      if( dsNew != null )
       {
         dataSet.enableDataSetEvents( false );
         try {
           cancelDataSetData = dsNew;
           try { dsNew.loadDataSet( dataSet ); }
           finally { cancelDataSetData = null; }

           if( B_TRACE )
             System.out.println( "loadDataSet: " + m_tim.elapsedF() );
         }
         finally {
           dataSet.enableDataSetEvents( true );
           /*if( row != -1 && m_bPageChanfed == false )
            try { dataSet.goToRow( row ); }
            catch( DataSetException e ) {}*/
           m_bPageChanfed = false;
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

  private ParameterRow paramRow = null;
  private IMDataControl dataCtrlIntf = null;
  private String nameQryOnServer = null;

  public IMDataControl getDataCtrlIntf() { return dataCtrlIntf; }
  public String getNameQryOnServer() { return nameQryOnServer; }
  public void setDataCtrlIntf( IMDataControl dc ) throws RemoteException, DataSetException
   {
     dataCtrlIntf = dc;
     paramRow = dataCtrlIntf.IMD_GetParameterRow( nameQryOnServer );
     if( paramRow != null ) m_tvp = new TParameterRow_ValTransfer();
   }
  public void setNameQryOnServer( String s ) { nameQryOnServer = s; }

  public ParameterRow getParamRow() { return paramRow; }

  private int curPage = 0;
  private int totalPages = 1;
  private int sizePage = 50;

  public int getCurPage() { return curPage; }
  public void setCurPage( int p ) throws DataSetException
   {
     if( p == curPage ) return;

     int iKey = curPage;
     curPage = p;
     m_bPageChanfed = true;
     if( m_sDS == null ) return;
     try {
       m_bNeedRecalcPages = false;
       m_sDS.refresh();
     }
     catch( DataSetException ex )
      {
        curPage = iKey;
        throw ex;
      }
   }
  private void SetTotalPages_Internal( int p, boolean bNoFire )
   {
     if( !m_bFirstCall && totalPages == p ) return;
     m_bFirstCall = false;

     totalPages = p;

     if( lstPages == null ) return;
     int iKey = lstPages.getSelectedIndex();
     lstPages.removeItemListener( m_itemLst );
     boolean bFiring = false;
     try {
        lstPages.removeAll();
        for( int i = 1; i <= totalPages; ++i )
          lstPages.add( " " + (new Integer(i)).toString() );

        if( iKey < 0 ) { lstPages.select(curPage); bFiring = true; }
        else if( iKey < totalPages ) lstPages.select( iKey );
        else { lstPages.select( (curPage=totalPages - 1) ); bFiring = true; }
      }
     finally {
        lstPages.addItemListener( m_itemLst );
      }
     if( bNoFire == false && bFiring == true )
      {
        int it = lstPages.getSelectedIndex();
        lstPages.deselect( it );
        lstPages.select( it );
      }
   }
  public int getTotalPages() { return totalPages; }
  public void setTotalPages( int p )
   {
     SetTotalPages_Internal( p, false );
   }
  public int getSizePage() { return sizePage; }
  public void setSizePage( int ps ) { sizePage = ps; }

  private List lstPages = null;

  public List getLstPages() { return lstPages; }
  public void setLstPages( List lst )
   {
     RemoveOldListeners();
     lstPages = lst;
     lstPages.addItemListener( m_itemLst );
   }

  class ThItemListener implements java.awt.event.ItemListener
   {
     public void itemStateChanged( ItemEvent e )
       {
         listPag_itemStateChanged( e );
       }
   }

  private ThItemListener m_itemLst = this.new ThItemListener();
  private transient Vector pgProvListeners;

  void listPag_itemStateChanged( ItemEvent e )
   {
     if( e.getStateChange() != ItemEvent.SELECTED ) return;

     try {
       setCurPage( Integer.parseInt(e.getItem().toString()) );
     }
     catch( DataSetException ex )
      {
        ex.printStackTrace();
        MessageDialog dlg = new MessageDialog( new DecoratedFrame(),
         "Error (m)", ex.getMessage(), ButtonDialog.OK );
        dlg.show();

        lstPages.removeItemListener( m_itemLst );
        try {
          lstPages.select( curPage );
          m_bPageChanfed = false;
         }
        finally
         {
           lstPages.addItemListener( m_itemLst );
         }
      }
   }
  private void RemoveOldListeners()
   {
     if( lstPages != null ) lstPages.removeItemListener( m_itemLst );
   }

  public synchronized void removePgProvListener(PgProvListener l) {
    if (pgProvListeners != null && pgProvListeners.contains(l)) {
      Vector v = (Vector) pgProvListeners.clone();
      v.removeElement(l);
      pgProvListeners = v;
    }
  }

  public synchronized void addPgProvListener(PgProvListener l) {
    Vector v = pgProvListeners == null ? new Vector(2) : (Vector) pgProvListeners.clone();
    if (!v.contains(l)) {
      v.addElement(l);
      pgProvListeners = v;
    }
  }

  protected void firePageLoaded(PgProvEvent e) {
    if (pgProvListeners != null) {
      Vector listeners = pgProvListeners;
      int count = listeners.size();
      for (int i = 0; i < count; i++)
        ((PgProvListener) listeners.elementAt(i)).pageLoaded(e);
    }
  }




}


