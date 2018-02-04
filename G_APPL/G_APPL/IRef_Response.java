package G_APPL;

import java.rmi.*;
import java.rmi.server.*;

public interface IRef_Response
 {
   int ResponseMakeRef( RefController rcCtrl );
   void ResponseClearRef( RefController rcCtrl );
 }

