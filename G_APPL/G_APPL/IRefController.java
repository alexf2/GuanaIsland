package G_APPL;

import java.rmi.*;
import java.rmi.server.*;

public interface IRefController extends Remote
 {
   void ClearRef() throws RemoteException;
   //RemoteObject GetInterface( String sName ) throws RemoteException;
 }

