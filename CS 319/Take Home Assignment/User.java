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
 * Description: User class for stock market simulation.
 */

import java.util.Map;
import java.util.HashMap;

public abstract class User {

    // Constant instances
    private final StockMarket stockMarket;
    private final String name;
    private final Map<String, Integer> investmentPortfolio;

    // Variable instances
    private double investmentBudget;
    private int dailyTransactions;
    protected int allowedDailyTransactions;

    // Constructor
    public User(StockMarket stockMarket, String name, double investmentBudget) {
        this.stockMarket = stockMarket;
        this.name = name;
        this.investmentBudget = investmentBudget;
        this.investmentPortfolio = new HashMap<>();
        this.stockMarket.register(this);
    }

    // Getters
    public String getName() {
        return name;
    }

    public double getInvestmentBudget() {
        return investmentBudget;
    }

    public int getPortfolioQuantity(String stockName) {
        return investmentPortfolio.get(stockName);
    }

    public int getDailyTransactions() {
        return dailyTransactions;
    }

    public int getAllowedDailyTransactions() {
        return allowedDailyTransactions;
    }

    // Other methods
    public void putPortfolioQuantity(String stockName, final int quantity) {
        investmentPortfolio.put(stockName, quantity);
    }

    public void increaseInvestmentBudget(double amountOfIncrease) {
        // Note that this method can be also used to decrease the investment budget as well
        // by passing a negative value as the parameter.
        investmentBudget += amountOfIncrease;
    }

    public void increaseDailyTransactions() {
        dailyTransactions += 1;
    }

    public void resetDailyTransactions() {
        dailyTransactions = 0;
    }

    public boolean isAllowedToMakeTransaction() {
        return dailyTransactions > allowedDailyTransactions;
    }

    /**
     * This method is called by the stock market when a stock is changed.
     *
     * @param stock The stock that is changed.
     */
    public void alert(Stock stock) {

        System.out.println(name + " is alerted of change in " + stock.getRepresentation() + '.');

        final int shouldBuyStock = shouldBuyStock(stock);
        if (shouldBuyStock > 0) {
            stockMarket.buyStock(stock, shouldBuyStock, this);
            return;
        }

        if (investmentPortfolio.isEmpty()) {

            System.out.println(name + " does not own any shares so cannot sell "
                    + stock.getRepresentation() + '.');

            return;
        }

        final int shouldSellStock = shouldSellStock(stock);
        if (shouldSellStock > 0)
            stockMarket.sellStock(stock, shouldSellStock, this);
    }

    // Abstract methods
    public abstract int shouldBuyStock(Stock stock);

    public abstract int shouldSellStock(Stock stock);
}
