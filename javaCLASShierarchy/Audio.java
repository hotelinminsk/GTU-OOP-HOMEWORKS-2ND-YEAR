public class Audio extends MediaObject implements Playable{
    private String length;

    public Audio(String name, String info, String length){
        super(name, info);
        this.length = length;
    }

    @Override
    public void info(){
        System.out.println("Audio: " + name + ", Info :  " + info + ", Length : " + length);
    }


}
