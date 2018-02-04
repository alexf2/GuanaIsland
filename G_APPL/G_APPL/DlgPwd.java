package G_APPL;

import java.awt.*;
import com.sun.java.swing.*;

public class DlgPwd extends JDialog {
  JPanel panel1 = new JPanel();
  BorderLayout borderLayout1 = new BorderLayout();

  
  public DlgPwd(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try  {
      jbInit();
      pack();
    }
    catch (Exception ex) {
      ex.printStackTrace();
    }
  }

  
  public DlgPwd() {
    this(null, "", false);
  }

  void jbInit() throws Exception {
    panel1.setLayout(borderLayout1);
    getContentPane().add(panel1);
  }
}

 