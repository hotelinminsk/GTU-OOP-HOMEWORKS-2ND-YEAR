
package HOMEWORK;

/**
 * JavaContainer
 */
public interface JavaContainer<T> {
    
    /**
     * abstract add method
     * which is used to add values to containers
     * @param valueT  value to add to container
     */
    public void add(T valueT);
    /**
     * abstract remove method
     * which is used to remove senden key value from containers
     * @param valueT value to remove
     * 
     */
    public void remove(T keyT);
    /***
     * abstract size method which is 
     * used to return used size in containers
     * @return returns the containers used size
     */
    public int size();
    /**
     * abstract method for returning iterators from containers
     * @return returns the iterator with index 0
     */
    public myIter getIterator();
    /**
     * abstract toString method
     *  @return returns the contents of Container in string form
     */
    public String toString();

}