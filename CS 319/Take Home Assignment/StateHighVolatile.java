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
 * Description: StateHighVolatile class for stock market simulation.
 */

public class StateHighVolatile extends StateOpen {

    public StateHighVolatile() {
        this.setFee(3.0);
    }
}
