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
 * Description: Tester class for stock market simulation.
 */

public class Tester {

    public static void main(String[] args) {

        // Create stock markets
        StockMarket nyse = new StockMarket("New York Stock Exchange", "NYSE");
        StockMarket nasdaq = new StockMarket(
                "National Association of Securities Dealers Automated Quotations", "NASDAQ");

        // Open stock markets
        nyse.open();
        nasdaq.open();

        // Create stocks
        Stock aapl = new Stock("Apple", "AAPL", 100.00,
                3.2, 1000000, 10000);

        Stock sbux = new Stock("Starbucks", "SBUX", 105.90,
                -0.74, 500000, 3000);

        // Add stocks to stock markets
        nyse.addStock(aapl);
        nyse.addStock(sbux);
        nasdaq.addStock(aapl);

        // Create users
        User ege = new BalancedInvestor(nyse, "Hasan Ege Bronze", 1000000);
        User alper = new AggressiveInvestor(nasdaq, "Alper Migrant", 1700000);
        User sarper = new ConservativeInvestor(nyse, "Sarper Copper", 2300000);

        // Stocks to negative percentage change
        aapl.update(50, -1.3, 1000000, 10000);
        sbux.update(60, -1.2, 500000, 3000);

        // Stocks to positive percentage change
        aapl.update(70, 3.11, 1000000, 10000);
        sbux.update(80, 2.22, 500000, 3000);

        // Close market
        nyse.close();

        // Test closed markets not allow transactions
        aapl.update(90, -1.33, 1000000, 10000);

        // Test toString methods
        System.out.println(nyse);
        System.out.println(nasdaq);

        System.out.println(aapl);
        System.out.println(sbux);

        nasdaq.close();
    }
}
