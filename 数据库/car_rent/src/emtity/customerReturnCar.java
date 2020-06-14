package emtity;

import javafx.beans.property.SimpleStringProperty;

public class customerReturnCar {
    private final SimpleStringProperty CarId;//汽车牌号
    private final SimpleStringProperty CarBrand;//汽车品牌
    private final SimpleStringProperty CarRent;//租金
    private final SimpleStringProperty CarStatus;//汽车状况
    private final SimpleStringProperty CarPledge;//押金
    private final SimpleStringProperty CarValid;//是否占用
    public customerReturnCar(String id,String brand,String rent,String status,String pledge,String valid)
    {
        this.CarRent = new SimpleStringProperty(rent);
        this.CarPledge=new SimpleStringProperty(pledge);
        this.CarId=new SimpleStringProperty(id);
        this.CarBrand=new SimpleStringProperty(brand);
        this.CarValid=new SimpleStringProperty(valid);
        this.CarStatus=new SimpleStringProperty(status);
    }

    public String getCarBrand() {
        return CarBrand.get();
    }

    public String getCarId() {
        return CarId.get();
    }

    public String getCarPledge() {
        return CarPledge.get();
    }

    public String getCarRent() {
        return CarRent.get();
    }

    public String getCarStatus() {
        return CarStatus.get();
    }

    public String getCarValid() {
        return CarValid.get();
    }
    public void setCarId(String id)
    {
        this.CarId.set(id);
    }
    public void setCarBrand(String s)
    {
        this.CarBrand.set(s);
    }
    public void setCarRent(String s)
    {
        this.CarRent.set(s);
    }
    public void setCarStatus(String statuss)
    {
        this.CarStatus.set(statuss);
    }
    public void setCarPledge(String s)
    {
        this.CarPledge.set(s);
    }
    public void setCarValid(String s)
    {
        this.CarValid.set(s);
    }
}
