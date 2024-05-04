package HOMEWORK;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

public class JavaVector<T> implements JavaContainer<T>{
    private Object[] innerArray;
    private int size;
    private int cap;
    private myIter iter;
    /***
     * Function to change inner array's capacity
     * dynamically based on the used size of array
     * if used size gets bigger than 2/3 capacity we make
     * inner array's capacity twice as bigger
     */
    private void getArrayBigger() {
        cap = cap * 2;
        Object[] tempArray = new Object[cap];
        for (var i = 0; i < size; i++) {
            tempArray[i] = innerArray[i];
        }
        innerArray = tempArray;
        iter = new myIter<T>(0, this); // Reset iterator
    }
    
    private void getArraySmaller() {
        cap = cap / 2;
        Object[] tempArray = new Object[cap];
        for (var i = 0; i < size; i++) {
            tempArray[i] = innerArray[i];
        }
        innerArray= tempArray;
        iter = new myIter<T>(0, this); // Reset iterator
    }
    
    /**
     * parameterized constructor of our JavaVector class,
     * our innerArray's capacitiy gets assigned to the _c parameter
     * this constructor also creates the iterator of our JavaVector
     * @param _c capacity value
     */
    public JavaVector(int _c){
        cap = _c;
        size = 0;
        innerArray = new Object[cap];
        iter = new myIter<>(0, this);

    }
    /***
     * default constructor of JavaVector
     * calls parameterized constructor with default value of 20
     */
    public JavaVector(){
        this(20);
    }
    @Override
    /**
     *  override of add method in JavaContainer interface
     * @param val  value to add to the JavaVector
     */
    public void add(T val){

        if(size > 2* cap/3  ){
            getArrayBigger();
        }
        innerArray[size++] = val;

    }
    @Override
    /***
     * override of size method in JavaContainer interface
     * returns the usedsize of our innerArray
     */
    public int size(){

        return size;
    }

    /**
     * simply an indexing operator to return the element in given index
     * @param val index of the wanted element
     * @return element on the innerArray at given index
     */
    public T getElementAt(int val){
        return (T)innerArray[val];
    }

    @Override
    /**
     * returns the iterator which is a private member of our class
     *
     */
    public myIter getIterator(){
        return iter;
    }
    /**
     * private is_exist method checks the array if sended value is already exists or not
     * @param key value to check
     * @return returns true if key exists in array returns false if not
     */
    private boolean is_exists(T key){
        for(int i=0;i<size;i++){
            if(innerArray[i] instanceof Comparable){
                if(innerArray[i].equals(key)){
                    return true;
                }
            }
        }
        return false;
    }

    @Override
    /***
     * overrided remove method 
     * removes the key value from vector if it exists in vector
     */
    public void remove(T key){
        if(is_exists(key)){
            var j = 0;
            Object[] tempA = new Object[cap];
            for( int i=0;i<size;i++){
                if(innerArray[i] == key){
                    continue;
                }
                tempA[j++] = innerArray[i];
            }

            size = size-1;
            innerArray = tempA;
            if(size < cap/3){
                getArraySmaller();
            }
        }else{
            System.err.println("Invalid argument.");
        }
    }
    /***
     * equals method checks for two JavaVectors to see if they are equal
     * if user sends same object it returns true if its not the same object
     * method checks for two inner arrays of objects
     * first checks their sizes if they are equal checks for every element in the array
     * if elements are not same nor in the same order it returns false
     * 
     */
    @Override
    public boolean equals(Object object){
        if(this == object){
            return true;
        }

        boolean same = true;

        if(object instanceof JavaVector){
            if(size ==  ((JavaVector)object).size()){
                for(var i = 0; i < ((JavaVector)object).size();i++){
                     if(innerArray[i] != ((JavaVector)object).getElementAt(i)){
                        same = false;
                        break;
                     }
                }

                return same;
            }else{
                return false;
            }

        }




        return same;
    }
    /***
     * Overriding the toString function for our Java Vector class
     * Using mutable StringBuilder class to create our string
     * @return returns the contents of innerarray in string format
     */
    @Override
    public String toString(){
        StringBuilder sc = new StringBuilder("Java Vector: -> ");
        sc.append("[");
        for(int i=0;i<size;i++){
            sc.append(innerArray[i].toString());
            if(i != size-1){
                sc.append(", ");
            }else{
                sc.append(" ");
            }
        }
        sc.append("]");

        return sc.toString();
    }

    /**
     * to_txt method to write contents of vector to an txt file
     * 
     * @param p_set path of the text file to write
     */
    public void to_txt(Path p_set){
        try {
            StringBuilder st = new StringBuilder("vec_");
            st.append("Values");
            byte[] title_byte = st.toString().getBytes();
            Files.write(p_set,title_byte,StandardOpenOption.APPEND);
            Files.write(p_set, System.lineSeparator().getBytes(), StandardOpenOption.APPEND);
            Files.write(p_set, this.toString().getBytes(),StandardOpenOption.APPEND);
            Files.write(p_set, System.lineSeparator().getBytes(), StandardOpenOption.APPEND);
        } catch (IOException e) {
            System.err.println("Error when opening  vec txt file.");
            // e.printStackTrace();
        }
    }

}
