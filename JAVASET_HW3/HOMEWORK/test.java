package HOMEWORK;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Scanner;

/***
 * Driver test code for JavaContainers
 */
public class test {

    public static void main(String[] args) {
        try {
            Files.createDirectories(Paths.get("HOMEWORK/saves"));

            Path j_vec = Paths.get("HOMEWORK/saves/vectors.txt");
            Path j_set = Paths.get("HOMEWORK/saves/sets.txt");

            try {
                Files.write(j_set, "".getBytes(), StandardOpenOption.CREATE,StandardOpenOption.TRUNCATE_EXISTING);
                Files.write(j_vec, "".getBytes(), StandardOpenOption.CREATE,StandardOpenOption.TRUNCATE_EXISTING);
            } catch (IOException e) {
                System.err.println("Error when opening save file.");
            }

            System.out.println("Testing JavaVector:");
            JavaVector<Integer> vector = new JavaVector<>(20);
            JavaVector<String> vector2 = new JavaVector<String>(20);
            JavaVector<Double> vector3 = new JavaVector<Double>(20);

            vector.add(1);
            vector.add(2);
            vector.add(3);

            vector2.add("Merhaba");
            vector2.add("Dunya");
            vector2.add("Hello world!");

            vector3.add(12.6);
            vector3.add(1.5);
            vector3.add(2.87);

            System.out.println("Vector size: " + vector.size());
            System.out.println("Vector elements: " + vector.toString());

            System.out.println("Vector2 size: " + vector2.size());
            System.out.println("Vector2 elements: " + vector2.toString());

            System.out.println("Vector3 size: " + vector3.size());
            System.out.println("Vector3 elements: " + vector3.toString());

            vector.to_txt(j_vec);
            vector2.to_txt(j_vec);
            vector3.to_txt(j_vec);

            // Test JavaSet
            System.out.println("\nTesting JavaSet:");
            JavaSet<String> set = new JavaSet<>(20);
            JavaSet<Double> set2 = new JavaSet<Double>(20);
            JavaSet<Integer> set3 = new JavaSet<Integer>(20);

            set.add("apple");
            set.add("banana");
            set.add("orange");
            set.add("apple");

            set2.add(22.3);
            set2.add(21.3);
            set2.add(250.25);
            set2.add(22.3);

            set3.add(1);
            set3.add(3);
            set3.add(1);
            set3.add(2);
            set3.add(3);
            set3.add(2);
            set3.add(1);
            set3.add(6);

            System.out.println("Set size: " + set.size());
            System.out.println("Set elements: " + set.toString());
            System.out.println("Set2 size: " + set2.size());
            System.out.println("Set2 elements: " + set2.toString());
            System.out.println("Set3 size: " + set3.size());
            System.out.println("Set3 elements: " + set3.toString());

            set.to_txt(j_set);
            set2.to_txt(j_set);
            set3.to_txt(j_set);

            // Test removal
            set.remove("banana");
            System.out.println("\nSet after removing 'banana': " + set.toString());
            set2.remove(21.3);
            System.out.println("\nSet2 after removing '21.3': " + set2.toString());
            set3.remove(6);
            System.out.println("\nSet3 after removing '6': " + set3.toString());

            // Test removal for vectors
            vector2.remove("Dunya");
            System.out.println("\nVector2 after removing 'Dunya': " + vector2.toString());
            vector.remove(1);
            System.out.println("\nVector1 after removing '1': " + vector.toString());
            vector3.remove(12.6);
            System.out.println("\nVector3 after removing '12.6': " + vector3.toString());

            // Test iterator for set
            System.out.println("\nUsing Iterator for set 1:");
            myIter iterator = set.getIterator();
            while (iterator.hasNext()) {
                System.out.println(iterator.next());
            }
            System.out.println("\n Using Iterator for set 2:");
            iterator = set2.getIterator();
            while (iterator.hasNext()) {
                System.out.println(iterator.next());
            }
            System.out.println("\n Using Iterator for set 3:");
            iterator = set3.getIterator();
            while (iterator.hasNext()) {
                System.out.println(iterator.next());
            }

            // Test iterator for vector
            System.out.println("\n Using Iterator for vector 1");
            iterator = vector.getIterator();
            while (iterator.hasNext()) {
                System.out.println(iterator.next());
            }
            System.out.println("\n Using Iterator for vector 2");
            iterator = vector2.getIterator();
            while (iterator.hasNext()) {
                System.out.println(iterator.next());
            }
            System.out.println("\n Using Iterator for vector 3");
            iterator = vector3.getIterator();
            while (iterator.hasNext()) {
                System.out.println(iterator.next());
            }

            // Interactive user input test
            Scanner scanner = new Scanner(System.in);
            System.out.println("\nInteractive Test: Do you want to perform additional tests? (yes/no)");
            String response = scanner.nextLine().toLowerCase();

            if (response.equals("yes")) {
                System.out.println("Which container do you want to create? (1 for vector, 2 for set)");
                int containerType = scanner.nextInt();
                scanner.nextLine(); // Consume the newline character

                if (containerType == 1) {
                    JavaVector userVector;
                    int choice = 0;
                    System.out.println("Enter the type of the value you wanna add, 1 for Int, 2 for Double,3 for String: ");
                    
                    while (scanner.hasNextInt()) {
                        choice = scanner.nextInt();
                        if(choice == 1){
                            System.out.println("You choose Integer.");
                            break;
                        }else if(choice == 2){
                            System.out.println("You choose Double.");
                            break;
                        }else if(choice == 3){
                            System.out.println("You choose String.");
                            break;
                        }else{
                            System.out.println("Non valid response..");
                        }
                    }
                    scanner.nextLine();
                    System.out.println("Enter the capacity of container : ");
                    int c_cap=10 ;
                    c_cap = scanner.nextInt();
                    scanner.nextLine();
                    
                    String inputType;
                    switch (choice) {
                        case 1:
                                userVector = new JavaVector<Integer>(c_cap);
                                inputType = "Integer";
                                break;
                        case 2:
                                userVector = new JavaVector<Double>(c_cap);
                                inputType = "Double";
                                break;
                        default:
                                userVector = new JavaVector<String>(c_cap);    
                                inputType = "String";
                                break;
                    }
                    

                    System.out.println("Enter the values for the vector (enter a 'stop' to stop): ");
                    Integer int_val;
                    Double d_val;
                    while (true) {
                        String userInput = scanner.nextLine();
                        if (userInput.equalsIgnoreCase("stop")) {
                            break;
                        }
            
                        try {
                            if(choice == 1){
                                int_val = Integer.parseInt(userInput);
                                userVector.add(int_val);
                            }else if(choice == 2){
                                d_val = Double.parseDouble(userInput);
                                userVector.add(d_val);
                            }
                            else{
                                userVector.add(userInput);                                
                            }

                        } catch (NumberFormatException e) {
                            
                            System.out.printf("Invalid input. Please enter an %s or 'stop' to stop.\n",inputType);
                        }
                    }
                    System.out.println("User Vector size: " + userVector.size());
                    System.out.println("User Vector elements: " + userVector.toString());
                
                } else if (containerType == 2) {
                    JavaSet userSet;
                    int choice = 0;
                    System.out.println("Enter the type of the value you wanna add, 1 for Int, 2 for Double,3 for String: ");
                    
                    while (scanner.hasNextInt()) {
                        choice = scanner.nextInt();
                        if(choice == 1){
                            System.out.println("You choose Integer.");
                            break;
                        }else if(choice == 2){
                            System.out.println("You choose Double.");
                            break;
                        }else if(choice == 3){
                            System.out.println("You choose String.");
                            break;
                        }else{
                            System.out.println("Non valid response..");
                        }
                    }

                    scanner.nextLine();
                    System.out.println("Enter the capacity of container : ");
                    int c_cap=10 ;
                    c_cap = scanner.nextInt();
                    scanner.nextLine();
                    
                    String inputType;
                    switch (choice) {
                        case 1:
                                userSet = new JavaSet<Integer>(c_cap);
                                inputType = "Integer";
                                break;
                        case 2:
                                userSet = new JavaSet<Double>(c_cap);
                                inputType = "Double";
                                break;
                        default:
                                userSet = new JavaSet<String>(c_cap);    
                                inputType = "String";
                                break;
                    }
                    

                    System.out.println("Enter the values for the set (enter a 'stop' to stop): ");
                    Integer int_val;
                    Double d_val;
                    while (true) {
                        String userInput = scanner.nextLine();
                        if (userInput.equalsIgnoreCase("stop")) {
                            break;
                        }
            
                        try {
                            if(choice == 1){
                                int_val = Integer.parseInt(userInput);
                                userSet.add(int_val);
                            }else if(choice == 2){
                                d_val = Double.parseDouble(userInput);
                                userSet.add(d_val);
                            }
                            else{
                                userSet.add(userInput);                                
                            }

                        } catch (NumberFormatException e) {
                            
                            System.out.printf("Invalid input. Please enter an %s or 'stop' to stop.\n",inputType);
                        }
                    }
                    System.out.println("User Vector size: " + userSet.size());
                    System.out.println("User Vector elements: " + userSet.toString());
                } else {
                    System.out.println("Invalid choice. Exiting...");
                }
            }

            scanner.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
