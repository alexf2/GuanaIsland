package G_APPL;


public class TIntervalTimer {

  private long startTime = 0;

  public TIntervalTimer() {
   }

   public long start() {
     long delay = System.currentTimeMillis() - startTime;
     startTime = System.currentTimeMillis();
     return delay;
   }
   public String startF() {
     long delay = System.currentTimeMillis() - startTime;
     startTime = System.currentTimeMillis();
     return Long.toString( delay );
   }

   public long elapsed() {
     return System.currentTimeMillis() - startTime;
   }

   public String elapsedF() {
     return Long.toString( System.currentTimeMillis() - startTime );
   }
}
