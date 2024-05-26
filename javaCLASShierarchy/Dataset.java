import java.awt.*;
import java.util.ArrayList;
import java.util.List;


public class Dataset {
    private List<MediaObject> items;
    private List<Observer> observers;

    public Dataset(){
        items = new ArrayList<>();
        observers = new ArrayList<>();
    }

    public void add(MediaObject object){
        items.add(object);
        notifyObservers(object,"add");
    }

    public void remove(MediaObject object){
        items.remove(object);
        notifyObservers(object,"remove");
    }

    public void registerObserver(Observer observer){
        observers.add(observer);
    }

    public void unregisterObserver(Observer observer){
        observers.remove(observer);
    }

    private void notifyObservers(MediaObject object,String action){
        for(Observer observer : observers){
            observer.update(object,action);
        }
    }
}
