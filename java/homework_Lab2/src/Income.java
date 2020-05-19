import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

public class Income {
    private StringProperty departmentName;
    private StringProperty doctorId;
    private StringProperty doctorName;
    private StringProperty isProfressional;
    private StringProperty register_Number;
    private StringProperty income;
    public Income(String departmentName,String doctorId,String doctorName,String isProfressioanl,String register_number,String income)
    {
        this.departmentName=new SimpleStringProperty(departmentName);
        this.doctorId=new SimpleStringProperty(doctorId);
        this.doctorName = new SimpleStringProperty(doctorName);
        this.isProfressional = new SimpleStringProperty(isProfressioanl);
        this.register_Number = new SimpleStringProperty(register_number);
        this.income = new SimpleStringProperty(income);
    }

    public String isProfressionalProperty() {
        return isProfressional.get();
    }

    public String departmentNameProperty() {
        return departmentName.get();
    }

    public String doctorIdProperty() {
        return doctorId.get();
    }

    public String incomeProperty() {
        return income.get();
    }

    public String doctorNameProperty() {
        return doctorName.get();
    }

    public String register_NumberProperty() {
        return register_Number.get();
    }
}
