/*
 * Bilkent University
 * Department of Computer Engineering
 *
 * Course: CS319 (Object-Oriented Software Engineering)
 * The Take Home Assignment
 *
 * Author: Deniz Tuna Onguner
 * Bilkent ID: 22001788
 *
 * Date: 17.06.2023, Wednesday
 * Description: StateOpen class for stock market simulation.
 */

public class StateOpen implements MarketState {

    private double fee;

    public StateOpen() {
        fee = 1.5;
    }

    public void setFee(double fee) {
        this.fee = fee;
    }

    @Override
    public boolean isOpen() {
        return true;
    }

    @Override
    public double getFee() {
        return fee;
    }
}
