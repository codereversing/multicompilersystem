import java.io.BufferedReader; 
import java.io.IOException; 
import java.io.InputStreamReader; 

public class MyTestClass {
  public static void main(String[] args) throws IOException {
    System.out.println("Hello, World! stdout");
    System.err.println("Hello, World! stderr");
    
    BufferedReader reader = new BufferedReader( 
            new InputStreamReader(System.in)); 
  
    for(int i = 0; i < 5; i++)
    {
        String name = reader.readLine(); 
        System.out.println("You entered: " + name);
    }
  }
}
