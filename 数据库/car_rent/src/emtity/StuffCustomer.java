package emtity;

import javafx.beans.property.SimpleStringProperty;

public class StuffCustomer {
    private final SimpleStringProperty Cusid;
    private final SimpleStringProperty Cusname;
    private final SimpleStringProperty Cusmoral;
    private final SimpleStringProperty Cuspasswd;
    private final SimpleStringProperty Cusmember;
    public StuffCustomer(String Cusid,String Cusname,String passwd,String Cusmoral,String m)
    {
        this.Cusid = new SimpleStringProperty(Cusid);
        this.Cusname = new SimpleStringProperty(Cusname);
        this.Cuspasswd = new SimpleStringProperty(passwd);
        this.Cusmoral=new SimpleStringProperty(Cusmoral);
        this.Cusmember=new SimpleStringProperty(m);
    }

    public String getCusid() {
        return Cusid.get();
    }

    public String getCusmember() {
        return Cusmember.get();
    }
public void setCusmember(String s){
        this.Cusmember.set(s);
}
    public String getCusname() {
        return Cusname.get();
    }

    public String getCuspasswd() {
        return Cuspasswd.get();
    }

    public String getCusmoral() {
        return Cusmoral.get();
    }
    public void setCusid(String s)
    {
        this.Cusid.set(s);
    }
    public void setCusname(String s)
    {
        this.Cusname.set(s);
    }
    public void setCusmoral(String s)
    {
        this.Cusmoral.set(s);
    }
    public void setCuspasswd(String s)
    {
        this.Cuspasswd.set(s);
    }
}
