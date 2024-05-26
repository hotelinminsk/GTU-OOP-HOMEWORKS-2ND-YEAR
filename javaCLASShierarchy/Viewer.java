import java.util.ArrayList;
import java.util.List;

public class Viewer implements Observer{
    private List<NonPlayable> viewList;
    private int currentViewIndex;

    public Viewer(){
        this.viewList = new ArrayList<>();
        this.currentViewIndex = 0;
    }

    @Override
    public void update(MediaObject object,String action) throws IllegalArgumentException{
        if(object instanceof NonPlayable){
            switch (action){
                case "add":
                    viewList.add((NonPlayable) object);
                    break;
                case "remove":
                    viewList.remove(object);
                    if(currentViewIndex >= viewList.size()){
                        currentViewIndex = 0;
                    }
                    break;
                default:
                    System.out.println("Invalid action");
            }
        }else{
            //System.out.println("Media object is not viewable");
        }
    }

    public void showViewList(){
        for(NonPlayable object : viewList){
            object.info();
        }
    }

    public NonPlayable currentlyViewing() throws IllegalArgumentException{
        if(!viewList.isEmpty()){
            return viewList.get(currentViewIndex);
        }else{
            throw new IllegalArgumentException("View list is empty");
        }
    }

    public void next(String type){
        if(type.equals("text")){
            for(int i = currentViewIndex + 1; i < viewList.size(); i++){
                if(viewList.get(i) instanceof Text){
                    currentViewIndex = i;
                    return;
                }
            }
        }else if(type.equals("image")){
            for(int i = currentViewIndex + 1; i < viewList.size(); i++){
                if(viewList.get(i) instanceof Image){
                    currentViewIndex = i;
                    return;
                }
            }
        }

        currentViewIndex = 0; // if no media is found, view the first media
    }

    public void previous(String type){
        if(type.equals("text")){
            for(int i = currentViewIndex - 1; i >= 0; i--){
                if(viewList.get(i) instanceof Text){
                    currentViewIndex = i;
                    return;
                }
            }
        }else if(type.equals("image")){
            for(int i = currentViewIndex - 1; i >= 0; i--){
                if(viewList.get(i) instanceof Image){
                    currentViewIndex = i;
                    return;
                }
            }
        }
        currentViewIndex = 0; // if no media is found, view the first media
    }


}
