package emtity;

import javafx.beans.property.SimpleStringProperty;

public class StuffPerson {
    private final SimpleStringProperty Stuffid;
    private final SimpleStringProperty Stuffname;
    private final SimpleStringProperty Stufftitle;
    private final SimpleStringProperty Stuffpasswd;
    public StuffPerson(String Stuffid,String Stuffname,String Stufftitle,String Stuffpasswd)
    {
        this.Stuffid = new SimpleStringProperty(Stuffid);
        this.Stuffname = new SimpleStringProperty(Stuffname);
        this.Stufftitle = new SimpleStringProperty(Stufftitle);
        this.Stuffpasswd=new SimpleStringProperty(Stuffpasswd);
    }
    public String getStuffid(){
        return this.Stuffid.get();
    }
    public void setStuffid(String ci) {
        this.Stuffid.set(ci);
    }
    public String getStuffname(){
        return this.Stuffname.get();
    }
    public void setStuffname(String ci) {
        this.Stuffname.set(ci);
    }
    public String getStufftitle(){
        return this.Stufftitle.get();
    }
    public void setStufftitle(String ci) {
        this.Stufftitle.set(ci);
    }
    public String getStuffpasswd(){
        return this.Stuffpasswd.get();
    }
    public void setStuffpasswd(String ci) {
        this.Stuffpasswd.set(ci);
    }
}
