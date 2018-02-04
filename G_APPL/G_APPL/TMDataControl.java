package  G_APPL;

import java.awt.*;
import java.awt.event.*;
import borland.jbcl.layout.*;
import borland.jbcl.control.*;
import borland.jbcl.dataset.*;
import borland.sql.dataset.*;

import java.rmi.*;
import java.lang.reflect.*;
import borland.jbcl.util.*;
import java.util.*;


public class TMDataControl implements IMDataControl, LoadCancel, IAccount {

  static final  boolean B_TRACE = true;
  private TIntervalTimer m_tim = B_TRACE ? new TIntervalTimer():null;

  public TMDataControl( String sServer )
   {
     m_sServer = sServer;
     m_dm = DMod_Main.getDataModule();
   }
  
  public void IMD_OpenCnct( String sName, String sPwd, String sObj  ) throws RemoteException, DataSetException
   {
     Database db = (Database)GFindObj( m_dm, sObj );
     if( db.isOpen() ) db.closeConnection();
     
     db.setConnection( new ConnectionDescriptor(
       "jdbc:odbc:GUANA",//"jdbc:BorlandBridge:GUANAB",
       sName, sPwd, false, "sun.jdbc.odbc.JdbcOdbcDriver"/*"borland.jdbc.Bridge.LocalDriver"*/) );

     db.openConnection();
   }
  private Object GFindObj( Object oPar, String sNam ) throws RemoteException
   {
     try  {
       Field[] ff = oPar.getClass().getFields();
       int i = ff.length - 1;
       for( ; i >= 0; --i )
        {
          if( ff[ i ].getName().compareTo(sNam) == 0 )
            return ff[ i ].get( oPar );
        }
       throw new NoSuchFieldException( "Field" + " '" + sNam + "' " + "not found" );
     }
    catch( NoSuchFieldException e )
     {
       e.printStackTrace();
       throw new RemoteException( e.getMessage() );
     }
    catch( SecurityException e )
     {
       e.printStackTrace();
       throw new RemoteException( e.getMessage() );
     }
    catch( IllegalAccessException e )
     {
       e.printStackTrace();
       throw new RemoteException( e.getMessage() );
     }
   }
  public void IMD_CloseCnct( String sObj ) throws RemoteException, DataSetException
   {
     Database db = (Database)GFindObj( m_dm, sObj );
     if( db.isOpen() ) db.closeConnection();
   }

