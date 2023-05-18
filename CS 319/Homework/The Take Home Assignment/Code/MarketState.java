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
 * Description: MarketState interface for stock market simulation.
 */

public interface MarketState {

    boolean isOpen(); // returns true if the market is open, false otherwise.

    double getFee(); // returns the fee of the state of the market.
}
