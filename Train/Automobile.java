/** Automobile Class
 * Declared as abstract so the only subclasses can be instantiated
 * @author Philip Papadopoulos
 * @version   16 Feb 2017
 */
public abstract class Automobile implements Item
{
    protected String make, model;
    protected double weight;
    /** Constructor
     *  @param make automobile manufacturer
     *  @param model which model of automobile
     *  @param weight weight of automobile in KG
     */
    public Automobile(String make, String model, double weight)
    {
        this.make = make;
        this.model = model;
        this.weight = weight;
    }
    @Override
    /** Better printing of object
     * @return String describing object
     */
    public String toString() {
        return description();
    }

    // Following two methods fulfill our contract for implementing
    // the Item interface

    /** Getter
     * @return weight of automobile in KG
     */
    public double getWeight() {
        return weight;
    }
    /** Description
     * @return description of automobile
     */
    public String description() {
        return "".format ("%s %s: %f (KG)",make,model,weight);
    }
}
// vim: ts=4:sw=4:et
