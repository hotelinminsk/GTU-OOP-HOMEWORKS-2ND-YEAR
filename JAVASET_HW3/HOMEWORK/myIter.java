package HOMEWORK;
import java.security.InvalidKeyException;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * myIterator class which implements Iterator interface
 */
public class myIter<T> implements Iterator<T> {
    private int index;
    private JavaContainer<T> reference;

    /**
     * myIter constructor which is called only from Containers constructors
     * takes 2 parameters 
     * @param _i  private member index get assigned to the taken _i argument
     * @param ref  private member reference gets assigned to the taken ref argument
     */
    public myIter(int _i, JavaContainer ref) {
        reference = ref;
        index = _i;
    }

    @Override
    /**
     * override of hasNext method of Iterator interface 
     * returns true if index is lower than reference's size 
     * and returns false when index is greater than size
     */
    public boolean hasNext() {
        return index < reference.size();
    }
    /***
     * if there is an element in the referenced object's inner array
     * it returns that element
     * @throws NoSuchElementException when there is none element in referenced object's inner array.
     */
    @Override
    public T next() throws NoSuchElementException {
        if (hasNext()) {
            var currentIndex = index;
            index++;

            if (reference instanceof JavaSet) {
                return ((JavaSet<T>) reference).getElementAt(currentIndex);
            } else if (reference instanceof JavaVector) {
                return ((JavaVector<T>) reference).getElementAt(currentIndex);
            }
        } else {
            throw new NoSuchElementException("Iterator can't go out of bounds.");
        }

        return null;
    }
}
