package HOMEWORK;

import javax.management.openmbean.InvalidKeyException;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
public class JavaSet<T> implements JavaContainer<T> {
    private Object[] innerArr;
    private int cap;
    private int size;
    private boolean ordered;
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
        for (int i = 0; i < size; i++) {
            tempArray[i] = innerArr[i];
        }
        innerArr = tempArray;
        iter = new myIter<T>(0, this); // Reset iterator
    }
    /***
     * Function to change inner array's capacity
     * dynamically based on the used size of array
     * if used size gets lower than 1/3 of the capacity we make
     * inner array's capacity twice as lower
     */
    private void getArraySmaller() {
        cap = cap / 2;
        Object[] tempArray = new Object[cap];
        for (var i = 0; i < size; i++) {
            tempArray[i] = innerArr[i];
        }
        innerArr = tempArray;
        iter = new myIter<T>(0, this); // Reset iterator
    }
    

    /***
     * main constructor of our javaset class
     * @param c our innerArray's capacity gets the value of this parameter
     */
    public JavaSet(int c){
        cap = c;
        innerArr = new Object[cap];
        size = 0;
        iter = new myIter<T>(0, this);
    }
    /***
     * we call javaset's parametirezed constructor if the default constructor
     * gets called
     */
    public JavaSet(){
        this(15);
    }
    /**
     * private method to check if given value is exists on our set or not
     * gets called when user tryna add values to the set
     * 
     * @param key value to check
     * @return returns true if value exists in set and returns false if not 
     */
    private boolean is_exists(T key){
        for(int i=0;i<size;i++){
            if(innerArr[i] instanceof Comparable){
                if(innerArr[i].equals(key)){
                    return true;
                }
            }
        }
        return false;
    }
    /***
     * private method to sort the set in increasing order if values are Comparable type
     * this gets called after every add method
     */
    private void incOrder(){
        if(this.innerArr[0] instanceof Comparable){
            for(var i=0;i<size;i++){
                for(var j= i+1;j<size;j++){
                    Comparable<T> comparablei = (Comparable<T>) innerArr[i];
                    if(comparablei.compareTo((T)innerArr[j]) > 0){
                        T temp = (T) innerArr[i];
                        innerArr[i] = innerArr[j];
                        innerArr[j] = temp;
                    }
                    
                }
            }
            ordered = true;
        }else{
            System.err.println("Cant order these elements in increasing way..(Elements are not comparable)");
            ordered = false;
        }
    }

    @Override
    /***
     * override of our add method in JavaSet class
     * if given value is exists in set it we print an error
     * @param val generic value taken from user to add to the set
     */
    public void add(T val){
        if(is_exists(val)){
            System.err.println("Same element exists in the set.");
            return;
        }
        
        if(size > 2* cap/3 ){
            getArrayBigger();
        }

        innerArr[size++] = val;
        if(val instanceof Comparable){
            incOrder();
        }else{
            ordered = false;
        }
    }
    @Override
    /***
     * override of size method in JavaContainer interface
     * returns innerArray's used size
     */
    public int size(){
        return size;
    }
  

    @Override
    /**
     * this function will work more efficiently if the set is in increasing order.
     * if set is in increasing order we do the searching by binarySearch
     * if not we check every value in innerArray one by one to find an exact match
     * @param key The value to remove
     */
    public void remove(T key){
        int midd;
        var end=size;
        var start=0;

        if(is_exists(key)){
            
            if(ordered){
                var index = 0;
                while (start <= end) {
                    midd = (start + end)/2;
                    Comparable<T> c_element = (Comparable<T>)innerArr[midd];
                    if(c_element.compareTo((T)key) > 0){
                        end = midd -1 ;
                    }else if(c_element.compareTo((T)key) < 0){
                        start = midd +1;
                    }
                    else{
                        index = midd;
                        break;
                    }
                }            
                int j = 0;
                Object[] tempAr = new Object[cap];
                for(int i=0;i<size;i++){
                    if(i == index){
                        continue;
                    }else{
                        tempAr[j++] = innerArr[i];
                    }
    
                }
    
                size = size -1;
                innerArr = tempAr;
                if(size < cap/3){
                    getArraySmaller();
                }        
            }
            else{

                Object[] tArray = new Object[cap];
                int j  = 0;
                for(int i= 0 ; i<size;i++){
                    if(innerArr[i].equals(key)){
                        continue;
                    }
                    tArray[j++] = innerArr[i];
                }

                size = size -1 ;
                innerArr = tArray;
                if(size < cap/3){
                    getArraySmaller();
                }
            }

        }else{
            throw new IllegalArgumentException("Can't remove an value doesnt exists in container.");
        }
    }
    /**
     * returns the object at given index, simply an [] indexing operator
     *  
     * @param val an int for wanted value's index
     * @return  returns the value at the given index
     */
    public T getElementAt(int val){
        return (T)innerArr[val];

    }
    @Override
    /**
     * @return myIter object which is an hand created iterator class for JavaContainers.
     */
    public myIter getIterator(){
        return iter; 
    }

    /***
     * Overriding the toString function for our Java Set class
     * Using mutable StringBuilder class to create our string
     * @return  returns the  contents of inner array in string format 
     */
    @Override
    public String toString(){
        StringBuilder sc = new StringBuilder("Java Set: -> ");
        sc.append("[");
        for(var i=0;i<size;i++){
            sc.append(innerArr[i].toString());
            if(i != size-1){
                sc.append(", ");
            }else{
                sc.append(" ");
            }
        }
        sc.append("]");

        return sc.toString();
    }

    
    @Override
    /**
     * Overriding of equals function in JavaContainer interface
     * returns true if given JavaContainer object is an JavaSet object and every element in that set is also
     * an element of our set, if thats the case we check for size because we dont look for subsets we look for an exact match
     * 
     * @param object the JavaContainer object we check for equality
     * @return  returns true if sended object is equal to the our object in given conditions
     *  
     */
    public boolean equals(Object object){
        if(this == object){
            return true;
        }


        boolean same = true;

        if(object instanceof JavaSet){
            JavaSet<T> j_s = (JavaSet<T>)object;
            for(var m=0;m<j_s.size();m++){
                
                if(is_exists(j_s.getElementAt(m))){
                    same = true;
                }else{
                    same = false;
                    break;
                }

            }
            ;

            return same && this.size == j_s.size() ;
        }else{
            return false;
        }
    }
    /**
     * to_txt method to write contents of set to an txt file
     * @param p_set path variable to the txt file
     */
    public void to_txt(Path p_set){
        try {
            StringBuilder st = new StringBuilder("set_");
            st.append("Values");
            byte[] title_byte = st.toString().getBytes();
            Files.write(p_set,title_byte,StandardOpenOption.CREATE,StandardOpenOption.APPEND);
            Files.write(p_set, System.lineSeparator().getBytes(), StandardOpenOption.APPEND);
            Files.write(p_set, this.toString().getBytes(),StandardOpenOption.APPEND);
            Files.write(p_set, System.lineSeparator().getBytes(), StandardOpenOption.APPEND);
        } catch (IOException e) {
            System.err.println("Error when opening set txt file");
            // e.printStackTrace();
        }
    }
}
