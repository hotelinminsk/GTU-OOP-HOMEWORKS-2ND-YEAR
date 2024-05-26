public class Main {
    public static void main(String[] args) {
        Dataset ds = new Dataset();

        Player p1 = new Player();
        Player p2 = new Player();
        Viewer v1 = new Viewer();
        Viewer v2 = new Viewer();

        ds.registerObserver(p1);
        ds.registerObserver(p2);
        ds.registerObserver(v1);
        ds.registerObserver(v2);

        ds.add(new Image("imagename1", "dimension info1", "other info1"));
        ds.add(new Image("imagename2", "dimension info2", "other info2"));
        ds.add(new Image("imagename3", "dimension info3", "other info3"));
        ds.add(new Audio("audioname1", "duration1", "other info1"));
        ds.add(new Audio("audioname2", "duration2", "other info2"));
        ds.add(new Video("videoname1", "duration1", "other info1"));
        ds.add(new Video("videoname2", "duration2", "other info2"));
        ds.add(new Text("textname1", "other info1"));
        ds.add(new Text("textname2", "other info2"));

        System.out.println("Player p1 playlist:");
        p1.showPlaylist();

        System.out.println("\nViewer v1 view list:");
        v1.showViewList();

        Playable po = p1.currentlyPlaying();

        if (po != null) {
            System.out.println("\nCurrently playing in p1:");
            po.info();
        }

        NonPlayable np = v1.currentlyViewing();
        if (np != null) {
            System.out.println("\nCurrently viewing in v1:");
            np.info();
        }

        ds.remove((MediaObject) po);
        System.out.println("\nAfter removing the currently playing item from dataset and updating observers:");

        System.out.println("Player p1 playlist:");
        p1.showPlaylist();

        System.out.println("\nViewer v1 view list:");
        v1.showViewList();
    }
}
