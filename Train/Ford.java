/** Define a few cars that are from the Ford family
 * @author Philip Papadopoulos
 * @version 16 Feb 2017
 */
public class Ford extends Automobile {
    private static final String make = "Ford";
    /** Constructor
     * @param model name of the model that we are constructing
     */
    public Ford(String model) {
        super(make,model,model2weight(model));
    }

    /** map a model name to its weight
     * @return weight in KG of model. Unknown model defaults to 2000.00
     * null model has no weight
     * This MUST be a static method because of how it's used in the
     * constructor
     */

    private static double model2weight(String model) {
        if (model == null) return 0.0;
        switch (model.toLowerCase()) {
            case "mustang": return 1900.00;
            case "focus" : return 1500.00;
            case "excursion": return 3500.00;
        }
        return 2000.00;
    }
}

// vim: ts=4:sw=4:et
