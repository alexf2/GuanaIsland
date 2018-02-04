package G_APPL;

import borland.jbcl.dataset.DataSetData;
import borland.jbcl.dataset.DataSetException;
import java.rmi.Remote;
import java.rmi.RemoteException;


public interface IGServer extends Remote {
  IRefController ConnectClient( java.rmi.dgc.VMID vmVMID ) throws RemoteException;
}
 