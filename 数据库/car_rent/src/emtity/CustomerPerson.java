package emtity;

import javafx.beans.property.SimpleStringProperty;

//顾客的个人信息
public class CustomerPerson {
    private final SimpleStringProperty customer_id;
    private final SimpleStringProperty customer_name;
    private final SimpleStringProperty customer_member;
    private final SimpleStringProperty customer_dlrq;
    private final SimpleStringProperty customer_moral;
    public CustomerPerson(String ci,String cname,String cm,String cmoral,String dlrq) {
        this.customer_id = new SimpleStringProperty(ci);
        this.customer_name = new SimpleStringProperty(cname);
        this.customer_member = new SimpleStringProperty(cm);
        this.customer_dlrq = new SimpleStringProperty(dlrq);
        this.customer_moral=new SimpleStringProperty(cmoral);
    }

    public String getCustomer_dlrq() {
        return customer_dlrq.get();
    }

    public String getCustomer_id() {
        return customer_id.get();
    }

    public String getCustomer_member() {
        return customer_member.get();
    }

    public String getCustomer_moral() {
        return customer_moral.get();
    }

    public String getCustomer_name() {
        return customer_name.get();
    }
    public void setCustomer_id(String s)
    {
        this.customer_id.set(s);
    }
    public void setCustomer_name(String s)
    {
        this.customer_name.set(s);
    }
    public void setCustomer_member(String s)
    {
        this.customer_member.set(s);
    }
    public void setCustomer_dlrq(String s)
    {
        this.customer_dlrq.set(s);
    }
    public void setCustomer_moral(String s)
    {
        this.customer_moral.set(s);
    }



}
