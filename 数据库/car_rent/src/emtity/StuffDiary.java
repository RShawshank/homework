package emtity;

import javafx.beans.property.SimpleStringProperty;

public class StuffDiary {
    private final SimpleStringProperty Did;
    private final SimpleStringProperty Dcarid;
    private final SimpleStringProperty Dcusid;
    private final SimpleStringProperty Dstuffid;
    private final SimpleStringProperty Dtime;
    private final SimpleStringProperty Devent;
    private final SimpleStringProperty Dcost;
    public StuffDiary(String Did,String Dcarid,String Dcusid,String Dstuffid,String Dtime,String Devent,String Dcost)
    {
        this.Did=new SimpleStringProperty(Did);
        this.Dcarid=new SimpleStringProperty(Dcarid);
        this.Dcusid=new SimpleStringProperty(Dcusid);
        this.Dstuffid=new SimpleStringProperty(Dstuffid);
        this.Dtime=new SimpleStringProperty(Dtime);
        this.Devent=new SimpleStringProperty(Devent);
        this.Dcost=new SimpleStringProperty(Dcost);
    }
    public String getDcost(){
        return this.Dcost.get();
    }
    public void setDcost(String ci) {
        this.Dcost.set(ci);
    }
    public String getDid(){
        return this.Did.get();
    }
    public void setDid(String ci) {
        this.Did.set(ci);
    }
    public String getDcarid(){
        return this.Dcarid.get();
    }
    public void setDcarid(String ci) {
        this.Dcarid.set(ci);
    }
    public String getDcusid(){
        return this.Dcusid.get();
    }
    public void setDcusid(String ci) {
        this.Dcusid.set(ci);
    }
    public String getDstuffid(){
        return this.Dstuffid.get();
    }
    public void setDstuffid(String ci) {
        this.Dstuffid.set(ci);
    }
    public String getDtime(){
        return this.Dtime.get();
    }
    public void setDtime(String ci) {
        this.Dtime.set(ci);
    }
    public String getDevent(){
        return this.Devent.get();
    }
    public void setDevent(String ci) {
        this.Devent.set(ci);
    }
}
