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
 * Description: StateClosed class for stock market simulation.
 */

public class StateClosed implements MarketState {

    @Override
    public boolean isOpen() {
        return false;
    }

    @Override
    public double getFee() { // This method is not expected to be called when the market is closed.
        throw new UnsupportedOperationException();
    }
}
