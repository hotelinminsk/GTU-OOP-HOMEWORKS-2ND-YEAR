public class Image extends MediaObject implements  Visual,NonPlayable{
    private String dimension;

    public Image(String name, String info, String dimension){
        super(name, info);
        this.dimension = dimension;
    }

    @Override
    public void info(){
        System.out.println("Image: " + name + ", Info :  " + info + ", Dimension : " + dimension);
    }

}
