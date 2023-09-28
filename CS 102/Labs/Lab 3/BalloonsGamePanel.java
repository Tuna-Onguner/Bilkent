import java.awt.event.*;
import javax.swing.*;
import java.awt.*;
import shapes.*;

public class BalloonsGamePanel extends JPanel {

    private int points, elapsedTime;
    private final ShapeContainer balloons = new ShapeContainer();
    public final JLabel label = new JLabel();

    public BalloonsGamePanel() {
        setBackground(Main.randomColor());
        points = 0;
        label.setText("Points: " + points);
        this.add(label);
        addMouseListener(mouseListener);
        timer.start();
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        g.setColor(Color.BLACK);
        for (shapes.Shape shape : balloons.getListOfShapes())
            shape.draw(g);
    }

    public ShapeContainer getBalloons() {
        return balloons;
    }

    public Timer timer = new Timer(90, new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            elapsedTime += 1;
            for (int i = 0; i < balloons.getListOfShapes().size(); i++) {
                balloons.getListOfShapes().set(i,
                        ((Balloon)balloons.getListOfShapes().get(i)).grow());
                if (((Balloon) balloons.getListOfShapes().get(i)).getRadius() >= 100)
                    balloons.removeSelected();
            }
            repaint();
            if (balloons.size() < 15) {
                for (int i = 0; i <= 25; i++) {
                    Balloon balloon = new Balloon();
                    balloon.setLocation((int) Math.round(Math.random() * Main.FRAME_WIDTH),
                            (int) Math.round(Math.random() * Main.FRAME_HEIGHT));
                    getBalloons().add(balloon);
                }
                balloons.removeSelected();
            }
            repaint();
            if (elapsedTime > 90) {
                timer.stop();
                int PlayAgainOrNot =
                        JOptionPane.showConfirmDialog(
                                null,
                                "Play Again?",
                                "EndGame",
                                JOptionPane.YES_NO_OPTION,
                                JOptionPane.QUESTION_MESSAGE,
                                null);
                if (PlayAgainOrNot == 1)
                    System.exit(0);
                else {
                    setBackground(Main.randomColor());
                    points = 0;
                    label.setText("Points: " + points);
                    add(label);
                    addMouseListener(mouseListener);
                    timer.start();
                    elapsedTime = 0;
                    balloons.getListOfShapes().clear();
                }

            }
        }
    });

    public MouseListener mouseListener = new MouseListener() {
        final Point point = MouseInfo.getPointerInfo().getLocation();
        @Override
        public void mouseClicked(MouseEvent e) {

        }

        @Override
        public void mousePressed(MouseEvent e) {

            points += balloons.selectAtAll((int) e.getX(), (int) e.getY());
            label.setText("Points: " + points);
            balloons.removeSelected();
            repaint();
        }

        @Override
        public void mouseReleased(MouseEvent e) {

        }

        @Override
        public void mouseEntered(MouseEvent e) {

        }

        @Override
        public void mouseExited(MouseEvent e) {

        }
    };

}