/** Electronics
 * @author John David Lin
 * @version 26 Feb 2017
 */

public class Electronics implements Item{
    private String description = "";
    private double weight = 0.00;

    /** Constructor
     * @param description description of object
     * @param weight weight of object
     */
    public Electronics (String description, double weight) {
        this.description = description;
        this.weight = weight;
    }

    /** Getter
     * @return weight of electronics in KG
     */
    public double getWeight() {
        return weight;
    }

    /** Description
     * @return description of electronics
     */
    public String description() {
        return toString();
    }

    @Override
    /** Better printing of object
     * @return String describing object
     */
    public String toString() {
        return "".format ("Electronics: %s (%f KG)",description,weight);
    }
}
