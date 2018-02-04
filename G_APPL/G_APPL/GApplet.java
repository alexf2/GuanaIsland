package G_APPL;

import java.awt.*;
import java.awt.event.*;
import java.applet.*;
import borland.jbcl.layout.*;
import borland.jbcl.control.*;
import com.sun.java.swing.*;
import borland.jbcl.dataset.*;

import borland.sql.dataset.*;
import borland.jbcl.dataset.*;
import borland.jbcl.util.*;
import borland.jbcl.control.*;
import java.rmi.*;
import java.net.InetAddress;
import java.util.*;
import java.lang.reflect.*;
//import pvWinJ.*;
import java.sql.Timestamp;
import java.util.Calendar;
import java.math.BigDecimal;
import borland.jbcl.view.*;
import java.lang.*;
import borland.jbcl.model.*;


//import com.sun.java.swing.UIManager;
public class GApplet extends JApplet {

  public static final  boolean B_TRACE = true;
  private TIntervalTimer m_tim = B_TRACE ? new TIntervalTimer():null;

  IGServer m_igServer = null;
  IRefController m_irRefControl = null;

  private boolean m_bInit = false;

  int m_iKeyID_GUEST = 0, m_iKeyBILL_NO = 0;
  boolean isStandalone = false;
  JPanel jPanel1 = new JPanel();
  FlowLayout flowLayout1 = new FlowLayout();
  CardLayout cardLayout1 = new CardLayout();
  Panel bpGuest = new Panel();
  Panel bpBill = new Panel();
  Panel bpLists = new Panel();
  Panel bpTime = new Panel();
  Panel bpPayLists = new Panel();
  TableDataSet tbRoom = new TableDataSet();
  TPgProvider m_prRoom = new TPgProvider( this ), m_prGuest = new TPgProvider( this ), m_prGuestCat = new TPgProvider( this ), m_prBillCat = new TPgProvider( this ), m_prBill = new TPgProvider( this ), m_prRoomsUsed = new TPgProvider( this ), m_prAddCh = new TPgProvider( this ), m_prPayments = new TPgProvider( this );
  TAsyncAllProvider m_prLook_GuestCat = new TAsyncAllProvider( this ), m_prLook_Room = new TAsyncAllProvider( this ), m_prLook_BillCat = new TAsyncAllProvider( this );
  TResolver   m_resRoom =
    new TResolver( this ), m_resGuest = new TResolver( this ), m_resGuestCat = new TResolver( this ), m_resBillCat = new TResolver( this ), m_resBill = new TResolver( this ), m_resRoomsUsed = new TResolver( this ), m_resAddCh = new TResolver( this ), m_resPayments = new TResolver( this );
//Get a parameter value

  protected void headerItemClicked_Handle( HeaderEvent e, GridControl m_rGrd )
   {
     if( e.getID() != HeaderEvent.ITEM_CLICKED ) return;
     if( !(e.getSource() instanceof HeaderView) ) return;
     HeaderView hv = (HeaderView)e.getSource();
     if( hv != m_rGrd.getRowHeaderView() ) return;

     DataSet ds = m_rGrd.getDataSet();
     if( ds == null || !(ds instanceof StorageDataSet) ) return;
     Resolver res = ((StorageDataSet)ds).getResolver();
     if( res == null || !(res instanceof TResolver) ) return;

     TResolver rRes = (TResolver)res;

     int iInd = e.getIndex() + 1;
     TErrorItem it = rRes.FindErrorByInt( iInd );
     if( it == null || it.m_ex == null ) return;

     //if( it.m_ex instanceof DataSetException )
     DataSetException.handleException( this.getContentPane(), it.m_ex, true );

     /*JOptionPane.showMessageDialog(
       getContentPane(), it.m_ex.getMessage(),
       "In row #" + Integer.toString(iInd) + " fired follow error:",
       JOptionPane.ERROR_MESSAGE );*/
   }

 class TErrorCustomize implements borland.jbcl.view.CustomItemListener
  {
    GridControl m_rGrd;

    public TErrorCustomize( GridControl rGrd )
     {
       super(); m_rGrd = rGrd;
     }
    public void customizeItem( Object address, Object data, int state, CustomPaintSite site)
     {
       if( !(address instanceof MatrixLocation) ) return;
       if( (state&(ItemPainter.SELECTED|ItemPainter.FOCUSED|ItemPainter.DISABLED|ItemPainter.INDETERMINATE)) != 0 ) return;

       DataSet ds = m_rGrd.getDataSet();
       if( ds == null || !(ds instanceof StorageDataSet) ) return;
       Resolver res = ((StorageDataSet)ds).getResolver();
       if( res == null || !(res instanceof TResolver) ) return;

       TResolver rRes = (TResolver)res;

       int iRow = ((MatrixLocation)address).row + 1;
       /*long lRow;
       try {
         lRow = ds.getInternalRow();
        }
       catch( Exception ex )
        {
          return;
        }*/
       if( rRes.FindErrorByInt(iRow) == null ) return;

       site.setBackground( new Color(128,64,0) );
       site.setForeground( Color.yellow );
     }
  }

 void InitProviderResolver( TableDataSet ds, String sName, TPgProvider pr, TResolver res ) throws RemoteException, DataSetException
  {
    ds.close();

    MkBindMeta( ds, sName );

    pr.setSizePage( ds.getMaxRows() );
    pr.setCurPage( 0 );
    pr.setNameQryOnServer( sName );
    pr.setDataCtrlIntf( (IMDataControl)m_irRefControl );

    res.setNameQryOnServer( sName );
    res.setDataCtrlIntf( (IMDataControl)m_irRefControl );
    res.setIdentityDetectMAX( true );

    ds.setProvider( pr ); ds.setResolver( res );
    res.BindToPageProvider( pr );
  }
  private void SetGuestQuery() throws DataSetException, RemoteException
   {
     String sF = textFMask.getText().trim();
     String sL = textLMask.getText().trim();
     if( sF.length() == 0 ) sF = "%";
     if( sL.length() == 0 ) sL = "%";
     String sQ;
     if( sF == "%" && sL == "%" )
       sQ = "select * from Guest ORDER BY FNAME, LNAME";
     else sQ = "select * from Guest where [FNAME] like '" + sF +
       "' and [LNAME] like '" + sL + "'  ORDER BY FNAME, LNAME";

     ((IMDataControl)m_irRefControl).IMD_SetQueryText( "m_qGuest", "m_dbMain", sQ ) ;
   }
  void InitProviders() throws RemoteException, DataSetException
   {
     m_prLook_GuestCat.setDataCtrlIntf( (IMDataControl)m_irRefControl );
     m_prLook_GuestCat.setNameQryOnServer( "q_LookGuestCat" );
     tbLook_GuestCat.setProvider( m_prLook_GuestCat );

     m_prLook_Room.setDataCtrlIntf( (IMDataControl)m_irRefControl );
     m_prLook_Room.setNameQryOnServer( "m_qLook_Room" );
     tbLook_Room.setProvider( m_prLook_Room );

     m_prLook_BillCat.setDataCtrlIntf( (IMDataControl)m_irRefControl );
     m_prLook_BillCat.setNameQryOnServer( "m_qLook_PayCat" );
     tbLook_BillCat.setProvider( m_prLook_BillCat );

     InitProviderResolver( tbRoom, "m_qRoom", m_prRoom, m_resRoom );
     InitProviderResolver( tbGuest, "m_qGuest", m_prGuest, m_resGuest );
     //m_prGuest.getParamRow().setString( "fmask", GetMask(textFMask) );
     //m_prGuest.getParamRow().setString( "lmask", GetMask(textLMask) );
     SetGuestQuery();

     InitProviderResolver( tbGuestCat, "m_qGuestCat", m_prGuestCat, m_resGuestCat );
     InitProviderResolver( tbBillCat, "m_qBillCat", m_prBillCat, m_resBillCat );

     InitProviderResolver( tbBill, "m_qBill", m_prBill, m_resBill );

     InitProviderResolver( tbRoomsUsed, "m_qRoomsUsed", m_prRoomsUsed, m_resRoomsUsed );
     InitProviderResolver( tbAddCh, "m_qAddCh", m_prAddCh, m_resAddCh );
     InitProviderResolver( tbPayments, "m_qPayments", m_prPayments, m_resPayments );
                                  //*****

     PickListDescriptor pdTmp = new PickListDescriptor(
          tbLook_GuestCat,
          new String[] {"ID_CAT"},
          new String[] {"NAMECAT"},
          new String[] {"ID_CAT"},
          "NAMECAT", true );
     AssPickTo( tbGuest, "ID_CAT", pdTmp );

     pdTmp = new PickListDescriptor(
          tbLook_Room,
          new String[] {"ID_ROOM"},
          new String[] {"NAME"},
          new String[] {"ID_ROOM"},
          "NAME", true );
     AssPickTo( tbRoomsUsed, "ID_ROOM", pdTmp );

     pdTmp = new PickListDescriptor(
          tbLook_BillCat,
          new String[] {"ID_BILL"},
          new String[] {"NAME"},
          new String[] {"ID_BILL"},
          "NAME", true );
     AssPickTo( tbAddCh, "ID_BILL", pdTmp );

   }
  String GetMask( JTextField tx )
   {
     //return "'" + tx.getText() + "'";
     return tx.getText();
   }
  void AssPickTo( TableDataSet ds, String sClm, PickListDescriptor pd ) throws DataSetException
   {
     Column clm = ds.getColumn( sClm );
     clm.setPickList( pd );
   }
  public String getParameter(String key, String def) {
    return isStandalone ? System.getProperty(key, def) :
      (getParameter(key) != null ? getParameter(key) : def);
  }

