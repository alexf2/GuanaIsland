package G_APPL;

import borland.sql.dataset.*;
import borland.jbcl.dataset.*;
import borland.jbcl.util.*;
import borland.jbcl.control.*;
import java.rmi.*;
import java.util.*;
import java.awt.*;
import com.sun.java.swing.*;

class TErrorItem
 {
   public TErrorItem()
    {
      m_ex = null; m_lIntRow = 0;
    }
   public TErrorItem( Exception ex, long  lIntRow )
    {
      m_ex = ex; m_lIntRow = lIntRow;
    }
   public Exception m_ex;
   public long  m_lIntRow;
 }


public class TResolver extends Resolver {
  static final  boolean B_DEBUG = false;
  static final  boolean B_TRACE = GApplet.B_TRACE;
  private TIntervalTimer m_tim = B_TRACE ? new TIntervalTimer():null;
  
  private Component m_cPar = null;

  public TErrorItem[] m_vErrors = null;

  public TResolver() {
  }
  public TResolver( Component c ) { super(); m_cPar = c; }

  public TErrorItem FindErrorByInt( long lIntRow )
   {
     if( m_vErrors == null ) return null;
     for( int i = m_vErrors.length - 1; i >= 0; --i )
       if( m_vErrors[i].m_lIntRow == lIntRow ) return m_vErrors[i];

     return null;
   }


  class TPgProvList implements PgProvListener
   {
     public void pageLoaded( PgProvEvent e )
      {
        m_vErrors = null;
      }
   }

  
  public void BindToPageProvider( TPgProvider prPg )
   {
     prPg.addPgProvListener( this.new TPgProvList() );
   }

