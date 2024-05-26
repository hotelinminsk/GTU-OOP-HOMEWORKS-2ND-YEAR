import java.util.ArrayList;
import java.util.List;
public class Player implements Observer {
    private List<Playable> playlist;
    private int currentMediaIndex;

    public Player(){
        this.playlist = new ArrayList<>();
        this.currentMediaIndex = 0;
    }

    @Override
    public void update(MediaObject mediaObject,String action) throws IllegalArgumentException{
        if(mediaObject instanceof Playable){
            switch (action){
                case "add":
                    playlist.add((Playable) mediaObject);
                    break;
                case "remove":
                    playlist.remove(mediaObject);
                    if(currentMediaIndex >= playlist.size()){
                        currentMediaIndex = 0;
                    }
                    break;
                default:
                    throw new IllegalArgumentException("Invalid action");
            }
        }else{
            //System.out.println("Media object is not playable");
        }
    }

    public void showPlaylist(){
        for(Playable mediaObject : playlist){
            mediaObject.info();
        }
    }

    public Playable currentlyPlaying() throws IllegalArgumentException{
        if(!playlist.isEmpty()){
            return playlist.get(currentMediaIndex);
        }else{
            throw new IllegalArgumentException("Playlist is empty");
        }
    }

    public void next(String type){
        if(type.equals("audio")){
            for(int i = currentMediaIndex + 1; i < playlist.size(); i++){
                if(playlist.get(i) instanceof Audio){
                    currentMediaIndex = i;
                    return;
                }
            }
        }else if(type.equals("video")){
            for(int i = currentMediaIndex + 1; i < playlist.size(); i++){
                if(playlist.get(i) instanceof Video){
                    currentMediaIndex = i;
                    return;
                }
            }
        }
        currentMediaIndex = 0; // if no media is found, play the first media
    }

    public void previous(String type){
        if(type.equals("audio")){
            for(int i = currentMediaIndex - 1; i >= 0; i--){
                if(playlist.get(i) instanceof Audio){
                    currentMediaIndex = i;
                    return;
                }
            }
        }else if(type.equals("video")){
            for(int i = currentMediaIndex - 1; i >= 0; i--){
                if(playlist.get(i) instanceof Video){
                    currentMediaIndex = i;
                    return;
                }
            }
        }
        currentMediaIndex =0; // if no media is found, play the first media
    }



}