  private void MkLogin() throws Exception
   {
     PanPwd pPwd = new PanPwd();

     int iRes = JOptionPane.showConfirmDialog(
       getContentPane(), pPwd, "Login", JOptionPane.YES_NO_OPTION,
       JOptionPane.QUESTION_MESSAGE );


     if( iRes != JOptionPane.YES_OPTION )
       throw new Exception( "Login canceled" );
     else
      {
        m_sUser = pPwd.txtLogin.getText();
        m_sPwnd = pPwd.txtPwd.getText();
      }
   }

  private void MkOpedDb( boolean bFl ) throws RemoteException, DataSetException
   {
     if( bFl == true )
      {
        ((IMDataControl)m_irRefControl).IMD_OpenCnct( m_sPwnd, m_sUser, "m_dbMain" );
        ((IMDataControl)m_irRefControl).IMD_OpenCnct( m_sPwnd, m_sUser, "m_dbLook" );
      }
     else
      {
        ((IMDataControl)m_irRefControl).IMD_CloseCnct( "m_dbMain" );
        ((IMDataControl)m_irRefControl).IMD_CloseCnct( "m_dbLook" );
      }
   }
  void MkBindMeta( TableDataSet ds, String qName ) throws RemoteException, DataSetException
   {
     Column[] clms = ((IMDataControl)m_irRefControl).IMD_GetColumns( qName );
     ds.close();
     ds.setColumns( clms );
   }

  //Construct the applet

  public GApplet() {
  }
//Initialize the applet

  public void init() {
    try {
      //System.setSecurityManager( new RMISecurityManager() );
      //setSize( new Dimension(400, 300) );
      MkLogin();
      //m_sUser = "sa"; m_sPwnd = "";
      jbInit();
      //doLayout();
      //getContentPane().doLayout();
      cardLayout1.show( bpGuest.getParent(), "bpGuest" );
    }
    catch (Exception e) {
      e.printStackTrace();
      JOptionPane.showMessageDialog(
        this.getContentPane(), e.getMessage(),
        "Error", JOptionPane.ERROR_MESSAGE );
    }
  }
  static {
    try {
      UIManager.setLookAndFeel(new com.sun.java.swing.plaf.metal.MetalLookAndFeel());
      //UIManager.setLookAndFeel(new com.sun.java.swing.plaf.motif.MotifLookAndFeel());
      //UIManager.setLookAndFeel(new com.sun.java.swing.plaf.windows.WindowsLookAndFeel());
    }
  catch( Exception e ) { e.printStackTrace(); }
    }
//Component initialization