  public void close( StorageDataSet dataSet ) throws DataSetException
   {
   }
  public void ifBusy( StorageDataSet dataSet ) throws DataSetException
   {
   }
  static void DumpDS( DataSet dataSet, String sPrt )
   {
     if( TResolver.B_DEBUG == false ) return;
     try {
       System.out.println("");
       System.out.println( " *** " + sPrt + " *** " );
       int iCnt = dataSet.getRowCount();
       long lKey = dataSet.getInternalRow();
       dataSet.enableDataSetEvents( false );
       for( int i = 0; i < iCnt; ++i )
        {
          final  int[] iStats = new int[]{
            RowStatus.DELETED,
            RowStatus.INSERTED,
            RowStatus.LOADED,
            RowStatus.ORIGINAL,
            RowStatus.PENDING_RESOLVED,
            RowStatus.UPDATED };
          final String[] sStatsDS = new String[]{
            "DELETED",
            "INSERTED",
            "LOADED",
            "ORIGINAL",
            "PENDING_RESOLVED",
            "UPDATED" };

          dataSet.goToRow( i );

          String sStats = " ";
          for( int k = iStats.length - 1; k >= 0; --k )
            if( (dataSet.getStatus()& iStats[ k ]) != 0 )
              sStats += sStatsDS[ k ] + " ";

          sStats += " [" + dataSet.getString(1) + "]";

          System.out.println( sStats );
        }
       dataSet.goToInternalRow( lKey );
       dataSet.enableDataSetEvents( true );

       System.out.println( "" );
     }
    catch( Exception exx )
     {
     }
   }
  class TUpdReturn
   {
     TUpdReturn()
      {
        m_lInternalRow = -1; m_dsNew = null; m_exc = null;
      }
     public DataSetData m_dsNew;
     public DataSetException m_exc;
     public long m_lInternalRow;
   }
  private void MkChainErrors( DataSet errorDataSet, TUpdReturn urRet ) throws DataSetException
   {
     m_vErrors = new TErrorItem[ errorDataSet.getRowCount() ];

     errorDataSet.first();
     ExceptionChain chain = new ExceptionChain();
     int iCnt = 0;
   try {
      do {
        m_vErrors[ iCnt++ ] = new TErrorItem(
          (Exception)errorDataSet.getObject(ResolverHelp.sException),
          errorDataSet.getLong( ResolverHelp.iIdx_INTERNALROW ) );

        chain.append( (Exception)errorDataSet.getObject(ResolverHelp.sException) );
      } while( errorDataSet.next() );

      errorDataSet.first();
      urRet.m_lInternalRow = errorDataSet.getLong( ResolverHelp.iIdx_INTERNALROW );
   }
   catch( Exception ex ) {
     DataSetException.handleException( ex, true );
   }

   urRet.m_exc =
     new DataSetException(  //DataSetException.GENERIC_ERROR,
       DataSetException.EXCEPTION_CHAIN,
       "Errors fired while saving changes", chain );
   }
  private long CalcMaxIntRow( DataSet[] arr ) throws DataSetException
   {
     long lmax = 0;
     for( int i = arr.length - 1; i >= 0; --i )
      if( arr[i] != null )
       {
         arr[i].first();
         for( int k = arr[i].getRowCount(); k > 0; --k )
          {
            lmax = Math.max( lmax, arr[i].getInternalRow() );
            arr[i].next();
          }
         arr[i].first();
       }
     return lmax;
   }
  private int ConvertOpToStat( DataSet ds )  throws DataSetException
   {
     if( ds.getInt(ResolverHelp.iIdx_OPERACION) == ResolverHelp.iInsert )
       return RowStatus.INSERTED|RowStatus.LOADED;
     if( ds.getInt(ResolverHelp.iIdx_OPERACION) == ResolverHelp.iUpdate )
       return RowStatus.UPDATED|RowStatus.LOADED;

     throw new DataSetException( 0, "RESOLVER ERROR: ConvertOpToStat receive invalid OP code. Contact with software VENDOR." );
   }
  private boolean FindIR( DataSet ds, long lIntRow, int iClmIndexOfInternalRow ) throws DataSetException
   {                                                                             //column 'INTERNALROW' is the last in 'dsFrom'
     if( ds == null ) return false;                                              //only  in 'errorDataSet' it is first
     ds.first();

     if( iClmIndexOfInternalRow == -1 )
       iClmIndexOfInternalRow = ds.getColumnCount() - 1;

     for( int i = ds.getRowCount(); i > 0; --i )
       if( ds.getLong(iClmIndexOfInternalRow) == lIntRow )
         return true;
       else ds.next();

     return false;
   }
  class TFakeCancel implements LoadCancel
   {
     public void cancelLoad() {
      }
   }
  private void MkLoadRow( Variant[] vvt, ReadWriteRow dsFrom, StorageDataSet sdsTo, int iStatus ) throws DataSetException
   {
     for( int k = 0; k < vvt.length; ++k ) //column 'INTERNALROW' is the last in 'dsFrom'
      {                                    //only  in 'errorDataSet' it is first
        Variant v = new Variant();
        dsFrom.getVariant( k, v );
        vvt[ k ].setVariant( v );
      }
     sdsTo.loadRow( iStatus );
   }
  private TUpdReturn FullUpdating( DataSet dataSet, DataSetData resyncs, DataSetData errors ) throws DataSetException
   {
     TUpdReturn urRet = this.new TUpdReturn();

     dataSet.enableDataSetEvents( false );
     try {
        DataSetView dsDel = new DataSetView();
        dsDel.setStorageDataSet( dataSet.getStorageDataSet() );
        dataSet.getStorageDataSet().getDeletedRows( dsDel );
        dsDel.open();

        DataRow rwOriginal = new DataRow( dataSet );

        TableDataSet dsNew = new TableDataSet();
        dsNew.setColumns( dataSet.getStorageDataSet().cloneColumns() );
        try {
           dsNew.open();

           TableDataSet errorDataSet = null, resDataSet = null;
           try {
             if( errors != null )
              {
                errorDataSet = new TableDataSet();
                errors.loadDataSet( errorDataSet );
                MkChainErrors( errorDataSet, urRet );
              }
             if( resyncs != null )
              {
                resDataSet = new TableDataSet();
                resyncs.loadDataSet( resDataSet );
              }

             long lMaxInternalRow = CalcMaxIntRow(
                new DataSet[]{ dataSet, dsDel }
              );

             Variant[] vvt = dsNew.startLoading( this.new TFakeCancel(), RowStatus.LOADED, false );
             try {
               for( int i = 1; i <= lMaxInternalRow; ++i )
                {
                  DataSet dsFrom;
                  int iStat;

                  if( FindIR(resDataSet, (long)i, -1) )// FindIR - moves to founded record
                   {
                     dsFrom = resDataSet; iStat = RowStatus.LOADED;
                   }
                  else if( FindIR(errorDataSet, (long)i, 0) )
                   {
                     if( errorDataSet.getInt(ResolverHelp.iIdx_OPERACION) == ResolverHelp.iDelete )
                      {
                        dsDel.goToInternalRow(i);
                        dsFrom = dsDel;
                        iStat = RowStatus.LOADED;
                      }
                     else
                      {
                        dsFrom = dataSet;
                        if( !dataSet.goToInternalRow(i) ) continue;//my logic error !!!
                          //throw new DataSetException( 0, "RESOLVER ERROR: cann't find internal error in dataSet. Contact with software VENDOR." );
                        iStat = ConvertOpToStat( errorDataSet );
                      }
                   }
                  else
                   {
                     dsFrom = dataSet;
                     if( !dataSet.goToInternalRow(i) ) continue;//my logic error !!!
                       //throw new DataSetException( 0, "RESOLVER ERROR: cann't find internal error in dataSet. Contact with software VENDOR." );
                     iStat = RowStatus.LOADED;
                   }

                  if( (iStat & RowStatus.UPDATED) != 0 )
                   {
                     dsFrom.getStorageDataSet().getOriginalRow( dsFrom, rwOriginal );
                     MkLoadRow( vvt, rwOriginal, dsNew, RowStatus.ORIGINAL|RowStatus.LOADED );
                   }
                  MkLoadRow( vvt, dsFrom, dsNew, iStat );
                }//for
               }//try
              finally {
                dsNew.endLoading();
              }
            }//try
           finally
            {
              if( errorDataSet != null ) errorDataSet.close();
              if( resDataSet != null ) resDataSet.close();
              dsNew.close();
            }
           urRet.m_dsNew = DataSetData.extractDataSet( dsNew );
         }
        finally { dsDel.close(); }
      }
     finally { dataSet.enableDataSetEvents( true ); }

     return urRet;
   }
  protected TUpdReturn MkClientDSUpdate( DataSet dataSet, DataSetData resyncs, DataSetData errors )
   {
     try {
       if( resyncs == null )
        {
          if( errors == null )
           {
             dataSet.resetPendingStatus( true ); return null;
           }
          TUpdReturn urHan = handleErrors( dataSet, errors );
          if( urHan != null ) return urHan;
        }
       return FullUpdating( dataSet, resyncs, errors );
      }
     catch( Exception ex ) {
       DataSetException.handleException( ex, true );
     }
    try {
       dataSet.resetPendingStatus( false );
     } catch( DataSetException err ) {}

    return null;
   }
  public void resolveData( DataSet dataSet ) throws DataSetException
   {
      if( dataCtrlIntf == null ) return;

      if( B_TRACE ) m_tim.start();

      TUpdReturn urRet = null;
      long lRowKey = dataSet.getInternalRow();

      try {
         DumpDS( dataSet, "DataSet dataSet before resolving:" );

         ProviderHelp.startResolution( dataSet.getStorageDataSet(), true );
         //DumpDS( dataSet, "DataSet dataSet after start resolution:" );

         Cursor csrOld = m_cPar.getCursor();
         m_cPar.setCursor( new Cursor(Cursor.WAIT_CURSOR) );
         DataSetData changes;
         DataSetData errors, resyncs;
         DataSetData[] dsArrRet;
         try {
           changes = DataSetData.extractDataSetChanges( dataSet );
           if( B_TRACE )
             System.out.println( "Extract changes: " + m_tim.startF() );  
           dsArrRet = dataCtrlIntf.IMD_ResolveChanges( nameQryOnServer, changes, identityDetectMAX );

           if( dsArrRet == null )
            {
              if( m_cPar instanceof JApplet )
                JOptionPane.showMessageDialog(
                 ((JApplet)m_cPar).getContentPane(), "Nothing to save",
                 "Information", JOptionPane.INFORMATION_MESSAGE );
              return;
            }
           errors = dsArrRet[ 1 ]; resyncs = dsArrRet[ 0 ];
           DumpDS( dataSet, "DataSet dataSet after extractDataSetChanges&IMD_ResolveChanges:" );
          }
         finally
          {
            m_cPar.setCursor( csrOld );
            
            if( B_TRACE )
              System.out.println( "Save changes on server: " + m_tim.startF() );
          }

         if( errors == null ) m_vErrors = null;

         urRet = MkClientDSUpdate( dataSet, resyncs, errors );
         if( urRet == null )
          {
            DumpDS( dataSet, "DataSet dataSet after return from MkClientDSUpdate() with null result" );
            return;
          }
      }
      catch( DataSetException ex ) {
         dataSet.resetPendingStatus( false );
         ex.printStackTrace();
         throw ex;
      }
      catch( Exception ex ) {
         ex.printStackTrace();
         dataSet.resetPendingStatus( false );
         String error = ex.getMessage();
         if( error == null ) error = "No connected to server";
         throw new DataSetException( 0, "caught in resolveData", ex );
      }
      finally {
         ProviderHelp.endResolution( dataSet.getStorageDataSet() );
      }

     if( urRet.m_dsNew != null )
      {
        dataSet.enableDataSetEvents( false );
        try {
          dataSet.emptyAllRows();
          urRet.m_dsNew.loadDataSet( dataSet );

          if( B_TRACE )
            System.out.println( "Reload client DataSet: " + m_tim.startF() );
         }
        finally
         {
           dataSet.enableDataSetEvents( true );
           dataSet.goToInternalRow( lRowKey );
         }
        DumpDS( dataSet, "DataSet dataSet after urRet.m_dsNew.loadDataSet( dataSet );" );
      }
     else
       DumpDS( dataSet, "DataSet dataSet after MkClientDSUpdate() with no loading NEW" );

     if( urRet.m_lInternalRow != -1 ) dataSet.goToInternalRow( urRet.m_lInternalRow );
     if( urRet.m_exc != null ) throw urRet.m_exc;
  }

