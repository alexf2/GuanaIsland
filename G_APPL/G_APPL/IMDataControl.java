package G_APPL;

import java.awt.*;
import java.awt.event.*;
import borland.jbcl.layout.*;
import borland.jbcl.control.*;
import borland.jbcl.dataset.*;

import java.rmi.*;

import java.io.Serializable;


class TDataRet implements Serializable
 {
   TDataRet()
    {
      m_iReturnedPage = 0;
      m_iPages = 1;
    }
   public DataSetData m_ds;
   public int m_iPages;
   public int m_iReturnedPage;
   public TParameterRow_ValTransfer m_tpvPR;
 }

public interface IMDataControl extends Remote
 {
   void IMD_OpenCnct( String sName, String sPwd, String sObj  ) throws RemoteException, DataSetException;
   void IMD_CloseCnct( String sObj ) throws RemoteException, DataSetException;

   Column[] IMD_GetColumns( String sObj )  throws RemoteException, DataSetException;

   TDataRet IMD_ProvideData(
      String sQryName,
      int iPageSize,
      int iPage,
      boolean bCalcPages,
      TParameterRow_ValTransfer tpvPR ) throws RemoteException, DataSetException;

   DataSetData[] IMD_ResolveChanges(
      String sQryName,
      DataSetData changes,
      boolean bIdentityMAX ) throws RemoteException, DataSetException;
   //0 - refreshed ds.; 1 - errors or null

   ParameterRow IMD_GetParameterRow( String sStorageName ) throws RemoteException, DataSetException;

   DataSetData IMD_ProvideDataAll( String sQryName )  throws RemoteException, DataSetException;

   void IMD_SetQueryText( String sQryName, String sDBName, String sQryText )  throws RemoteException, DataSetException;
 }


