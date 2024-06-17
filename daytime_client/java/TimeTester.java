import java.time.format.DateTimeFormatter;
import java.time.LocalDateTime;

public class TimeTester {
    public static String getTimeInfo()
        {
         DateTimeFormatter dtf = DateTimeFormatter.ofPattern("MM/dd/yyyy hh:mm:ss a");  
         LocalDateTime now = LocalDateTime.now();  
         return dtf.format(now); 
        }

    public static void main(String[] args)
        {
         String timeInfo = getTimeInfo();
         System.out.println(timeInfo);
        }
        
}
