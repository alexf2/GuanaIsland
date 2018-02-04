package G_APPL;

import java.awt.*;
import com.sun.java.swing.JPanel;
import java.awt.event.*;
import borland.jbcl.layout.*;

public class PanPwd extends JPanel {
  //BevelPanel bevelPanel1 = new BevelPanel();
  TextField txtLogin = new TextField();
  TextField txtPwd = new TextField();
  Label label1 = new Label();
  Label label2 = new Label();


  public PanPwd() {
    try  {
      jbInit();
    }
    catch (Exception ex) {
      ex.printStackTrace();
    }
  }

  void jbInit() throws Exception {
    this.setLayout(xYLayout1);
    //bevelPanel1.setLayout(xYLayout1);
    txtLogin.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        txtLogin_actionPerformed(e);
      }
    });
    this.setNextFocusableComponent(txtLogin);
    this.setOpaque(false);
    this.setDoubleBuffered(false);
    txtPwd.setEchoChar('*');
    label1.setFont(new Font("Dialog", 1, 12));
    label1.setText("User Name:");
    label2.setFont(new Font("Dialog", 1, 12));
    label2.setText("Password:");
    xYLayout1.setHeight(97);
    xYLayout1.setWidth(238);
    //this.add(bevelPanel1, new GridBagConstraints2(0, 0, 1, 1, 0.0, 0.0
      //      ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 15, 12));
    this.add(txtLogin, new XYConstraints(101, 16, 117, -1));
    this.add(txtPwd, new XYConstraints(101, 55, 117, -1));
    this.add(label1, new XYConstraints(19, 16, -1, -1));
    this.add(label2, new XYConstraints(19, 55, -1, -1));

    txtLogin.requestFocus();
  }

  void txtLogin_actionPerformed(ActionEvent e) {
  }

  void btnOK_actionPerformed(ActionEvent e) {
     m_iRes = 1;
  }

  void btnCancel_actionPerformed(ActionEvent e) {
     m_iRes = 0;
  }
  public int m_iRes = 0;
  XYLayout xYLayout1 = new XYLayout();
}

