package G_APPL;

import borland.jbcl.dataset.*;
import java.io.Serializable;
import borland.jbcl.util.*;
import borland.sql.dataset.*;

//Usage:
//call Prepare() on client with parameter isServer=false
//pass TParameterRow_ValTransfer on server as parameter
//on server call Load()
//at end of server proc check NeedReturn()
//if true, then call Prepare() with isServer=true and return TParameterRow_ValTransfer to client
public class TParameterRow_ValTransfer implements Serializable {

  public TParameterRow_ValTransfer() {
  }

  public void Load( ParameterRow pr )  throws DataSetException
   {
     if( pr == null || m_var == null || m_var.length == 0 || m_var[0] == null ) return;

     for( int i = m_var.length - 1; i >= 0; --i )
       if( m_var[i] != null ) pr.setVariant( m_iIdx[i], m_var[i] );
       else break;

     m_var = null; m_iIdx = null;
   }
  public void Load( StorageDataSet sd )  throws DataSetException
   {
     Load( StripPRow(sd) );
   }
  public static ParameterRow StripPRow( StorageDataSet sd )  throws DataSetException
   {
     if( sd instanceof QueryDataSet )
       return (ParameterRow)((QueryDataSet)sd).getParameterRow();
     else if( sd instanceof ProcedureDataSet )
       return (ParameterRow)((ProcedureDataSet)sd).getParameterRow();
     else
       throw new DataSetException( DataSetException.GENERIC_ERROR, "TParameterRow_ValTransfer.Prepare() called for invalid class" );
   }
  public void Prepare( ParameterRow pr, boolean isServer ) throws DataSetException
   {
     Column[] clms = pr.getColumns();
     m_var = new Variant[ clms.length ];
     m_iIdx = new int[ clms.length ];
     bNeedReturn = false;
     for( int i = clms.length - 1, j = 0; i >= 0; --i )
      {
        int iTyp = clms[i].getParameterType();
        if( !isServer )//on client
         {
           if( iTyp == ParameterType.OUT || iTyp == ParameterType.RETURN )
            {
              bNeedReturn = true; continue;
            }
           if( iTyp == ParameterType.IN_OUT || iTyp == ParameterType.RESULT ||
               iTyp == ParameterType.NONE
             ) bNeedReturn = true;

           m_iIdx[ j ] = i;
           m_var[ j ] = new Variant();
           pr.getVariant( i, m_var[j] );
           ++j;
         }
        else//on server
         {
           if( iTyp == ParameterType.IN ) continue;
           m_iIdx[ j ] = i;
           m_var[ j ] = new Variant();
           pr.getVariant( i, m_var[j] );
           ++j;
         }
      }
   }
  public void Prepare( StorageDataSet sd, boolean isServer ) throws DataSetException
   {
     if( sd == null ) return;
     ParameterRow pr = StripPRow( sd );
     if( pr == null ) return;

     Prepare( pr, isServer );
   }

  public boolean NeedReturn()
   {
     return bNeedReturn;
   }

  Variant[] m_var;
  int[] m_iIdx;
  private boolean bNeedReturn = false;
}
