package emtity;

import javafx.beans.property.SimpleStringProperty;

public class StuffCar {
    private final SimpleStringProperty Carid;
    private final SimpleStringProperty Carbrand;
    private final SimpleStringProperty Carrent_pay;
    private final SimpleStringProperty Carstatus;
    private final SimpleStringProperty Carpledge_pay;
    private final SimpleStringProperty Carvalid;

    public StuffCar(String Carid, String Carbrand, String Carrent, String Carstatus, String Carpledge, String Carvalid) {
        this.Carid = new SimpleStringProperty(Carid);
        this.Carbrand = new SimpleStringProperty(Carbrand);
        this.Carrent_pay = new SimpleStringProperty(Carrent);
        this.Carstatus = new SimpleStringProperty(Carstatus);
        this.Carpledge_pay = new SimpleStringProperty(Carpledge);
        this.Carvalid = new SimpleStringProperty(Carvalid);
    }

    public String getCarbrand() {
        return Carbrand.get();
    }

    public String getCarid() {
        return Carid.get();
    }

    public String getCarpledge_pay() {
        return Carpledge_pay.get();
    }

    public String getCarrent_pay() {
        return Carrent_pay.get();
    }

    public String getCarstatus() {
        return Carstatus.get();
    }

    public String getCarvalid() {
        return Carvalid.get();
    }
    public void setCarbrand(String s)
    {
        this.Carbrand.set(s);
    }
    public void setCarrent_pay(String s)
    {
        this.Carrent_pay.set(s);
    }
    public void setCarstatus(String s)
    {
        this.Carstatus.set(s);
    }
    public void setCarpledge_pay(String s)
    {
        this.Carpledge_pay.set(s);
    }
    public void setCarvalid(String s)
    {
        this.Carvalid.set(s);
    }
}
