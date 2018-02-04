package G_APPL;

import java.awt.*;
import java.awt.event.*;
import borland.jbcl.layout.*;
import borland.jbcl.control.*;
import java.util.*;
import borland.sql.dataset.*;
import borland.jbcl.dataset.*;


public class DataServerFrame extends DecoratedFrame {

  GridBagLayout gridBagLayout1 = new GridBagLayout();

  List list1 = new List();
  Label label1 = new Label();
  
  ButtonControl buttonControl1 = new ButtonControl();
  public DataServerFrame( GServer srv ) {
    try  {
      app = srv;
      jbInit();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    this.setLayout(gridBagLayout1);
    this.setTitle("Guana server1");
    this.addWindowListener(new java.awt.event.WindowAdapter() {
      public void windowClosing(WindowEvent e) {
        this_windowClosing(e);
      }
    });
    label1.setFont(new Font("Dialog", 1, 12));
    label1.setAlignment(1);
    label1.setText("Clients");
    buttonControl1.setActionCommand("");
    buttonControl1.setLabel("Close server");
    buttonControl1.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        buttonControl1_actionPerformed(e);
      }
    });
    this.add(list1, new GridBagConstraints2(0, 1, 1, 1, 1.0, 0.7
            ,GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 4, 0, 4), 400, 200));
    this.add(label1, new GridBagConstraints2(0, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 43, 0));
    this.add(buttonControl1, new GridBagConstraints2(0, 2, 1, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(4, 0, 4, 0), 0, 0));
  }

  void buttonControl1_actionPerformed( ActionEvent e ) {
    app.closing();
    System.exit( 1 );
  }

  void this_windowClosing( WindowEvent e ) {
    app.closing();
  }

  private GServer app = null;
}