  private TUpdReturn handleErrors( DataSet dataSet, DataSetData errors ) throws DataSetException
   {
      TableDataSet errorDataSet = new TableDataSet();
      errors.loadDataSet( errorDataSet );
      long internalRow = -1;
      errorDataSet.first();

      ExceptionChain chain = new ExceptionChain();

      DumpDS( dataSet, "DataSet dataSet before ass. in cycle resetPendingStatus( internalRow, false ); in HANDLE:" );

      m_vErrors = new TErrorItem[ errorDataSet.getRowCount() ];
      int iCnt = 0;
   try {
      do {
        if( errorDataSet.getInt(ResolverHelp.iIdx_OPERACION) ==
            ResolverHelp.iDelete
          )
          {
            m_vErrors = null;
            errorDataSet.close(); return null;
          }

        m_vErrors[ iCnt++ ] = new TErrorItem(
          (Exception)errorDataSet.getObject(ResolverHelp.sException),
          errorDataSet.getLong( ResolverHelp.iIdx_INTERNALROW ) );

        internalRow = errorDataSet.getLong( ResolverHelp.iIdx_INTERNALROW );
        dataSet.resetPendingStatus( internalRow, false );

        chain.append( (Exception)errorDataSet.getObject( ResolverHelp.sException ) );

      } while( errorDataSet.next() );


      DumpDS( dataSet, "DataSet dataSet before resetPendingStatus( false ) in HANDLE:" );
      dataSet.resetPendingStatus( true );
      DumpDS( dataSet, "DataSet dataSet after resetPendingStatus( true ) in HANDLE:" );

      errorDataSet.first();
      internalRow = errorDataSet.getLong( ResolverHelp.iIdx_INTERNALROW );
      //dataSet.goToInternalRow( internalRow );
   }
   catch( Exception ex ) {
     DataSetException.handleException( ex, true );
   }

   TUpdReturn urRet = this.new TUpdReturn();
   urRet.m_dsNew = null;
   urRet.m_exc =
     new DataSetException(  DataSetException.GENERIC_ERROR,
     //DataSetException.EXCEPTION_CHAIN,
     "Errors fired while saving changes", chain );
   urRet.m_lInternalRow = internalRow;

   return urRet;
  }
/*
  private void handleResyncs( DataSet dataSet, DataSetData resyncs ) throws DataSetException
   {
      TableDataSet resyncDataSet = new TableDataSet();
      resyncs.loadDataSet( resyncDataSet );
      long internalRow = dataSet.getInternalRow();

      resyncDataSet.first();

      DumpDS( dataSet, "DataSet dataSet before cycle in RESYNC:" );

      DataRow rwTmp = new DataRow( dataSet );
   try {
      do {

        long iRow = resyncDataSet.getLong( ResolverHelp.sInternalRow );
        //dataSet.resetPendingStatus( iRow, true );

        dataSet.goToInternalRow( iRow );
        resyncDataSet.copyTo( rwTmp );
        dataSet.editRow();
        dataSet.updateRow( rwTmp );
        dataSet.post();
        dataSet.resetPendingStatus( iRow, false );
        //dataSet.resetPendingStatus( iRow, true );
      } while( resyncDataSet.next() );
    }
   finally { dataSet.goToInternalRow(internalRow); }

   DumpDS( dataSet, "DataSet dataSet after cycle in RESYNC:" );
  }*/

  private IMDataControl dataCtrlIntf = null;
  private String nameQryOnServer = null;
  private boolean identityDetectMAX = false;

  public boolean getIdentityDetectMAX() { return identityDetectMAX; }
  public void setIdentityDetectMAX( boolean bFl ) { identityDetectMAX = bFl; }
  public IMDataControl getDataCtrlIntf() { return dataCtrlIntf; }
  public String getNameQryOnServer() { return nameQryOnServer; }
  public void setDataCtrlIntf( IMDataControl dc ) { dataCtrlIntf = dc; }
  public void setNameQryOnServer( String s ) { nameQryOnServer = s; }
}