  private void jbInit() throws Exception {
    this.setSize( new Dimension(700, 500) );
    jPanel1.setDoubleBuffered(false);
    jPanel1.setSize( new Dimension(700, 500) );

    bpBill.setLayout(borderLayout3);
    bpLists.setLayout(borderLayout2);

    bpGuest.setLayout(borderLayout1);

    //this.getContentPane().add( jPanel1, null );
    //this.setLayout( flowLayout1 );
    this.setContentPane(jPanel1);


    tbRoom.setMaxRows(25);
    tbRoom.setMetaDataUpdate(MetaDataUpdate.NONE);

    tbRoom.setTableName("");
    tbRoom.addEditListener(new borland.jbcl.dataset.EditAdapter() {
      public void deleted(DataSet dataSet) throws DataSetException {
         ClearErrorsInResolver( dataSet );
      }
      public void inserted(DataSet dataSet) throws DataSetException {
         ClearErrorsInResolver( dataSet );
      }
    });
    m_prRoom.setLstPages(listRoom);
    m_prGuest.setLstPages(listGuest);
    m_prGuestCat.setLstPages(listGuestCat);
    m_prBillCat.setNameQryOnServer("");
    m_prBillCat.setLstPages(listBillCat);
    m_prBill.setLstPages(listBill);
    m_prRoomsUsed.setLstPages(listRoomsUsed);
    m_prAddCh.setLstPages(listAddCh);
    m_prPayments.setLstPages(listPayments);


    tbLook_GuestCat.setTableName("");
    textTtGuest.setBackground(new Color(192, 210, 192));
    textTtGuest.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    textTtGuest.setForeground(Color.blue);
    textTtGuest.setFont(new Font("Helvetica", 3, 16));
    textTtGuest.setMargins(new Insets(0, 0, 5, 0));
    textTtGuest.setText("Guests");
    textTtGuest.setTransparent(false);
    jbGotoBill.setText("Guest bills>>");
    jbGotoBill.setFont(new Font("Dialog", 1, 11));
    jbGotoBill.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jbGotoBill_actionPerformed(e);
      }
    });
    jbGotoLists.setText("Lists");
    jbGotoLists.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jbGotoLists_actionPerformed(e);
      }
    });
    jbGotoTime.setText("Time report");
    gridGuest.setDataSet(tbGuest);
    gridGuest.setDoubleBuffered(false);
    gridGuest.setNavigateOnTab(false);
    gridGuest.setSortOnHeaderClick(false);
    gridGuest.addHeaderListener(new borland.jbcl.view.HeaderAdapter() {
      public void headerItemClicked(HeaderEvent e) {
        gridGuest_headerItemClicked(e);
      }
    });



    gridGuest.addCustomItemListener( this.new TErrorCustomize(gridGuest) );

    navGuest.setButtonOrientation(borland.jbcl.util.Orientation.HORIZONTAL);
    navGuest.setDataSet(tbGuest);
    navGuest.setShowRollover(true);
    navGuest.setToolTipText("");
    listGuest.setFont(new Font("Dialog", 1, 14));
    listGuest.setBackground(SystemColor.info);
    btnApplyMask.setLabel("Apply mask");
    btnApplyMask.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        btnApplyMask_actionPerformed(e);
      }
    });
    textControl1.setText("First name");
    textControl2.setText("Last Name");
    textControl3.setText("Address");
    textControl4.setText("City");
    textControl5.setText("State");
    fcPostal.setAlignment(borland.jbcl.util.Alignment.RIGHT | borland.jbcl.util.Alignment.MIDDLE);
    fcPostal.setDataSet(tbGuest);
    fcPostal.setColumnName("POST");
    textControl6.setText("Postal Code");
    fcDeposit.setDataSet(tbGuest);
    fcDeposit.setColumnName("DEPOSIT");
    textControl7.setText("Deposit ($)");
    fcStatus.setDataSet(tbGuest);
    fcStatus.setColumnName("ID_CAT");
    textControl8.setText("Status");
    tbGuest.setMetaDataUpdate(MetaDataUpdate.NONE);

    tbGuest.setTableName("Guest");
    tbGuest.addEditListener(new borland.jbcl.dataset.EditAdapter() {
      public void inserted(DataSet dataSet) throws DataSetException {
        ClearErrorsInResolver( dataSet );
        tbGuest_inserted(dataSet);
      }
      public void addError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbGuest_addError(dataSet, row, ex, response);
      }
      public void updateError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbGuest_updateError(dataSet, row, ex, response);
      }
      public void deleteError(DataSet dataSet, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbGuest_deleteError(dataSet, ex, response);
      }
      public void editError(DataSet dataSet, Column column, Variant value, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbGuest_editError(dataSet, column, value, ex, response);
      }
      public void deleted(DataSet dataSet) throws DataSetException {
        ClearErrorsInResolver( dataSet );
      }
    });
    fcFNAME.setColumnName("FNAME");
    fcFNAME.setDataSet(tbGuest);
    fcADDRESS.setColumnName("ADDRESS");
    fcADDRESS.setDataSet(tbGuest);
    fcCITY.setDataSet(tbGuest);
    fcCITY.setColumnName("CITY");
    textFMask.setText("%");
    textFMask.setFont(new Font("Dialog", 1, 12));
    textLMask.setText("%");
    textLMask.setFont(new Font("Dialog", 1, 12));
    textTtGuest1.setBackground(new Color(192, 210, 192));
    textTtGuest1.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    textTtGuest1.setForeground(Color.blue);
    textTtGuest1.setFont(new Font("Helvetica", 3, 16));
    textTtGuest1.setMargins(new Insets(0, 0, 5, 0));
    textTtGuest1.setText("Lists");
    textTtGuest1.setTransparent(false);
    bevelPanel2.setDoubleBuffered(false);
    bevelPanel2.setTextureName("");
    jPanel3.setPreferredSize(new Dimension(10, 40));
    jPanel3.setBackground(new Color(192, 210, 192));
    jPanel3.setDoubleBuffered(false);
    jPanel3.setLayout(xYLayout3);
    textControl9.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    textControl9.setForeground(Color.blue);
    textControl9.setFont(new Font("Helvetica", 3, 12));
    textControl9.setText("List of all rooms");

    gridRoom.setDataSet(tbRoom);
    gridRoom.setDoubleBuffered(false);
    gridRoom.setNavigateOnTab(false);
    gridRoom.setSortOnHeaderClick(false);
    gridRoom.addHeaderListener(new borland.jbcl.view.HeaderAdapter() {
      public void headerItemClicked(HeaderEvent e) {
        gridRoom_headerItemClicked(e);
      }
    });

    gridRoom.addCustomItemListener( this.new TErrorCustomize(gridRoom) );


    listRoom.setFont(new Font("Dialog", 1, 14));
    gridGuestCats.setDataSet(tbGuestCat);
    gridGuestCats.setDoubleBuffered(false);
    gridGuestCats.setNavigateOnTab(false);
    gridGuestCats.setSortOnHeaderClick(false);
    gridGuestCats.addHeaderListener(new borland.jbcl.view.HeaderAdapter() {
      public void headerItemClicked(HeaderEvent e) {
        gridGuestCats_headerItemClicked(e);
      }
    });

    gridGuestCats.addCustomItemListener( this.new TErrorCustomize(gridGuestCats) );


    gridBillCat.setDataSet(tbBillCat);
    gridBillCat.setDoubleBuffered(false);
    gridBillCat.setNavigateOnTab(false);
    gridBillCat.setSortOnHeaderClick(false);
    gridBillCat.addHeaderListener(new borland.jbcl.view.HeaderAdapter() {
      public void headerItemClicked(HeaderEvent e) {
        gridBillCat_headerItemClicked(e);
      }
    });

    gridBillCat.addCustomItemListener( this.new TErrorCustomize(gridBillCat) );


    navRoom.setDataSet(tbRoom);
    navRoom.setShowRollover(true);
    jbCloseLists.setText("Guest bills>>");
    jbCloseLists.setForeground(new Color(200, 0, 0));
    jbCloseLists.setLabel("Close");
    jbCloseLists.setFont(new Font("Dialog", 1, 11));
    jbCloseLists.setOpaque(true);
    jbCloseLists.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jbCloseLists_actionPerformed(e);
      }
    });
    tbGuestCat.setMetaDataUpdate(MetaDataUpdate.NONE);
    tbGuestCat.setTableName("CategoryGuest");
    tbGuestCat.addEditListener(new borland.jbcl.dataset.EditAdapter() {
      public void deleted(DataSet dataSet) throws DataSetException {
        ClearErrorsInResolver( dataSet );
      }
      public void inserted(DataSet dataSet) throws DataSetException {
        ClearErrorsInResolver( dataSet );
      }
    });
    tbBillCat.setMetaDataUpdate(MetaDataUpdate.NONE);
    tbBillCat.setMaxRows(25);
    tbBillCat.setTableName("BillCategory");
    tbBillCat.addEditListener(new borland.jbcl.dataset.EditAdapter() {
      public void deleted(DataSet dataSet) throws DataSetException {
         ClearErrorsInResolver( dataSet );
      }
      public void inserted(DataSet dataSet) throws DataSetException {
         ClearErrorsInResolver( dataSet );
      }
    });

    txtTtlBill.setBackground(new Color(192, 210, 192));
    txtTtlBill.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    txtTtlBill.setForeground(Color.blue);
    txtTtlBill.setFont(new Font("Helvetica", 3, 16));
    txtTtlBill.setMargins(new Insets(0, 0, 5, 0));
    txtTtlBill.setText("Bills of");
    txtTtlBill.setTransparent(false);
    bevelPanel3.setDoubleBuffered(false);
    bevelPanel3.setTextureName("");
    jPanel4.setPreferredSize(new Dimension(10, 40));
    jPanel4.setBackground(new Color(192, 210, 192));
    jPanel4.setDoubleBuffered(false);

    gridBill.setDataSet(tbBill);
    gridBill.setDoubleBuffered(false);
    gridBill.setNavigateOnTab(false);

    gridBill.setSortOnHeaderClick(false);
    gridBill.addHeaderListener(new borland.jbcl.view.HeaderAdapter() {
      public void headerItemClicked(HeaderEvent e) {
        gridBill_headerItemClicked(e);
      }
    });
    gridBill.addCustomItemListener( this.new TErrorCustomize(gridBill) );


    jPanel4.setLayout(xYLayout5);
    textControl12.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    textControl12.setForeground(Color.blue);
    textControl12.setFont(new Font("Helvetica", 3, 12));
    textControl12.setText("Bills");
    navBill.setDataSet(tbBill);
    navBill.setShowRollover(true);
    listBill.setFont(new Font("Dialog", 1, 14));
    fcBILL_NO.setDataSet(tbBill);
    fcBILL_NO.setColumnName("BILL_NO");
    fcBILL_NO.setReadOnly(true);
    fcBILL_NO.setText("fcFNAME1");
    textControl14.setText("Folio");
    fcFOLIO.setDataSet(tbBill);
    fcFOLIO.setColumnName("FOLIO");
    textAreaComments.setColumnName("COMMENTS");
    textAreaComments.setDataSet(tbBill);
    fcHEARD_OF_GI.setDataSet(tbBill);
    fcHEARD_OF_GI.setColumnName("HEARD_OF_GI");
    textControl16.setText("Date");
    textControl17.setText("Heard of GI:");
    fcDATE_.setDataSet(tbBill);
    fcDATE_.setColumnName("DATE_");
    textControl18.setText("Details");
    groupBox1.setLayout(xYLayout4);
    groupBox1.setForeground(new Color(64, 128, 128));
    groupBox1.setFont(new Font("Dialog", 1, 12));
    groupBox1.setLabel("Summarize");
    fc_TOTAL_ROOMSPAY.setText("fcBILL_NO2");
    textControl19.setForeground(Color.black);
    textControl19.setFont(new Font("Dialog", 0, 12));
    textControl19.setText("Total rooms");
    fc_TAX_AND_SERVICE.setDataSet(tbBill);
    fc_TAX_AND_SERVICE.setColumnName("_TAX_AND_SERVICE");
    fc_TAX_AND_SERVICE.setReadOnly(true);
    fc_TAX_AND_SERVICE.setText("fcBILL_NO3");
    textControl110.setForeground(Color.black);
    textControl110.setFont(new Font("Dialog", 0, 12));
    textControl110.setText("Tax + Service");
    textControl111.setForeground(Color.black);
    textControl111.setFont(new Font("Dialog", 0, 12));
    textControl111.setText("Subtotal");
    fc_SUBTOTAL.setText("fcBILL_NO4");
    textControl112.setForeground(Color.black);
    textControl112.setFont(new Font("Dialog", 0, 12));
    textControl112.setText("Deposit received");
    fc_DEPOSIT_RECIEVED.setDataSet(tbBill);
    fc_DEPOSIT_RECIEVED.setColumnName("_DEPOSIT_RECIEVED");
    fc_DEPOSIT_RECIEVED.setReadOnly(true);
    fc_DEPOSIT_RECIEVED.setText("fcBILL_NO5");
    textControl113.setForeground(Color.black);
    textControl113.setFont(new Font("Dialog", 0, 12));
    textControl113.setText("Payments:");
    textControl114.setForeground(Color.black);
    textControl114.setFont(new Font("Dialog", 0, 12));
    textControl114.setText("Total:");
    textControl115.setForeground(Color.black);
    textControl115.setFont(new Font("Dialog", 0, 12));
    textControl115.setText("Balance Due:");
    fc_TOTAL_ADDITIONAL.setText("fcBILL_NO6");
    fc_TOTAL_PAYMENTS.setDataSet(tbBill);
    fc_TOTAL_PAYMENTS.setColumnName("_TOTAL_PAYMENTS");
    fc_TOTAL_PAYMENTS.setReadOnly(true);
    fc_TOTAL_PAYMENTS.setText("fcBILL_NO7");
    fc_TOTAL.setText("fcBILL_NO8");
    fc_BALANCE_DUE.setDataSet(tbBill);
    fc_BALANCE_DUE.setColumnName("_BALANCE_DUE");
    fc_BALANCE_DUE.setReadOnly(true);
    fc_BALANCE_DUE.setText("fcBILL_NO9");
    jbPayLists.setText("Pay Lists>>");
    jbPayLists.setFont(new Font("Dialog", 1, 11));
    jbPayLists.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jbPayLists_actionPerformed(e);
      }
    });
    jbCloseBill.setText("Close");
    jbCloseBill.setForeground(new Color(200, 0, 0));
    jbCloseBill.setFont(new Font("Dialog", 1, 11));
    jbCloseBill.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jbCloseBill_actionPerformed(e);
      }
    });
    tbBill.setMaxRows(25);
    tbBill.setMetaDataUpdate(MetaDataUpdate.NONE);
    tbBill.addNavigationListener(new borland.jbcl.dataset.NavigationListener() {
      public void navigated(NavigationEvent e) {
        tbBill_navigated(e);
      }
    });
    tbBill.addEditListener(new borland.jbcl.dataset.EditAdapter() {
      public void inserted(DataSet dataSet) throws DataSetException {
        ClearErrorsInResolver( dataSet );
        tbBill_inserted(dataSet);
      }
      public void addError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbBill_addError(dataSet, row, ex, response);
      }
      public void deleteError(DataSet dataSet, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbBill_deleteError(dataSet, ex, response);
      }
      public void updateError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbBill_updateError(dataSet, row, ex, response);
      }
      public void editError(DataSet dataSet, Column column, Variant value, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbBill_editError(dataSet, column, value, ex, response);
      }
      public void deleted(DataSet dataSet) throws DataSetException {
        ClearErrorsInResolver( dataSet );
      }

    });
    textControl118.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    textControl118.setForeground(Color.blue);
    textControl118.setFont(new Font("Helvetica", 3, 12));
    textControl118.setText("Comments");
    txtTtlPayLst.setBackground(new Color(192, 210, 192));
    txtTtlPayLst.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    txtTtlPayLst.setForeground(Color.blue);
    txtTtlPayLst.setFont(new Font("Helvetica", 3, 16));
    txtTtlPayLst.setMargins(new Insets(0, 0, 5, 0));
    txtTtlPayLst.setText("Pay lists for BillNo:");
    txtTtlPayLst.setTransparent(false);
    bevelPanel4.setDoubleBuffered(false);
    bevelPanel4.setTextureName("");
    jbClosePayLst.setText("Close");
    jbClosePayLst.setForeground(new Color(200, 0, 0));
    jbClosePayLst.setFont(new Font("Dialog", 1, 11));
    jbClosePayLst.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jbClosePayLst_actionPerformed(e);
      }
    });
    tbRoomsUsed.setMetaDataUpdate(MetaDataUpdate.NONE);
    tbRoomsUsed.addEditListener(new borland.jbcl.dataset.EditAdapter() {
      public void inserted(DataSet dataSet) throws DataSetException {
        ClearErrorsInResolver( dataSet );
        tbRoomsUsed_inserted(dataSet);
      }
      public void addError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbRoomsUsed_addError(dataSet, row, ex, response);
      }
      public void deleteError(DataSet dataSet, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbRoomsUsed_deleteError(dataSet, ex, response);
      }
      public void editError(DataSet dataSet, Column column, Variant value, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbRoomsUsed_editError(dataSet, column, value, ex, response);
      }
      public void updateError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbRoomsUsed_updateError(dataSet, row, ex, response);
      }
      public void deleted(DataSet dataSet) throws DataSetException {
        ClearErrorsInResolver( dataSet );
      }

    });
    tbAddCh.setMetaDataUpdate(MetaDataUpdate.NONE);
    tbAddCh.addEditListener(new borland.jbcl.dataset.EditAdapter() {
      public void inserted(DataSet dataSet) throws DataSetException {
        ClearErrorsInResolver( dataSet );
        tbAddCh_inserted(dataSet);
      }
      public void addError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbAddCh_addError(dataSet, row, ex, response);
      }
      public void deleteError(DataSet dataSet, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbAddCh_deleteError(dataSet, ex, response);
      }
      public void editError(DataSet dataSet, Column column, Variant value, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbAddCh_editError(dataSet, column, value, ex, response);
      }
      public void updateError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbAddCh_updateError(dataSet, row, ex, response);
      }
      public void deleted(DataSet dataSet) throws DataSetException {
        ClearErrorsInResolver( dataSet );
      }

    });
    tbPayments.setMetaDataUpdate(MetaDataUpdate.NONE);
    tbPayments.addEditListener(new borland.jbcl.dataset.EditAdapter() {
      public void inserted(DataSet dataSet) throws DataSetException {
        ClearErrorsInResolver( dataSet );
        tbPayments_inserted(dataSet);
      }
      public void addError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbPayments_addError(dataSet, row, ex, response);
      }
      public void deleteError(DataSet dataSet, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbPayments_deleteError(dataSet, ex, response);
      }
      public void editError(DataSet dataSet, Column column, Variant value, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbPayments_editError(dataSet, column, value, ex, response);
      }
      public void updateError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
        tbPayments_updateError(dataSet, row, ex, response);
      }
      public void deleted(DataSet dataSet) throws DataSetException {
         ClearErrorsInResolver( dataSet );
      }

    });
    tbLook_Room.setMetaDataUpdate(MetaDataUpdate.NONE);
    tbLook_BillCat.setMetaDataUpdate(MetaDataUpdate.NONE);
    jLabel1.setText("First name");
    jLabel2.setText("Last name");
    tbPayments.setMaxRows(25);
    tbAddCh.setMaxRows(25);
    tbRoomsUsed.setMaxRows(25);

    jPanel5.setPreferredSize(new Dimension(10, 40));
    jPanel5.setBackground(new Color(192, 210, 192));
    jPanel5.setDoubleBuffered(false);

    gridRoomsUsed.setDataSet(tbRoomsUsed);
    gridRoomsUsed.setDoubleBuffered(false);
    gridRoomsUsed.setNavigateOnTab(false);
    gridRoomsUsed.setSortOnHeaderClick(false);
    gridRoomsUsed.addHeaderListener(new borland.jbcl.view.HeaderAdapter() {
      public void headerItemClicked(HeaderEvent e) {
        gridRoomsUsed_headerItemClicked(e);
      }
    });
    gridRoomsUsed.addCustomItemListener( this.new TErrorCustomize(gridRoomsUsed) );


    jPanel5.setLayout(xYLayout6);
    textControl15.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    textControl15.setForeground(Color.blue);
    textControl15.setFont(new Font("Helvetica", 3, 12));
    textControl15.setText("Rooms used");
    navBill1.setDataSet(tbRoomsUsed);
    navBill1.setShowRollover(true);

    gridAddCh.setDataSet(tbAddCh);
    gridAddCh.setDoubleBuffered(false);
    gridAddCh.setNavigateOnTab(false);
    gridAddCh.setSortOnHeaderClick(false);
    gridAddCh.addHeaderListener(new borland.jbcl.view.HeaderAdapter() {
      public void headerItemClicked(HeaderEvent e) {
        gridAddCh_headerItemClicked(e);
      }
    });
    gridAddCh.addCustomItemListener( this.new TErrorCustomize(gridAddCh) );


    gridPayments.setDataSet(tbPayments);
    gridPayments.setDoubleBuffered(false);
    gridPayments.setNavigateOnTab(false);
    gridPayments.setSortOnHeaderClick(false);
    gridPayments.addHeaderListener(new borland.jbcl.view.HeaderAdapter() {
      public void headerItemClicked(HeaderEvent e) {
        gridPayments_headerItemClicked(e);
      }
    });
    gridPayments.addCustomItemListener( this.new TErrorCustomize(gridPayments) );


    navGuestCat1.setDataSet(tbAddCh);
    navGuestCat1.setShowRollover(true);
    navGuestCat1.setTextureName("");
    textControl119.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    textControl119.setForeground(Color.blue);
    textControl119.setFont(new Font("Helvetica", 3, 12));
    textControl119.setText("Additional charges");
    textControl120.setText("Payments");
    listAddCh.setFont(new Font("Dialog", 1, 14));
    navBillCat1.setDataSet(tbPayments);
    navBillCat1.setShowRollover(true);
    listPayments.setFont(new Font("Dialog", 1, 14));
    listRoomsUsed.setBackground(SystemColor.info);
    listRoomsUsed.setFont(new Font("Dialog", 1, 14));
    listPayments.setBackground(SystemColor.info);
    listAddCh.setBackground(SystemColor.info);
    textControl120.setFont(new Font("Helvetica", 3, 12));
    textControl120.setForeground(Color.blue);
    textControl120.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    bpPayLists.setLayout(borderLayout4);
    textControl116.setForeground(Color.black);
    textControl116.setFont(new Font("Dialog", 0, 12));
    textControl116.setText("Total additional:");
    fcFOLIO3.setReadOnly(true);
    fcFOLIO3.setDataSet(tbBill);
    fcFOLIO3.setColumnName("FULL_NAME");
    fcFOLIO3.setText("fcFOLIO3");
    textControl117.setText("Created by emp.:");
    jbEnrol.setText("Pay enrol");
    jbEnrol.setForeground(new Color(64, 128, 128));
    jbEnrol.setFont(new Font("Helvetica", 0, 11));
    jbEnrol.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jbEnrol_actionPerformed(e);
      }
    });
    jbMBack.setText("Pay enrol");
    jbMBack.setForeground(new Color(64, 128, 128));
    jbMBack.setLabel("Money back");
    jbMBack.setFont(new Font("Helvetica", 0, 11));
    jbMBack.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jbMBack_actionPerformed(e);
      }
    });
    fc_TOTAL.setReadOnly(true);
    fc_TOTAL.setDataSet(tbBill);
    fc_TOTAL.setColumnName("_TOTAL");
    fc_TOTAL_ADDITIONAL.setReadOnly(true);
    fc_TOTAL_ADDITIONAL.setDataSet(tbBill);
    fc_TOTAL_ADDITIONAL.setColumnName("_TOTAL_ADDITIONAL");
    fc_SUBTOTAL.setReadOnly(true);
    fc_SUBTOTAL.setDataSet(tbBill);
    fc_SUBTOTAL.setColumnName("_SUBTOTAL");
    fc_TOTAL_ROOMSPAY.setReadOnly(true);
    fc_TOTAL_ROOMSPAY.setDataSet(tbBill);
    fc_TOTAL_ROOMSPAY.setColumnName("_TOTAL_ROOMSPAY");
    fcDETAILS.setDataSet(tbBill);
    fcDETAILS.setColumnName("DETAILS");
    textControl13.setText("Bill No:");
    listBill.setBackground(SystemColor.info);
    tbGuestCat.setMaxRows(25);
    textControl10.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    textControl10.setForeground(Color.blue);
    textControl10.setFont(new Font("Helvetica", 3, 12));
    textControl10.setText("All guests categoryes");
    listGuestCat.setFont(new Font("Dialog", 1, 14));
    listBillCat.setBackground(SystemColor.info);
    listBillCat.setFont(new Font("Dialog", 1, 14));
    navGuestCat.setDataSet(tbGuestCat);
    navGuestCat.setShowRollover(true);
    textControl11.setText("All bills categoryes");
    navBillCat.setDataSet(tbBillCat);
    navBillCat.setShowRollover(true);
    textControl11.setFont(new Font("Helvetica", 3, 12));
    textControl11.setForeground(Color.blue);
    textControl11.setAlignment(borland.jbcl.util.Alignment.CENTER | borland.jbcl.util.Alignment.MIDDLE);
    listGuestCat.setBackground(SystemColor.info);
    listRoom.setBackground(SystemColor.info);
    fcSTATE.setColumnName("STATE");
    fcSTATE.setDataSet(tbGuest);
    fcLNAME.setColumnName("LNAME");
    fcLNAME.setDataSet(tbGuest);
    tbGuest.setMaxRows(25);
    bevelPanel1.setBevelInner(BevelPanel.FLAT);
    bevelPanel1.setDoubleBuffered(false);
    bevelPanel1.setBevelOuter(BevelPanel.LOWERED);
    bevelPanel1.setTextureName("");

    bevelPanel1.setLayout(xYLayout2);
    jPanel2.setPreferredSize(new Dimension(10, 40));
    jPanel2.setBackground(new Color(192, 210, 192));
    jPanel2.setDoubleBuffered(false);
    jPanel2.setLayout(xYLayout1);
    tbLook_GuestCat.setMetaDataUpdate(MetaDataUpdate.NONE);
    tbLook_GuestCat.setReadOnly(true);



    jPanel1.setLayout(cardLayout1);

    jPanel1.add(bpPayLists, "bpPayLists");
    bpPayLists.add(txtTtlPayLst, BorderLayout.NORTH);
    bpPayLists.add(bevelPanel4, BorderLayout.CENTER);
    bevelPanel4.add(gridRoomsUsed, new XYConstraints(9, 25, 633, 127));
    bevelPanel4.add(textControl15, new XYConstraints(289, 7, -1, -1));
    bevelPanel4.add(navBill1, new XYConstraints(9, 155, -1, -1));
    bevelPanel4.add(gridAddCh, new XYConstraints(9, 206, 331, 197));
    bevelPanel4.add(gridPayments, new XYConstraints(394, 206, 248, 197));
    bevelPanel4.add(navGuestCat1, new XYConstraints(9, 406, -1, -1));
    bevelPanel4.add(textControl119, new XYConstraints(121, 188, -1, -1));
    bevelPanel4.add(textControl120, new XYConstraints(489, 188, -1, -1));
    bevelPanel4.add(listAddCh, new XYConstraints(344, 206, 42, 197));
    bevelPanel4.add(navBillCat1, new XYConstraints(394, 406, -1, -1));
    bevelPanel4.add(listPayments, new XYConstraints(646, 206, 42, 197));
    bevelPanel4.add(listRoomsUsed, new XYConstraints(646, 25, 42, 127));
    bpPayLists.add(jPanel5, BorderLayout.SOUTH);
    jPanel5.add(jbClosePayLst, new XYConstraints(617, 8, -1, -1));
    jPanel1.add(bpBill, "bpBill");
    bpBill.add(txtTtlBill, BorderLayout.NORTH);
    bpBill.add(bevelPanel3, BorderLayout.CENTER);
    bevelPanel3.add(gridBill, new XYConstraints(15, 26, 294, 220));
    bevelPanel3.add(textControl12, new XYConstraints(145, 8, -1, -1));
    bevelPanel3.add(navBill, new XYConstraints(15, 250, -1, -1));
    bevelPanel3.add(listBill, new XYConstraints(313, 26, 42, 220));
    bevelPanel3.add(fcBILL_NO, new XYConstraints(364, 90, 85, -1));
    bevelPanel3.add(textControl13, new XYConstraints(364, 72, -1, -1));
    bevelPanel3.add(textControl14, new XYConstraints(485, 72, -1, -1));
    bevelPanel3.add(fcFOLIO, new XYConstraints(485, 90, 142, -1));
    bevelPanel3.add(textAreaComments, new XYConstraints(488, 287, 196, 132));
    bevelPanel3.add(fcHEARD_OF_GI, new XYConstraints(485, 156, 204, -1));
    bevelPanel3.add(textControl16, new XYConstraints(364, 139, -1, -1));
    bevelPanel3.add(textControl17, new XYConstraints(485, 139, -1, -1));
    bevelPanel3.add(fcDATE_, new XYConstraints(364, 156, 103, -1));
    bevelPanel3.add(fcDETAILS, new XYConstraints(364, 224, 325, -1));
    bevelPanel3.add(textControl18, new XYConstraints(364, 207, -1, -1));
    bevelPanel3.add(groupBox1, new XYConstraints(5, 285, 477, 141));
    groupBox1.add(fc_TOTAL_ROOMSPAY, new XYConstraints(2, 25, 106, -1));
    groupBox1.add(textControl19, new XYConstraints(2, 7, -1, -1));
    groupBox1.add(fc_TAX_AND_SERVICE, new XYConstraints(114, 25, 106, -1));
    groupBox1.add(textControl110, new XYConstraints(114, 7, -1, -1));
    groupBox1.add(textControl111, new XYConstraints(226, 7, -1, -1));
    groupBox1.add(fc_SUBTOTAL, new XYConstraints(226, 25, 106, -1));
    groupBox1.add(textControl112, new XYConstraints(339, 7, -1, -1));
    groupBox1.add(fc_DEPOSIT_RECIEVED, new XYConstraints(339, 25, 106, -1));
    groupBox1.add(textControl113, new XYConstraints(116, 65, -1, -1));
    groupBox1.add(textControl114, new XYConstraints(228, 65, -1, -1));
    groupBox1.add(textControl115, new XYConstraints(341, 65, -1, -1));
    groupBox1.add(fc_TOTAL_ADDITIONAL, new XYConstraints(4, 83, 106, -1));
    groupBox1.add(fc_TOTAL_PAYMENTS, new XYConstraints(116, 83, 106, -1));
    groupBox1.add(fc_TOTAL, new XYConstraints(228, 83, 106, -1));
    groupBox1.add(fc_BALANCE_DUE, new XYConstraints(341, 83, 106, -1));
    groupBox1.add(textControl116, new XYConstraints(4, 65, -1, -1));
    bevelPanel3.add(fcFOLIO3, new XYConstraints(423, 28, 266, -1));
    bevelPanel3.add(textControl117, new XYConstraints(423, 10, -1, -1));
    bevelPanel3.add(jbEnrol, new XYConstraints(308, 259, 90, -1));
    bevelPanel3.add(jbMBack, new XYConstraints(409, 259, 98, -1));
    bevelPanel3.add(textControl118, new XYConstraints(555, 265, -1, -1));
    bpBill.add(jPanel4, BorderLayout.SOUTH);
    jPanel4.add(jbPayLists, new XYConstraints(491, 8, -1, -1));
    jPanel4.add(jbCloseBill, new XYConstraints(617, 8, -1, -1));
    jPanel1.add(bpTime, "bpTime");
    jPanel1.add(bpLists, "bpLists");
    bpLists.add(textTtGuest1, BorderLayout.NORTH);
    bpLists.add(bevelPanel2, BorderLayout.CENTER);
    bevelPanel2.add(gridRoom, new XYConstraints(12, 24, 630, 170));
    bevelPanel2.add(textControl9, new XYConstraints(282, 7, -1, -1));
    bevelPanel2.add(listRoom, new XYConstraints(645, 24, 42, 170));
    bevelPanel2.add(gridGuestCats, new XYConstraints(9, 241, 289, 164));
    bevelPanel2.add(gridBillCat, new XYConstraints(353, 241, 289, 164));
    bevelPanel2.add(navRoom, new XYConstraints(12, 197, -1, -1));
    bevelPanel2.add(textControl10, new XYConstraints(91, 223, -1, -1));
    bevelPanel2.add(listGuestCat, new XYConstraints(301, 241, 42, 164));
    bevelPanel2.add(listBillCat, new XYConstraints(645, 241, 42, 164));
    bevelPanel2.add(navGuestCat, new XYConstraints(9, 407, -1, -1));
    bevelPanel2.add(textControl11, new XYConstraints(443, 223, -1, -1));
    bevelPanel2.add(navBillCat, new XYConstraints(353, 407, -1, -1));
    bpLists.add(jPanel3, BorderLayout.SOUTH);
    jPanel3.add(jbCloseLists, new XYConstraints(623, 8, -1, -1));
    jPanel1.add(bpGuest, "bpGuest");
    bpGuest.add(textTtGuest, BorderLayout.NORTH);
    bpGuest.add(bevelPanel1, BorderLayout.CENTER);
    bevelPanel1.add(gridGuest, new XYConstraints(7, 7, 323, 288));
    bevelPanel1.add(navGuest, new XYConstraints(7, 302, 309, -1));
    bevelPanel1.add(listGuest, new XYConstraints(333, 7, 49, 288));
    bevelPanel1.add(btnApplyMask, new XYConstraints(316, 374, -1, 39));
    bevelPanel1.add(textControl1, new XYConstraints(392, 10, -1, -1));
    bevelPanel1.add(textControl2, new XYConstraints(542, 10, -1, -1));
    bevelPanel1.add(textControl3, new XYConstraints(392, 65, -1, -1));
    bevelPanel1.add(textControl4, new XYConstraints(392, 121, -1, -1));
    bevelPanel1.add(textControl5, new XYConstraints(569, 122, -1, -1));
    bevelPanel1.add(fcPostal, new XYConstraints(391, 205, 111, -1));
    bevelPanel1.add(textControl6, new XYConstraints(391, 186, -1, -1));
    bevelPanel1.add(fcDeposit, new XYConstraints(587, 205, -1, -1));
    bevelPanel1.add(textControl7, new XYConstraints(587, 186, -1, -1));
    bevelPanel1.add(fcStatus, new XYConstraints(472, 266, 139, -1));
    bevelPanel1.add(textControl8, new XYConstraints(472, 248, -1, -1));
    bevelPanel1.add(fcFNAME, new XYConstraints(392, 29, 144, -1));
    bevelPanel1.add(fcLNAME, new XYConstraints(542, 29, 144, -1));
    bevelPanel1.add(fcADDRESS, new XYConstraints(392, 84, 295, -1));
    bevelPanel1.add(fcCITY, new XYConstraints(392, 139, 144, -1));
    bevelPanel1.add(fcSTATE, new XYConstraints(569, 139, 117, -1));
    bevelPanel1.add(textFMask, new XYConstraints(88, 366, 221, 23));
    bevelPanel1.add(textLMask, new XYConstraints(88, 399, 221, 23));
    bevelPanel1.add(jLabel1, new XYConstraints(9, 367, 76, 20));
    bevelPanel1.add(jLabel2, new XYConstraints(9, 400, 77, 20));
    bpGuest.add(jPanel2, BorderLayout.SOUTH);
    jPanel2.add(jbGotoBill, new XYConstraints(556, 9, 121, -1));
    jPanel2.add(jbGotoLists, new XYConstraints(11, 9, -1, -1));
    jPanel2.add(jbGotoTime, new XYConstraints(83, 9, -1, -1));

    /*bpGuest.setVisible( true );
    bpBill.setVisible( false );
    bpLists.setVisible( false );
    bpTime.setVisible( false );*/

    /*listGuest.setVisible( false );
    listRoom.setVisible( false );
    listGuestCat.setVisible( false );
    listBillCat.setVisible( false );*/
  }

  public void start() {
    try {
      if( B_TRACE ) m_tim.start();

      InetAddress inAddr = InetAddress.getByName( getCodeBase().getHost() );
      String host = inAddr.getHostAddress();
      String port = Integer.toString( getCodeBase().getPort() );
      String registryName = "rmi://" + host + ":" + port + "/GServer";
      m_igServer = (IGServer)Naming.lookup( registryName );

      if( B_TRACE )
        System.out.println( "Lookup server: " + m_tim.startF() );

      m_irRefControl = m_igServer.ConnectClient( new java.rmi.dgc.VMID() );

      if( B_TRACE )
        System.out.println( "Get reference to server: " + m_tim.startF() );
     }
    catch( Exception ex ) {
      ex.printStackTrace();
      MessageDialog dlg = new MessageDialog( new DecoratedFrame(),
        "Error (m)", ex.getMessage(), ButtonDialog.OK );
      //dlg.setVisible( true );
      dlg.show();
    }
   try {
     MkOpedDb( true );
     if( B_TRACE )
        System.out.println( "Open connection to DB: " + m_tim.startF() );
     InitProviders();

     if( !m_bInit )
      {
        GotoGuests();
        cardLayout1.show( bpGuest.getParent(), "bpGuest" );
      }

     if( B_TRACE )
        System.out.println( "Init providers: " + m_tim.startF() );

     m_bInit = true;

   }
   catch( Exception ex ) {
      ex.printStackTrace();
      MessageDialog dlg = new MessageDialog( new DecoratedFrame(),
        "Error (m)", ex.getMessage(), ButtonDialog.OK );
      //dlg.setVisible( true );
      dlg.show();
    }
  }

  private void MkCloseAllTables()
   {
     try {
       Field[] ff = this.getClass().getDeclaredFields();
       int i = ff.length - 1;
       for( ; i >= 0; --i )
        {
          if( ff[ i ].getClass().getName().compareTo("TableDataSet") == 0 )
           {
             TableDataSet tmp = (TableDataSet)ff[ i ].get( this );
             if( tmp.isOpen() ) tmp.close();
           }
        }
      }
     catch( Exception ex )
      {
        ex.printStackTrace();
      }
   }

  public void stop() {
    try {
      MkCloseAllTables();
      MkOpedDb( false );

      m_igServer = null;
      if( m_irRefControl != null ) m_irRefControl.ClearRef();
      m_irRefControl = null;
      //System.gc(); System.runFinalization();
    }
    catch( Exception ex ) {
      ex.printStackTrace();
      MessageDialog dlg = new MessageDialog( new DecoratedFrame(),
        "Error (m)", ex.getMessage(), ButtonDialog.OK );
      //dlg.setVisible( true );
      dlg.show();
    }
  }

  public void destroy() {
  }