  public void CloseAll_DS() throws DataSetException
   {
     if( m_dm.m_dbMain.isOpen() )
       m_dm.m_dbMain.closeConnection();
     if( m_dm.m_dbLook.isOpen() )
       m_dm.m_dbLook.closeConnection();
   }
  public Column[] IMD_GetColumns( String sObj )  throws RemoteException, DataSetException
   {
     if( m_dm == null ) return null;

     Object obj = GFindObj( m_dm, sObj );
     if( !(obj instanceof StorageDataSet) )
       throw new RemoteException( "Field " + "\"" + sObj + "\"" + " is not inst. of StorageDataSet" );

     /*int iCnt = ((StorageDataSet)obj).getColumnCount();
     Column[] clms = new Column[ iCnt ];
     for( int i = 0; i < iCnt; ++i )
       clms[ i ] = ((StorageDataSet)obj).getColumn( i );*/

     return ((StorageDataSet)obj).getColumns();
   }
  public void cancelLoad() {
  }
  public ParameterRow IMD_GetParameterRow( String sStorageName ) throws RemoteException, DataSetException
   {
     if( m_dm == null ) return null;
     Object obj = GFindObj( m_dm, sStorageName );
     if( obj instanceof QueryDataSet )
       return (ParameterRow)((QueryDataSet)obj).getParameterRow();
     else if( obj instanceof ProcedureDataSet )
       return (ParameterRow)((ProcedureDataSet)obj).getParameterRow();
     else throw new RemoteException( "Object " + "\"" + sStorageName + "\"" + " is not inst. of QueryDataSet or ProcedureDataSet" );
   }
  public TDataRet IMD_ProvideData( String sQryName, int iPageSize, int iPage, boolean bCalcPages, TParameterRow_ValTransfer tpvPR ) throws RemoteException, DataSetException
   {
     if( m_dm == null || m_dm.m_dbMain.isOpen() == false ) return null;

     if( B_TRACE ) m_tim.start();

     Object obj = GFindObj( m_dm, sQryName );
     if( !(obj instanceof QueryDataSet) )
       throw new RemoteException( "Object " + "\"" + sQryName + "\"" + " is not inst. of Query" );

     QueryDataSet q = (QueryDataSet)obj;

     if( tpvPR != null ) tpvPR.Load( q );

     TDataRet drRet = new TDataRet();

     q.close(); q.empty();
     q.setMaxRows( bCalcPages ? -1:(iPage * iPageSize + iPageSize + 1) );
     TableDataSet tmp = new TableDataSet();
     q.open();

     try {
       q.executeQuery();

       if( B_TRACE )
         System.out.println( "Execute query on server: " + m_tim.startF() );

       //while( !q.atLast() ) q.last();
       //iRet = q.getRowCount();
       if( q.getRowCount() < 1 )
        {
          drRet.m_ds = null;
          return drRet;
        }

       boolean bFl = q.getRowCount() <= iPage * iPageSize;

       if( bCalcPages || bFl )
        {
          drRet.m_iPages = q.getRowCount();
          if( drRet.m_iPages < 1 ) drRet.m_iPages = 1;
          drRet.m_iPages =
            drRet.m_iPages / iPageSize + (drRet.m_iPages % iPageSize != 0 ? 1:0);
        }
       if( bFl )
         iPage = drRet.m_iPages - 1;

       drRet.m_iReturnedPage = iPage;

       q.goToRow( iPage * iPageSize );

       tmp.setColumns( q.cloneColumns() );
       tmp.open();

       //DataRow row = new DataRow( tmp );
       Variant[] vvt = tmp.startLoading( this, RowStatus.LOADED, false );
       try {
         for( int i = iPageSize; i > 0; --i )
          {
            //q.copyTo( row );
            //tmp.addRow( row );
            for( int k = 0; k < vvt.length; ++k )
             {
               Variant v = new Variant();
               q.getVariant( k, v );
               vvt[ k ].setVariant( v );
             }
            tmp.loadRow();

            if( q.atLast() ) break;
            /*long lll = q.getInternalRow();
            long lll2 = q.getRow();*/
            q.next();
          }
         }
       finally
        {
          tmp.endLoading();
          tmp.close();
        }
     }
     finally
      {
        q.close(); q.empty();
      }

     if( tpvPR != null && tpvPR.NeedReturn() )
      {
        tpvPR.Prepare( q, true );
        drRet.m_tpvPR = tpvPR;
      }
     else drRet.m_tpvPR = null;

     drRet.m_ds = DataSetData.extractDataSet( tmp );

     if( B_TRACE )
       System.out.println( "Prepare results for client on server: " + m_tim.startF() );

     return drRet;
   }

