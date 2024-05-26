public abstract class MediaObject {
    protected String name;
    protected String info;

    public MediaObject(String name, String info){
        this.name = name;
        this.info = info;
    }

    public abstract void info();
}