//Get Applet information

  public String getAppletInfo() {
    return "Applet Information";
  }
//Get parameter info

  public String[][] getParameterInfo() {
    return null;
  }

  private String m_sUser, m_sPwnd;

  TableDataSet tbLook_GuestCat = new TableDataSet();
  TextControl textTtGuest = new TextControl();
  BevelPanel bevelPanel1 = new BevelPanel();
  JPanel jPanel2 = new JPanel();
  BorderLayout borderLayout1 = new BorderLayout();
  JButton jbGotoBill = new JButton();
  XYLayout xYLayout1 = new XYLayout();
  JButton jbGotoLists = new JButton();
  JButton jbGotoTime = new JButton();
  XYLayout xYLayout2 = new XYLayout();
  GridControl gridGuest = new GridControl();
  NavigatorControl navGuest = new NavigatorControl();
  List listGuest = new List();
  JButton btnApplyMask = new JButton();
  TextControl textControl1 = new TextControl();
  TextControl textControl2 = new TextControl();
  TextControl textControl3 = new TextControl();
  TextControl textControl4 = new TextControl();
  TextControl textControl5 = new TextControl();
  FieldControl fcPostal = new FieldControl();
  TextControl textControl6 = new TextControl();
  FieldControl fcDeposit = new FieldControl();
  TextControl textControl7 = new TextControl();
  FieldControl fcStatus = new FieldControl();
  TextControl textControl8 = new TextControl();
  TableDataSet tbGuest = new TableDataSet();
  FieldControl fcFNAME = new FieldControl();
  FieldControl fcLNAME = new FieldControl();
  FieldControl fcADDRESS = new FieldControl();
  FieldControl fcCITY = new FieldControl();
  FieldControl fcSTATE = new FieldControl();
  JTextField textFMask = new JTextField();
  JTextField textLMask = new JTextField();
  TextControl textTtGuest1 = new TextControl();
  BevelPanel bevelPanel2 = new BevelPanel();
  JPanel jPanel3 = new JPanel();
  BorderLayout borderLayout2 = new BorderLayout();
  GridControl gridRoom = new GridControl();
  TextControl textControl9 = new TextControl();
  List listRoom = new List(); 
  GridControl gridGuestCats = new GridControl();
  GridControl gridBillCat = new GridControl();
  NavigatorControl navRoom = new NavigatorControl();
  TextControl textControl10 = new TextControl();
  List listGuestCat = new List();
  List listBillCat = new List();
  NavigatorControl navGuestCat = new NavigatorControl();
  TextControl textControl11 = new TextControl();
  NavigatorControl navBillCat = new NavigatorControl();
  XYLayout xYLayout3 = new XYLayout();
  JButton jbCloseLists = new JButton();
  TableDataSet tbGuestCat = new TableDataSet();
  TableDataSet tbBillCat = new TableDataSet();
  TextControl txtTtlBill = new TextControl();
  BevelPanel bevelPanel3 = new BevelPanel();
  JPanel jPanel4 = new JPanel();
  BorderLayout borderLayout3 = new BorderLayout();
  GridControl gridBill = new GridControl();
  TextControl textControl12 = new TextControl();
  NavigatorControl navBill = new NavigatorControl();
  List listBill = new List();
  FieldControl fcBILL_NO = new FieldControl();
  TextControl textControl13 = new TextControl();
  TextControl textControl14 = new TextControl();
  FieldControl fcFOLIO = new FieldControl();
  TextAreaControl textAreaComments = new TextAreaControl();
  FieldControl fcHEARD_OF_GI = new FieldControl();
  TextControl textControl16 = new TextControl();
  TextControl textControl17 = new TextControl();
  FieldControl fcDATE_ = new FieldControl();
  FieldControl fcDETAILS = new FieldControl();
  TextControl textControl18 = new TextControl();
  GroupBox groupBox1 = new GroupBox();
  FieldControl fc_TOTAL_ROOMSPAY = new FieldControl();
  TextControl textControl19 = new TextControl();
  XYLayout xYLayout4 = new XYLayout();
  FieldControl fc_TAX_AND_SERVICE = new FieldControl();
  TextControl textControl110 = new TextControl();
  TextControl textControl111 = new TextControl();
  FieldControl fc_SUBTOTAL = new FieldControl();
  TextControl textControl112 = new TextControl();
  FieldControl fc_DEPOSIT_RECIEVED = new FieldControl();
  TextControl textControl113 = new TextControl();
  TextControl textControl114 = new TextControl();
  TextControl textControl115 = new TextControl();
  FieldControl fc_TOTAL_ADDITIONAL = new FieldControl();
  FieldControl fc_TOTAL_PAYMENTS = new FieldControl();
  FieldControl fc_TOTAL = new FieldControl();
  FieldControl fc_BALANCE_DUE = new FieldControl();
  TextControl textControl116 = new TextControl();
  FieldControl fcFOLIO3 = new FieldControl();
  TextControl textControl117 = new TextControl();
  JButton jbEnrol = new JButton();
  JButton jbMBack = new JButton();
  JButton jbPayLists = new JButton();
  XYLayout xYLayout5 = new XYLayout();
  JButton jbCloseBill = new JButton();
  TableDataSet tbBill = new TableDataSet();
  TextControl textControl118 = new TextControl();

  TextControl txtTtlPayLst = new TextControl();
  BevelPanel bevelPanel4 = new BevelPanel();
  JPanel jPanel5 = new JPanel();
  BorderLayout borderLayout4 = new BorderLayout();
  GridControl gridRoomsUsed = new GridControl();
  TextControl textControl15 = new TextControl();
  NavigatorControl navBill1 = new NavigatorControl();
  GridControl gridAddCh = new GridControl();
  GridControl gridPayments = new GridControl();
  NavigatorControl navGuestCat1 = new NavigatorControl();
  TextControl textControl119 = new TextControl();
  TextControl textControl120 = new TextControl();
  List listAddCh = new List();
  NavigatorControl navBillCat1 = new NavigatorControl();
  List listPayments = new List();
  List listRoomsUsed = new List();
  JButton jbClosePayLst = new JButton();
  XYLayout xYLayout6 = new XYLayout();
  TableDataSet tbRoomsUsed = new TableDataSet();
  TableDataSet tbAddCh = new TableDataSet();
  TableDataSet tbPayments = new TableDataSet();
  TableDataSet tbLook_Room = new TableDataSet();
  TableDataSet tbLook_BillCat = new TableDataSet();
  JLabel jLabel1 = new JLabel();
  JLabel jLabel2 = new JLabel();

  void btnApplyMask_actionPerformed(ActionEvent e) {
    try {
      //m_prGuest.getParamRow().setString( "fmask", GetMask(textFMask) );
      //m_prGuest.getParamRow().setString( "lmask", GetMask(textLMask) );
      SetGuestQuery();
      tbGuest.refresh();
     }
    catch( Exception ex )
     {
       ex.printStackTrace();
     }
  }

  void jbCloseLists_actionPerformed(ActionEvent e) {
    cardLayout1.show( bpGuest.getParent(), "bpGuest" );
  }

  void jbGotoLists_actionPerformed(ActionEvent e) {
    try {
       GotoLists();
     }
    catch( DataSetException ex )
     {
       DataSetException.handleException( ex, true );
       return;
     }
    cardLayout1.show( bpLists.getParent(), "bpLists" );
  }

  void jbGotoBill_actionPerformed(ActionEvent e) {
    try {
      if( !tbGuest.isOpen() || tbGuest.getRowCount() == 0 )
        throw new Exception( "None exception" );
      if( tbGuest.isNull("ID_GUEST") )
       {
         JOptionPane.showMessageDialog(
         this.getContentPane(), "Cann't edit bills list: selected guest is inserted but not saved",
         "Information", JOptionPane.INFORMATION_MESSAGE );
         return;
       }
      txtTtlBill.setText( "Bills of " +
        tbGuest.getString("FNAME") + " " +
        tbGuest.getString("LNAME")
       );
      m_prBill.getParamRow().setInt( "ID_GUEST_P", tbGuest.getInt("ID_GUEST") );
     }
    catch( Exception ex )
     {
       System.out.println( ex.getMessage() );
       JOptionPane.showMessageDialog(
         this.getContentPane(), "Cann't go to bills list - guest not selected",
         "Error", JOptionPane.ERROR_MESSAGE );
       return;
     }
    try {
       GotoBills();
     }
    catch( DataSetException ex )
     {
       DataSetException.handleException( ex, true );
       return;
     }
    cardLayout1.show( bpBill.getParent(), "bpBill" );
  }

  void jbCloseBill_actionPerformed(ActionEvent e) {
    cardLayout1.show( bpGuest.getParent(), "bpGuest" );
  }

  void tbBill_inserted(DataSet dataSet) throws DataSetException {
    dataSet.setInt( "ID_GUEST", tbGuest.getInt("ID_GUEST") );
    Timestamp tm = new Timestamp( System.currentTimeMillis() );
    tm.setHours( 0 ); tm.setMinutes( 0 );
    tm.setSeconds( 0 ); tm.setNanos( 0 );
    dataSet.setTimestamp( "DATE_", tm );
    dataSet.setString( "HEARD_OF_GI", "NOT ENTERED" );
    dataSet.setString( "DETAILS", "NOT ENTERED" );
  }

  void tbBill_navigated(NavigationEvent e) {
     boolean bFl = false;
     try {
       if( tbBill.getRowCount() == 0 || tbBill.isNull("_PAY_ENROLMENT") )
         throw new Exception( "NONE exception" );
       bFl = tbBill.getByte( "_PAY_ENROLMENT" ) == 0 ? false:true;
      }
     catch( Exception ex )
      {
        jbEnrol.setEnabled( false );
        jbMBack.setEnabled( false );
        return;
      }
     jbEnrol.setEnabled( !bFl );
     jbMBack.setEnabled( bFl );
  }

  void GotoPayLists() throws DataSetException
   {
     if( !tbLook_Room.isOpen() )
      {
        tbLook_Room.open();
        tbLook_BillCat.open();
      }

     if( !tbRoomsUsed.isOpen() )
      {
        tbRoomsUsed.open();
        tbAddCh.open();
        tbPayments.open();
      }

     tbRoomsUsed.refresh(); //for parameters reload
     tbAddCh.refresh();
     tbPayments.refresh();
   }
  void GotoLists() throws DataSetException
   {
     if( !tbRoom.isOpen() )
      {
        tbRoom.open(); tbGuestCat.open(); tbBillCat.open();
        tbRoom.refresh(); tbGuestCat.refresh(); tbBillCat.refresh();
      }
   }
  void GotoBills()  throws DataSetException
   {
     if( !tbBill.isOpen() )
      {
        tbBill.open();
      }
     tbBill.refresh(); //for parameters reload
   }
  void GotoGuests()  throws DataSetException
   {
     if( !tbGuest.isOpen() )
      {
        tbLook_GuestCat.open();
        tbGuest.open();
        tbGuest.refresh();
      }
   }

  void tbGuest_inserted(DataSet dataSet) throws DataSetException {
    dataSet.setBigDecimal( "DEPOSIT", new BigDecimal(100.0) );
    dataSet.setInt( "ID_CAT", tbLook_GuestCat.getInt("ID_CAT") );
    dataSet.setString( "CITY", "NOT ENTERED" );
    dataSet.setString( "STATE", "NOT ENTERED" );
    dataSet.setString( "ADDRESS", "NOT ENTERED" );
  }

  void jbEnrol_actionPerformed(ActionEvent e) {
    PayEnrol( true );
  }

  void jbMBack_actionPerformed(ActionEvent e) {
     PayEnrol( false );
  }
  private void PayEnrol( boolean bFl )
   {
      try {
        ((IAccount)m_irRefControl).IAcc_PayEnrol(
          tbBill.getInt("BILL_NO"), bFl );

        int iPos = tbBill.getRow();
        tbBill.refresh();
        tbBill.goToClosestRow( iPos );
       }
      catch( DataSetException ex )
       {
         DataSetException.handleException( ex, true );
         return;
       }
      catch( RemoteException ex ) {
        ex.printStackTrace();
        JOptionPane.showMessageDialog(
          this.getContentPane(), ex.getMessage(),
          "Error", JOptionPane.ERROR_MESSAGE );
        return;
      }
   }

  void tbGuest_addError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
    HandleDSError( dataSet, row, ex, response );
  }

  void tbGuest_updateError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
    HandleDSError( dataSet, row, ex, response );
  }

  void tbGuest_deleteError(DataSet dataSet, DataSetException ex, ErrorResponse response) throws DataSetException {
    HandleDSError( dataSet, null, ex, response );
  }

  void tbGuest_editError(DataSet dataSet, Column column, Variant value, DataSetException ex, ErrorResponse response) throws DataSetException {
    HandleDSEditError( dataSet, column, value, ex, response );
  }

  void tbBill_addError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
    HandleDSError( dataSet, row, ex, response );
  }

  void tbBill_deleteError(DataSet dataSet, DataSetException ex, ErrorResponse response) throws DataSetException {
    HandleDSError( dataSet, null, ex, response );
  }

  void tbBill_updateError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
    HandleDSError( dataSet, row, ex, response );
  }

  void tbBill_editError(DataSet dataSet, Column column, Variant value, DataSetException ex, ErrorResponse response) throws DataSetException {
    HandleDSEditError( dataSet, column, value, ex, response );
  }
  private void HandleDSError( DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response ) throws DataSetException
   {
     JOptionPane.showMessageDialog(
        this.getContentPane(), ex.getMessage(),
        "Error", JOptionPane.ERROR_MESSAGE );
     response.abort();
   }
  private void HandleDSEditError( DataSet dataSet, Column column, Variant value, DataSetException ex, ErrorResponse response ) throws DataSetException
   {
     JOptionPane.showMessageDialog(
        this.getContentPane(), ex.getMessage(),
        "Edit error in column '" + column.getColumnName() + "'",
        JOptionPane.ERROR_MESSAGE );

     response.abort();
   }

  void jbPayLists_actionPerformed(ActionEvent e) {
    try {
      if( !tbBill.isOpen() || tbBill.getRowCount() == 0 )
        throw new Exception( "None exception" );
      if( tbBill.isNull("BILL_NO") )
       {
         JOptionPane.showMessageDialog(
         this.getContentPane(), "Cann't edit pay lists: selected bill is inserted but not saved",
         "Information", JOptionPane.INFORMATION_MESSAGE );
         return;
       }
      txtTtlPayLst.setText( "Pay lists of #" +
        Integer.toString( tbBill.getInt("BILL_NO") )
       );
      m_prRoomsUsed.getParamRow().setInt( "BILL_NO", tbBill.getInt("BILL_NO") );
      m_prAddCh.getParamRow().setInt( "BILL_NO", tbBill.getInt("BILL_NO") );
      m_prPayments.getParamRow().setInt( "BILL_NO", tbBill.getInt("BILL_NO") );
     }
    catch( Exception ex )
     {
       System.out.println( ex.getMessage() );
       JOptionPane.showMessageDialog(
         this.getContentPane(), "Cann't go to pay lists - bill not selected",
         "Error", JOptionPane.ERROR_MESSAGE );
       return;
     }
    try {
       GotoPayLists();
     }
    catch( DataSetException ex )
     {
       DataSetException.handleException( ex, true );
       return;
     }
    cardLayout1.show( bpPayLists.getParent(), "bpPayLists" );
  }

  void jbClosePayLst_actionPerformed(ActionEvent e) {
    try {
        int iPos = tbBill.getRow();
        tbBill.refresh();
        tbBill.goToClosestRow( iPos );
      }
     catch( DataSetException ex )
      {
        DataSetException.handleException( ex, true );
      }
     cardLayout1.show( bpBill.getParent(), "bpBill" );
  }

  void tbRoomsUsed_inserted(DataSet dataSet) throws DataSetException {
    dataSet.setInt( "BILL_NO", tbBill.getInt("BILL_NO") );

    Timestamp tm = new Timestamp( System.currentTimeMillis() );
    tm.setHours( 0 ); tm.setMinutes( 0 );
    tm.setSeconds( 0 ); tm.setNanos( 0 );
    dataSet.setTimestamp( "DATE_IN", tm );

    Calendar cal = Calendar.getInstance();
    cal.set( Calendar.DATE, tm.getDate() );
    cal.add( Calendar.DATE, 3 );
    tm.setDate( cal.get(Calendar.DATE) );
    dataSet.setTimestamp( "DATE_OUT", tm );

    dataSet.setInt( "NO_OF_GUESTS", 2 );
    dataSet.setInt( "ID_ROOM", tbLook_Room.getInt("ID_ROOM") );
  }

  void tbAddCh_inserted(DataSet dataSet) throws DataSetException {
    dataSet.setInt( "BILL_NO", tbBill.getInt("BILL_NO") );

    Timestamp tm = new Timestamp( System.currentTimeMillis() );
    tm.setHours( 0 ); tm.setMinutes( 0 );
    tm.setSeconds( 0 ); tm.setNanos( 0 );
    dataSet.setTimestamp( "MKDATE", tm );
  }

  void tbPayments_inserted(DataSet dataSet) throws DataSetException {
    dataSet.setInt( "BILL_NO", tbBill.getInt("BILL_NO") );
  }

  void tbRoomsUsed_addError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
     HandleDSError( dataSet, row, ex, response );
  }

  void tbRoomsUsed_deleteError(DataSet dataSet, DataSetException ex, ErrorResponse response) throws DataSetException {
     HandleDSError( dataSet, null, ex, response );
  }

  void tbRoomsUsed_editError(DataSet dataSet, Column column, Variant value, DataSetException ex, ErrorResponse response) throws DataSetException {
     HandleDSEditError( dataSet, column, value, ex, response );
  }

  void tbAddCh_addError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
     HandleDSError( dataSet, row, ex, response );
  }

  void tbAddCh_deleteError(DataSet dataSet, DataSetException ex, ErrorResponse response) throws DataSetException {
     HandleDSError( dataSet, null, ex, response );
  }

  void tbAddCh_editError(DataSet dataSet, Column column, Variant value, DataSetException ex, ErrorResponse response) throws DataSetException {
    HandleDSEditError( dataSet, column, value, ex, response );
  }

  void tbAddCh_updateError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
    HandleDSError( dataSet, row, ex, response );
  }

  void tbRoomsUsed_updateError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
     HandleDSError( dataSet, row, ex, response );
  }

  void tbPayments_addError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
     HandleDSError( dataSet, row, ex, response );
  }

  void tbPayments_deleteError(DataSet dataSet, DataSetException ex, ErrorResponse response) throws DataSetException {
     HandleDSError( dataSet, null, ex, response );
  }

  void tbPayments_editError(DataSet dataSet, Column column, Variant value, DataSetException ex, ErrorResponse response) throws DataSetException {
      HandleDSEditError( dataSet, column, value, ex, response );
  }

  void tbPayments_updateError(DataSet dataSet, ReadWriteRow row, DataSetException ex, ErrorResponse response) throws DataSetException {
      HandleDSError( dataSet, row, ex, response );
  }

  void gridRoom_headerItemClicked(HeaderEvent e) {
     headerItemClicked_Handle( e, gridRoom );
  }

  void gridRoomsUsed_headerItemClicked(HeaderEvent e) {
     headerItemClicked_Handle( e, gridRoomsUsed );
  }

  void gridAddCh_headerItemClicked(HeaderEvent e) {
     headerItemClicked_Handle( e, gridAddCh );
  }

  void gridPayments_headerItemClicked(HeaderEvent e) {
    headerItemClicked_Handle( e, gridPayments );
  }

  void gridBill_headerItemClicked(HeaderEvent e) {
    headerItemClicked_Handle( e, gridBill );
  }

  void gridGuestCats_headerItemClicked(HeaderEvent e) {
    headerItemClicked_Handle( e, gridGuestCats );
  }

  void gridBillCat_headerItemClicked(HeaderEvent e) {
    headerItemClicked_Handle( e, gridBillCat );
  }

  void gridGuest_headerItemClicked(HeaderEvent e) {
    headerItemClicked_Handle( e, gridGuest );
  }

  protected void ClearErrorsInResolver( DataSet ds )
   {
     if( !(ds instanceof TableDataSet) ) return;
     TableDataSet tb = (TableDataSet)ds;
     Resolver res = tb.getResolver();
     if( res == null ) return;
     if( !(res instanceof TResolver) ) return;

     ((TResolver)res).m_vErrors = null;
   }
}


