package G_APPL;

import borland.sql.dataset.*;
import borland.jbcl.dataset.*;
import borland.jbcl.util.*;
import borland.jbcl.control.*;
import java.rmi.*;
import java.net.*;
import java.rmi.server.*;
import java.util.*;
import java.text.MessageFormat;



public class GServer extends UnicastRemoteObject
  implements IGServer, IRef_Response {

  public void ResponseClearRef( RefController rcCtrl )
   {
     if( rcCtrl.getNum() < 0 ) return;
     synchronized( frame.list1 )
      {
        frame.list1.remove( rcCtrl.getNum() );
      }
   }
  public int ResponseMakeRef( RefController rcCtrl )
   {
     int iRet = 0;
     try {
       String sAdd = MessageFormat.format(
             " {2,date} {2,time} \"{0}\" ({1})  #{3}",
             new Object[] {
               getClientHost(),
               InetAddress.getByName(getClientHost()).getHostAddress(),
               new Date( System.currentTimeMillis() ),
               rcCtrl.getVMID().toString()
             } );
       synchronized( frame.list1 )
        {
          frame.list1.add( sAdd, -1 );
          iRet = frame.list1.getItemCount();
        }
       }
     catch( ServerNotActiveException ex )
      {
        ex.printStackTrace();
      }
     catch( java.net.UnknownHostException ex )
      {
        ex.printStackTrace();
      }
     return iRet - 1;
   }
  public IRefController ConnectClient( java.rmi.dgc.VMID vmVMID ) throws RemoteException
   {
     return new RefController( this, vmVMID, dbServer );
   }

  public GServer() throws RemoteException {
    frame = new DataServerFrame( this );
    frame.pack();
    frame.setVisible( true );
  }

  public static void main( String[] args ) {
    System.setSecurityManager( new RMISecurityManager() );
    TimeZone.setDefault( TimeZone.getTimeZone("EAT") );
    GServer gsApp = null;
    try {
      gsApp = new GServer();
      gsApp.MkParseArgs( args );
      gsApp.bindToNamingService();
    }
    catch( Exception ex ) {
      ex.printStackTrace();
      DataSetException.handleException( ex, true );
      System.exit( 1 );
    }
  }

  void bindToNamingService() throws Exception {

    InetAddress address = InetAddress.getLocalHost();
    String thisHost = address.getHostAddress();
    Properties currentProperties = System.getProperties();
    currentProperties.put( "java.rmi.server.hostname", thisHost );
//currentProperties.put( "java.rmi.server.codebase", "file:/f:\\work\\G_APPL/" );
    System.setProperties( currentProperties );

    serviceURL = "rmi://" + thisHost + "/GServer";

    frame.setTitle( "Guana server1 \"" + serviceURL + "\"" );

    try {
      Naming.bind( serviceURL, this );
    }
    catch( java.rmi.AlreadyBoundException ex ) {
      ex.printStackTrace();
      MessageDialog dlg = new MessageDialog( frame,
        "Error", "Another server is already registered, Do you want to override that server?", ButtonDialog.YES_NO );
      dlg.setVisible( true );
      if( dlg.getResult() == ButtonDialog.YES ) {
        Naming.rebind( serviceURL, this );
      }
    }
    catch( java.rmi.ConnectException ex ) {
      ex.printStackTrace();
      ExceptionChain chain = new ExceptionChain();
      chain.append( ex );
      throw new DataSetException(
        DataSetException.GENERIC_ERROR, "Connection problem. Make sure to run rmiregistry from the \r\n jbuilder\\java\\bin directory first.", chain );
    }
  }

  void closing() {
    try {
      Naming.unbind( serviceURL );
    }
    catch( Exception ex ) {
      ex.printStackTrace();
    }
  }

  private void MkParseArgs( String[] args ) throws Exception
   {
      final String[]  arrArgs =  {
        "GSERVER"
      };

     for( int i = args.length - 1; i >= 0; --i )
      {
        String sCmp = (new String(args[i])).toUpperCase();
        int j = arrArgs.length - 1;
        for( ; j >= 0; --j )
          if( sCmp.indexOf(arrArgs[j]) == 0 ) break;
        if( j < -1 )
          throw new Exception( "In command line bad parameter: " + "\"" + args[i] + "\"" );

        switch( j )
         {
           case 0:
            {
              int k = args[i].indexOf( '=' );
              if( k == -1 )
                throw new Exception( "In command line bad arguments at: " + "\"" + args[i] + "\" - needing =" );

              try {
                dbServer = args[i].substring( k + 1, args[i].length() );
              }
              catch( StringIndexOutOfBoundsException ex )
               {
                 ex.printStackTrace();
                 throw new Exception( "In command line bad arguments at: " + "\"" + args[i] + "\" - needing = name_of_server" );
               }
              dbServer.trim();
            }
         }
      }
   }

  private String serviceURL;
  private DataServerFrame frame;
  private String dbServer = "SERVER1";
}

