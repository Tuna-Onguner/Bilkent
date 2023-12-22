import shapes.*;

import java.awt.*;

/**
 * =====================================================
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * Balloon Class:
 * Creates balloon object that extends Circle
 * Setting Radius to 25 as default
 * Setting selectable true if the radius equals to 100
 * =====================================================
 */
public class Balloon extends Circle implements Drawable {

    public Balloon() {
        super(25);
    }

    /**
     * Growing the circle
     * @return updated (grew) balloon
     */
    public Balloon grow() {

        if (getRadius() == 100) {
            setRadius(0);
            this.setSelected(true);
        }
        else if (getRadius() != 0) {
            setRadius(getRadius() + 1);
        }
        return this;

    }

    @Override
    public void draw(Graphics g) {
        super.draw(g);
        Color random = Main.randomColor();
        g.setColor(random);
        g.fillOval(this.x - super.getRadius(), this.y - super.getRadius(),
                this.getRadius() * 2, getRadius() * 2 );
    }
}
