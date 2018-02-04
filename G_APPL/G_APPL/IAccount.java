package G_APPL;

import borland.jbcl.dataset.*;

import java.rmi.*;
import java.io.Serializable;


public interface IAccount extends Remote
 {
   void IAcc_PayEnrol( int iBILL_NO, boolean bFlEnrol ) throws RemoteException, DataSetException;
 }