  String GetIDName( DataSet ds, String sNameDS ) throws RemoteException
   {
     Column[] clms = ds.getColumns();
     for( int i = clms.length - 1; i >= 0; --i )
       if( clms[i].isRowId() ) return clms[i].getColumnName();

     throw new RemoteException( "DataSet \"" + sNameDS + "\" haven't RowID. For resolving it's nessary" );
   }
  static public void RemoveColumn( StorageDataSet q, String sName ) throws DataSetException
   {
     Column[] clms = q.getColumns();
     Column[] clms2 = new Column[ clms.length - 1 ];
     boolean bFlFound = false;
     for( int i = 0, j = 0; i < clms.length; ++i )
       if( clms[i].getColumnName().compareTo(sName) != 0 )
         clms2[ j++ ] = clms[ i ];
       else bFlFound = true;

     if( bFlFound ) q.setColumns( clms2 );
   }
  /*public DataSetData MkRefetchRows( QueryDataSet q, DataSetData changes, DataSetData errors ) throws DataSetException
   {
     TableDataSet tmp = new TableDataSet();

     Column[] clms = q.getColumns();
     Column[] clms2 = new Column[ clms.length - 1 ];
     boolean bFlFound = false;
     for( int i = 0, j = 0; i < clms.length; ++i )
       if( clms[i].getColumnName().compareTo("INTERNALROW") != 0 )
         clms2[ j++ ] = clms[ i ];
       else bFlFound = true;

     q.close();
     q.setColumns( bFlFound ? clms2:clms );
     q.open();
     tmp.setColumns( q.cloneColumns() );

     tmp.open();
     try {
       q.first();
       do {
          DataRow rw = new DataRow( q );
          q.getDataRow( rw );
          q.refetchRow( rw );
          tmp.addRow( rw );
        } while( q.next() );
       }
     finally {
       tmp.resetPendingStatus( true );
       tmp.close();
      }

     return DataSetData.extractDataSet( tmp );
   }*/
  public DataSetData[] IMD_ResolveChanges( String sQryName, DataSetData changes, boolean bIdentityMAX ) throws RemoteException, DataSetException
   {
     if( m_dm == null || m_dm.m_dbMain.isOpen() == false ) return null;

     Object obj = GFindObj( m_dm, sQryName );
     if( !(obj instanceof QueryDataSet) )
       throw new RemoteException( "Object " + "\"" + sQryName + "\"" + " is not inst. of Query" );

     if( B_TRACE ) m_tim.start();

     QueryDataSet q = (QueryDataSet)obj;
     ResolverHelp rsHelp =
       new ResolverHelp( GetIDName(q, sQryName),
         q.getDatabase(), q, bIdentityMAX, m_dm.FindActionProc(q), q.getTableName() );

     //DataSetData dsRefetched = null;

     try {
      ((QueryResolver)q.getResolver()).addResolverListener( rsHelp );
    } catch( TooManyListenersException ex) {}
    try {
       if( q.isOpen() ) q.close(); q.empty();
       q.setMaxRows( -1 );
       q.open();
       try {
         changes.loadDataSet( q );
         //if( q.getRowCount() == 0 ) return null;

         if( B_TRACE )
           System.out.println( "Load DataSet for save: " + m_tim.startF() );

         q.saveChanges();

         /*q.getDatabase().start();
         try
          {
            q.saveChanges();
          }
         finally { q.getDatabase().commit(); }*/
       }
       finally {
         q.close(); /*q.empty();*/
         if( B_TRACE )
           System.out.println( "Save changes on DB server: " + m_tim.startF() );
       }
     }
    finally
     {
       try {
         ((QueryResolver)q.getResolver()).removeResolverListener( rsHelp );
       } catch( Exception ex ) {}
       try {
         TMDataControl.RemoveColumn( q, "INTERNALROW" );
         q.empty();

       } catch( Exception ex ) {}
     }

     //DataSetData dsRefetched = DataSetData.extractDataSet( q );

     if( rsHelp.getCntRec() == 0 )
      {
        rsHelp.CloseDS();
        return null;
      }

     DataSetData[] dssReturn =
       new DataSetData[]{ rsHelp.getResyncs(), rsHelp.getErrors() };
     rsHelp.CloseDS();

     if( B_TRACE )
       System.out.println( "Return resyncs&errors to client: " + m_tim.startF() );

     return dssReturn;
   }

  public void IMD_SetQueryText( String sQryName, String sDBName, String sQryText )  throws RemoteException, DataSetException
   {
     Object objQ = GFindObj( m_dm, sQryName );
     if( !(objQ instanceof QueryDataSet) )
       throw new RemoteException( "Object " + "\"" + sQryName + "\"" + " is not inst. of Query" );

     Object objDB = GFindObj( m_dm, sDBName );
     if( !(objDB instanceof Database) )
       throw new RemoteException( "Object " + "\"" + sDBName + "\"" + " is not inst. of DataBase" );

     QueryDataSet q = (QueryDataSet)objQ;
     Database db = (Database)objDB;

     if( q.isOpen() ) q.close();
     q.setQuery( new QueryDescriptor(db, sQryText, null, false, Load.ALL) );
   }

