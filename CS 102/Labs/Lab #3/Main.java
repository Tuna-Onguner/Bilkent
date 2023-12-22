import javax.swing.*;
import java.awt.*;
import shapes.*;

public class Main {

    public static final int FRAME_WIDTH  = 720;
    public static final int FRAME_HEIGHT = 800;
    static BalloonsGamePanel balloonsGamePanel = new BalloonsGamePanel();

    public static void main(String[] args) {

        JFrame frame = new JFrame();

        frame.setTitle("Lab03_Part2 - Balloons");
        frame.setSize(FRAME_WIDTH, FRAME_HEIGHT);

        frame.add(balloonsGamePanel);

        frame.setVisible(true);
        frame.setResizable(false);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    }

    public static Color randomColor() {
        return new Color((float)Math.random(),
                         (float)Math.random(),
                         (float)Math.random());
    }

}
