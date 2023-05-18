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
 * Description: StateLowVolatile class for stock market simulation.
 */

public class StateLowVolatile extends StateOpen {

    public StateLowVolatile() {
        this.setFee(0.5);
    }
}