  public DataSetData IMD_ProvideDataAll( String sQryName )  throws RemoteException, DataSetException
   {
     if( m_dm == null || m_dm.m_dbMain.isOpen() == false ) return null;

     Object obj = GFindObj( m_dm, sQryName );
     if( !(obj instanceof QueryDataSet) )
       throw new RemoteException( "Object " + "\"" + sQryName + "\"" + " is not inst. of Query" );

     QueryDataSet q = (QueryDataSet)obj;

     q.close();
     q.empty();
     q.setMaxRows( -1 );
     q.open();

     try {
       q.executeQuery();
       if( q.getRowCount() < 1 ) return null;
     }
     finally
      {
        q.close();
      }

     DataSetData dsRet = DataSetData.extractDataSet( q );
     q.empty();
     return dsRet;
   }

  public void IAcc_PayEnrol( int iBILL_NO, boolean bFlEnrol ) throws RemoteException, DataSetException
   {
     m_dm.prPayEnroll.setInt( "bill_no", iBILL_NO );
     m_dm.prPayEnroll.setInt( "flag", bFlEnrol ? 1:0 );
     ProcedureProvider.callProcedure(
       m_dm.m_dbMain,
       "execute PayEnroll :bill_no, :flag",
       m_dm.prPayEnroll );
   }

  protected DMod_Main m_dm = null;
  protected String m_sServer;
}


class ResolverHelp extends ResolverAdapter {

  private Database m_db;
  private QueryDataSet m_q;

  private QueryDataSet qResync = null, qGetIDENTITY = null, qGetMaxID = null;
  private ParameterRow prowResync = new ParameterRow();
  private int m_iCntRec = 0;
  private boolean m_bIdentityMAX = false;
  private long m_lIdentity = 0;

  private TExtProcs m_tepProcs = null;
  //FindActionProc
  private String m_sTableName;

  public int getCntRec() { return m_iCntRec; }

  public ResolverHelp( String sNameOfKey, Database db, QueryDataSet q, boolean bIdentityMAX, TExtProcs tepProcs, String sTableName )
   {
     super();
     m_sTableName = sTableName;
     m_sKey = sNameOfKey; m_db = db; m_q = q;
     m_bIdentityMAX = bIdentityMAX;
     m_tepProcs = tepProcs == null ? new TExtProcs():tepProcs;
   }

  public void deletingRow( ReadWriteRow rw, ResolverResponse rsp ) throws DataSetException
   {
     if( m_tepProcs.m_q != null )
      {
        rsp.skip();
        m_tepProcs.m_prDel.setInt( 0, rw.getInt(m_sKey) );
        ProcedureProvider.callProcedure( m_db, m_tepProcs.m_sDel, m_tepProcs.m_prDel );
        m_iCntRec++;
      }
     else { rsp.resolve(); m_db.start(); }
   }
  public void deletedRow( ReadWriteRow row ) throws DataSetException
   {
     m_iCntRec++;
     if( m_tepProcs.m_q == null ) m_db.commit();
   }
  public void deleteError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws  DataSetException {
    m_iCntRec++;
    handleError( ResolverHelp.iDelete, dataSet, row, ex, response, null );
    if( m_tepProcs.m_q == null ) m_db.rollback();
  }

