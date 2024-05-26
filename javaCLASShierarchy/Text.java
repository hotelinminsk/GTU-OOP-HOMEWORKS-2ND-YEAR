public class Text extends MediaObject implements  NonPlayable{
    public Text(String name,String otherInfo){
        super(name,otherInfo);
    }

    @Override
    public void info(){
        System.out.println("Text: " + name + ", Info :  " + info);
    }
}
