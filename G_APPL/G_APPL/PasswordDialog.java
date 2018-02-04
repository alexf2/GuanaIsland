package  G_APPL;

import java.awt.*;
import java.awt.event.*;
import borland.jbcl.layout.*;
import borland.jbcl.control.*;
import com.sun.java.swing.*;

public class PasswordDialog extends Dialog {
  Panel panel1 = new Panel();
  Panel panel2 = new Panel();
  Panel dialogPanel = new Panel();
  BevelPanel bevelPanel1 = new BevelPanel();
  Button btnOK = new Button();
  Button btnCancel = new Button();
  Label label1 = new Label();
  TextField txtPwd = new TextField();
  GridLayout gridLayout1 = new GridLayout();
  FlowLayout flowLayout1 = new FlowLayout();
  XYLayout xYLayout1 = new XYLayout();
  Label label2 = new Label();
  JTextField txtLogin = new JTextField();

  public PasswordDialog(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
    add(dialogPanel,BorderLayout.CENTER);
    pack();
    m_iRes = 0;
  }

  public PasswordDialog(Frame frame, String title) {
    this(frame, title, false);
  }

  public PasswordDialog(Frame frame) {
    this(frame, "", false);
  }

  private void jbInit() throws Exception {
    dialogPanel.setLayout(xYLayout2);
    bevelPanel1.setLayout(xYLayout1);
    btnOK.setLabel("OK");
    btnOK.addActionListener(new PasswordDialog_btnOK_actionAdapter(this));
    btnCancel.setLabel("Cancel");
    label1.setText("Password:");
    txtPwd.setEchoChar('@');
    gridLayout1.setVgap(4);
    label2.setText("Login:");
    gridLayout1.setHgap(6);
    btnCancel.addActionListener(new PasswordDialog_btnCancel_actionAdapter(this));
    this.addWindowListener(new PasswordDialog_this_windowAdapter(this));
    panel1.add(panel2);
    bevelPanel1.setMargins(new Insets(4, 4, 4, 4));
    panel2.setLayout(gridLayout1);
    panel1.setLayout(flowLayout1);
    dialogPanel.setSize(new Dimension(288, 100));
    bevelPanel1.setBevelInner(BevelPanel.RAISED);
    bevelPanel1.setBevelOuter(BevelPanel.LOWERED);
    bevelPanel1.add(label2, new XYConstraints(12, 2, 44, -1));
    dialogPanel.add(panel1, new XYConstraints(0, 67, 288, -1));
    panel2.add(btnOK);
    panel2.add(btnCancel);
    dialogPanel.add(bevelPanel1,new XYConstraints(0, 0, 288, 67));
    bevelPanel1.add(txtLogin, new XYConstraints(78, 2, 164, 23));
    bevelPanel1.add(label1, new XYConstraints(12, 30, 64, -1));
    bevelPanel1.add(txtPwd, new XYConstraints(78, 30, 164, -1));
    pack();
    txtLogin.requestFocus();
  }

  //OK
  void btnOK_actionPerformed(ActionEvent e) {
    m_iRes = 1;
    m_sPwd = txtPwd.getText(); m_sLogin = txtLogin.getText();
    dispose();
  }

  //Cancel
  void btnCancel_actionPerformed(ActionEvent e) {
    m_iRes = 0;
    dispose();
  }

  void this_windowClosing(WindowEvent e) {
    dispose();
  }
  public int m_iRes = 0;
  public String m_sPwd, m_sLogin;
  XYLayout xYLayout2 = new XYLayout();
}

class PasswordDialog_btnOK_actionAdapter implements ActionListener{
  PasswordDialog adaptee;

  PasswordDialog_btnOK_actionAdapter(PasswordDialog adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.btnOK_actionPerformed(e);
  }
}

class PasswordDialog_btnCancel_actionAdapter implements ActionListener{
  PasswordDialog adaptee;

  PasswordDialog_btnCancel_actionAdapter(PasswordDialog adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.btnCancel_actionPerformed(e);
  }
}

class PasswordDialog_this_windowAdapter extends WindowAdapter {
  PasswordDialog adaptee;

  PasswordDialog_this_windowAdapter(PasswordDialog adaptee) {
    this.adaptee = adaptee;
  }

  public void windowClosing(WindowEvent e) {
    adaptee.this_windowClosing(e);
  }
}