  public void insertingRow( ReadWriteRow row, ResolverResponse response ) throws DataSetException
   {
     response.resolve();
     m_db.start();
     if( m_bIdentityMAX )
      {
        createResyncDataSet();
        qGetMaxID.refresh();
        m_lIdentity = qGetMaxID.getInt( 0 ) + 1;
      }
   }
  public void insertedRow( ReadWriteRow row ) throws DataSetException
   {
     m_iCntRec++;
     try {
       if( m_bIdentityMAX ) MkResyncRow( row, new Long(m_lIdentity) );
       else
        {
          createResyncDataSet();
          qGetIDENTITY.refresh();
          java.math.BigDecimal bcd = qGetIDENTITY.getBigDecimal( 0 );
          MkResyncRow( row, new Long(bcd.longValue()) );
        }
       }
     finally { m_db.commit(); }
     //m_q.refetchRow( row );
   }
  public void insertError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
    m_iCntRec++;
    handleError( ResolverHelp.iInsert, dataSet, row, ex, response, null );
    m_db.rollback();
  }

  public void updatingRow( ReadWriteRow row, ReadRow oldRow, ResolverResponse response ) throws DataSetException
   {
     response.resolve();
     m_db.start();
   }
  public void updatedRow( ReadWriteRow row, ReadRow oldRow ) throws DataSetException
   {
     m_iCntRec++;
     MkResyncRow( row, null );
     m_db.commit();
   }
  public void updateError(DataSet dataSet, ReadWriteRow row, ReadRow oldRow, ReadWriteRow updRow, DataSetException ex, ErrorResponse response)  throws DataSetException {
    m_iCntRec++;
    handleError( ResolverHelp.iUpdate, dataSet, row, ex, response, updRow );
    m_db.rollback();
  }

  DataSetData getErrors() throws DataSetException {
    if( errorDataSet == null ) return null;
    DataSetData dsRet = DataSetData.extractDataSet( errorDataSet );
    errorDataSet.close();
    return dsRet;
  }

  DataSetData getResyncs() throws DataSetException {
    if( resyncDataSet == null ) return null;
    DataSetData dsRet = DataSetData.extractDataSet( resyncDataSet );
    resyncDataSet.close();
    return dsRet;
  }

  private void handleError( int iOp, DataSet dataSet, ReadWriteRow rwBad, DataSetException ex, ErrorResponse response, ReadWriteRow rw ) throws DataSetException {
    createErrorDataSet();
    errorDataSet.insertRow(false);

    /*int ii = dataSet.getInt( "ID_ROOM" );
    long ii2 = dataSet.getLong( "INTERNALROW" );
    String sss = dataSet.getString( "NAME" );
    Variant vvv = new Variant();
    dataSet.getVariant( "RATE", vvv );*/

    errorDataSet.setLong( ResolverHelp.sInternalRow, rwBad.getLong(ResolverHelp.sInternalRow) );
    errorDataSet.setObject( ResolverHelp.sException, ex );
    errorDataSet.setInt( ResolverHelp.sOperacion, iOp );

    if( iOp == iUpdate )
     {
       DataRow rww = new DataRow( dataSet );
       rw.copyTo( rww );
       try {
         dataSet.refetchRow( rww );
        }
       catch( Exception exc )
        {
        }
       errorDataSet.setObject( ResolverHelp.sNewRow, rww );
     }

    errorDataSet.post();
    response.ignore();
  }

  private void MkResyncRow( ReadWriteRow rw, Long lKey )
   {
     try {
       createResyncDataSet();

       prowResync.setInt( "my_pk_id", (int)(lKey == null ? rw.getLong(m_sKey):lKey.longValue()) );
       qResync.refresh();
       if( qResync.getRowCount() < 1 ) return;

       qResync.first();
       DataRow rNew = new DataRow( resyncDataSet );

       qResync.copyTo( rNew );
       rNew.setLong( ResolverHelp.sInternalRow, rw.getLong(ResolverHelp.sInternalRow) );
       resyncDataSet.addRow( rNew );
      }
     catch( Exception ex ) {}
   } //***************

  private void createResyncDataSet() throws DataSetException {
    if( resyncDataSet != null )
      return;

    resyncDataSet = new TableDataSet();
    //resyncDataSet.setTableName( m_q.getTableName() );
    resyncDataSet.setColumns( m_q.cloneColumns() );
    //TMDataControl.RemoveColumn( resyncDataSet, "INTERNALROW" );
    //INTERNALROW - is need

    resyncDataSet.open();

    try {
       Column clmPK_Resync = new Column();
       clmPK_Resync.setColumnName( "my_pk_id" );
       clmPK_Resync.setDataType( Variant.INT );
       prowResync.setColumns( new Column[]{clmPK_Resync} );

       qResync = new QueryDataSet();
       //qResync.setTableName( m_q.getTableName() );
       qResync.setMetaDataUpdate( MetaDataUpdate.NONE );
       qResync.setQuery(
         new QueryDescriptor(m_db,
           m_tepProcs.m_sResync == null ?
           ("select * from " + m_sTableName + " where " + m_sKey + "=:my_pk_id"):
           m_tepProcs.m_sResync,
           prowResync, false, Load.ALL) );
       qResync.setColumns( m_q.cloneColumns() );
       qResync.open();


       qGetIDENTITY = new QueryDataSet();
       //qGetIDENTITY.setTableName( m_q.getTableName() );
       qGetIDENTITY.setMetaDataUpdate( MetaDataUpdate.NONE );
       qGetIDENTITY.setQuery(
         new QueryDescriptor(m_db,
           "select @@IDENTITY as ID_NEW_ID",
           null, false, Load.ALL) );
       qGetIDENTITY.open();

       Column clmMax = new Column();
       clmMax.setColumnName( "ID_MAX_ID" );
       clmMax.setDataType( Variant.INT );

       qGetMaxID = new QueryDataSet();
       //qGetMaxID.setTableName( m_q.getTableName() );
       qGetMaxID.setMetaDataUpdate( MetaDataUpdate.NONE );
       qGetMaxID.setQuery(
         new QueryDescriptor(m_db,
           "select max(" + m_sKey + ") as ID_MAX_ID from " + m_sTableName,
           null, false, Load.ALL) );
       qGetMaxID.setColumns( new Column[]{clmMax} );
       qGetMaxID.open();

     }
     catch( Exception e )
      {
        System.out.println( e.getMessage() );
      }
  }

  private void createErrorDataSet() throws DataSetException {
    if (errorDataSet != null)
      return;
    errorDataSet = new TableDataSet();
    errorDataSet.setColumns(new Column[]{
      new Column( ResolverHelp.sInternalRow, ResolverHelp.sInternalRow, Variant.LONG ),
      new Column( ResolverHelp.sException, ResolverHelp.sException, Variant.OBJECT ),
      new Column( ResolverHelp.sOperacion, ResolverHelp.sOperacion, Variant.INT ),
      new Column( ResolverHelp.sNewRow, ResolverHelp.sNewRow, Variant.OBJECT )//used if update only
                                         });
    errorDataSet.getColumn( 0 ).setRequired( true );
    errorDataSet.getColumn( 1 ).setRequired( true );
    errorDataSet.getColumn( 2 ).setRequired( true );
    errorDataSet.getColumn( 3 ).setRequired( false );

    errorDataSet.open();
  }
  private void MkCloseDataSet( StorageDataSet ds ) throws DataSetException
   {
     if( ds != null && ds.isOpen() )
      { ds.close(); ds.empty(); }
   }
  public void CloseDS() throws DataSetException
   {
     MkCloseDataSet( errorDataSet );
     MkCloseDataSet( qResync );
     MkCloseDataSet( qGetIDENTITY );
     MkCloseDataSet( qGetMaxID );
     errorDataSet = null; qResync = null;
     qGetIDENTITY = null; qGetMaxID = null;
   }

  protected void finalize() throws Throwable {
     CloseDS();
     super.finalize();
  }


  private String m_sKey;
  private TableDataSet errorDataSet, resyncDataSet;

  static final String sInternalRow = "INTERNALROW";
  static final String sException   = "EXCEPTION";
  static final String sOperacion   = "OPERACION";
  static final String sNewRow      = "NEW_ROW";

  static final int iIdx_INTERNALROW = 0;
  static final int iIdx_EXCEPTION   = 1;
  static final int iIdx_OPERACION   = 2;
  static final int iIdx_NEW_ROW     = 3;

  static final int iInsert = 0;
  static final int iDelete = 1;
  static final int iUpdate = 2;
}


