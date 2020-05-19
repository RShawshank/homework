
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;


public class patientController {
    private final StringProperty registerId;
    private final StringProperty patientName;
    private final StringProperty registerDatetime;
    private final StringProperty isProfressional;

    public patientController(String registerId, String patientName, String registerDatetime, String isPro) {
        this.registerId = new SimpleStringProperty(registerId);
        this.patientName = new SimpleStringProperty(patientName);
        this.registerDatetime = new SimpleStringProperty(registerDatetime);
        this.isProfressional = new SimpleStringProperty(isPro);
    }

    public String patientNameProperty() {
        return patientName.get();
    }

    public String isProfressionalProperty() {
        return isProfressional.get();
    }

    public String registerDatetimeProperty() {
        return registerDatetime.get();
    }

    public String registerIdProperty() {
        return registerId.get();
    }
}
