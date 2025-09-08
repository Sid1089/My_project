import java.util.Scanner;

public class Main {
    
    public static void main(String[]args){

        try (Scanner scanner = new Scanner(System.in)) {
            System.out.print("Enter your name: ");
            String name = scanner.next();
            
            System.out.print("Enter your age:");
            int age = scanner.nextInt();
            
            System.out.print("Enter your weight:");
            double weight = scanner.nextDouble();
            
            
            System.out.print("Enter your favourite colour:");
            String colour = scanner.next();
            
            
            System.out.println("Hello " +name);
            System.out.println("Your are "+ age + " years old");
            System.out.println("Your weight is "+weight);
            System.out.println("Your favourite colour is " +colour);
        }
    }
}
