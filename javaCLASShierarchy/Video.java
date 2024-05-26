public class Video extends MediaObject implements Playable,Visual{
    private String length;

    public Video(String name, String info, String length){
        super(name, info);
        this.length = length;
    }

    @Override
    public void info(){
        System.out.println("Video: " + name + ", Info :  " + info + ", Length : " + length);
    }

}
