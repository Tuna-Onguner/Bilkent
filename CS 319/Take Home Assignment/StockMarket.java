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
 * Description: Stock Market class for stock market simulation.
 */

import java.util.LinkedList;

public class StockMarket {

    // Static instances
    private final static MarketState STATE_OPEN = new StateOpen();
    private final static MarketState STATE_LOW_VOLATILE = new StateLowVolatile();
    private final static MarketState STATE_HIGH_VOLATILE = new StateHighVolatile();
    private final static MarketState STATE_CLOSED = new StateClosed();

    // Constant instances
    private final String name;
    private final String symbol;
    private final LinkedList<Stock> stocks;
    private final LinkedList<User> users;

    // Variable instances
    private MarketState state;
    private double dailyTransactionsExpanse;
    private int dailyTransactionsCount;

    // Constructor
    public StockMarket(String name, String symbol) {
        this.name = name;
        this.symbol = symbol;
        this.stocks = new LinkedList<>();
        this.users = new LinkedList<>();
        this.state = STATE_CLOSED;
        System.out.println("Stock Market with name \"" + getRepresentation() + "\" created.");
    }

    // Getters
    public String getName() {
        return name;
    }

    // Other Methods

    /**
     * Get name and symbol of the stock market.
     *
     * @return The representation of the stock market.
     */
    public String getRepresentation() {
        return name + " (" + symbol + ')';
    }

    /**
     * Add a stock to the market.
     *
     * @param stock The stock to be added to the market.
     */
    public void addStock(Stock stock) {

        stock.register(this);
        stocks.add(stock);

        System.out.println(stock.getRepresentation() + " stock with price " + stock.getPrice() +
                ", " + stock.getPercentChange() + " percent change, " + stock.getVolume() + " volume and "
                + stock.getMarketCap() + " market cap is added to " + getRepresentation() + '.');
    }

    /**
     * Open the market.
     *
     * @return True if the market is successfully opened, false otherwise.
     */
    public boolean open() {

        if (state.isOpen()) // If the market is already open, return false.
            return false;

        // If the market is not open, reset the daily transactions.
        dailyTransactionsExpanse = 0;
        dailyTransactionsCount = 0;

        state = STATE_OPEN; // Set the state to open.
        System.out.println(getRepresentation() + " is now open.");

        for (User user : users) {
            user.resetDailyTransactions();
        }

        return true;
    }

    /**
     * Close the market.
     *
     * @return True if the market is successfully closed, false otherwise.
     */
    public boolean close() {

        if (state.isOpen()) { // If the market is open, close it.
            state = STATE_CLOSED; // Set the state to closed.
            System.out.println(getRepresentation() + " is closed.");
            return true;
        }

        // If the market is already closed, return false.
        return false;
    }

    /**
     * Make a transaction to buy a stock.
     *
     * @param stock    The stock to be sold.
     * @param quantity The quantity of the stock to be sold.
     * @param user     The user who is buying the stock.
     * @return True if the transaction is successful, false otherwise.
     */
    public boolean buyStock(Stock stock, final int quantity, User user) {

        if (areTransactionsAllowed(stock, quantity, user))
            return false;

        final double originalCost = stock.getPrice() * quantity,
                fee = originalCost * state.getFee(),
                totalCost = originalCost + fee;

        if (user.getInvestmentBudget() >= totalCost) {

            user.increaseInvestmentBudget(-totalCost);
            user.putPortfolioQuantity(stock.getName(), quantity);

            dailyTransactionsExpanse += totalCost;
            dailyTransactionsCount += 1;

            System.out.println(user.getName() + " has made a transaction to buy " + quantity +
                    " shares of " + stock.getRepresentation() + " while the Stock Market is open.\n" +
                    "Original cost: " + originalCost + " TRY\nFee: " + state.getFee() + "%\n" + "Total cost: " +
                    totalCost + " TRY\n" + user.getName() + " has " + user.getInvestmentBudget() + " TRY left.");

            user.increaseDailyTransactions();
            updateStateOfMarket();

            return true;
        }

        return false;
    }

    /**
     * Make a transaction to sell a stock.
     *
     * @param stock    The stock to be bought.
     * @param quantity The quantity of the stock to be bought.
     * @param user     The user who wants to sell the stock.
     * @return True if the transaction is successful, false otherwise.
     */
    public boolean sellStock(Stock stock, final int quantity, User user) {

        if (areTransactionsAllowed(stock, quantity, user))
            return false;

        final double originalCost = stock.getPrice() * quantity,
                fee = originalCost * state.getFee(),
                totalCost = originalCost + fee;

        if (user.getPortfolioQuantity(stock.getName()) >= quantity) {

            user.increaseInvestmentBudget(totalCost);
            user.putPortfolioQuantity(stock.getName(), -quantity);

            dailyTransactionsExpanse += totalCost;
            dailyTransactionsCount += 1;

            System.out.println(user.getName() + " has made a transaction to sell " + quantity +
                    " shares of " + stock.getRepresentation() + " while the Stock Market is open.\n" +
                    "Original cost: " + originalCost + " TRY\nFee: " + state.getFee() + "%\n" + "Total cost: " +
                    totalCost + " TRY\n" + user.getName() + " has " + user.getInvestmentBudget() + " TRY left.");

            user.increaseDailyTransactions();
            updateStateOfMarket();

            return true;
        }

        return false;
    }

    /**
     * Check if a transaction, either buy or sell, is allowed to a user.
     *
     * @param stock    The stock to be bought.
     * @param quantity The quantity of the stock to be bought.
     * @param user     The user who wants to sell the stock.
     * @return True if the transactions are allowed, false otherwise.
     */
    private boolean areTransactionsAllowed(Stock stock, final int quantity, User user) {

        if (!state.isOpen()) {

            System.out.println(user.getName() + " has made a transaction to buy " + quantity +
                    " shares of " + stock.getRepresentation() + " while the Stock Market is closed.\n" +
                    "Unable to process the transaction.");

            return true;
        }

        if (user.isAllowedToMakeTransaction()) {

            System.out.println(user.getName() + " tried to make their " + (user.getDailyTransactions() + 1) +
                    " transaction of the day," + "however as an " + user.getClass().getName() +
                    " they are only allowed up to " + user.getAllowedDailyTransactions() + '.');

            return true;
        }

        return false;
    }

    /**
     * Update the state of the stock market according to the daily transactions.
     */
    private void updateStateOfMarket() {

        if (state != STATE_HIGH_VOLATILE && dailyTransactionsExpanse > 1000000) {
            state = STATE_HIGH_VOLATILE;
            System.out.println(getRepresentation() + " is now in high volatile state.");
        } else if (state != STATE_LOW_VOLATILE && dailyTransactionsExpanse < 500000 && dailyTransactionsCount > 10) {
            state = STATE_LOW_VOLATILE;
            System.out.println(getRepresentation() + " is now in low volatile state.");
        } else if (state == STATE_LOW_VOLATILE && dailyTransactionsExpanse > 500000) {
            state = STATE_OPEN;
            System.out.println(getRepresentation() + " is now back in the initial state.");
        }
    }

    // Observer methods

    /**
     * Alert all users of the stock market.
     *
     * @param stock The stock that is being changed.
     */
    public void alert(Stock stock) {
        notifyUsers(stock);
    }

    // Subject methods

    /**
     * Register a user to the stock market.
     *
     * @param user The user that is being registered.
     */
    public void register(User user) {
        users.add(user);
        System.out.println(user.getName() + " is now trading on " + getRepresentation() + '.');
    }

    /**
     * Notify all users of the stock market.
     *
     * @param stock The stock that is being changed.
     */
    public void notifyUsers(Stock stock) {
        for (User user : users) {
            user.alert(stock);
        }
    }

    // toString
    @Override
    public String toString() {

        StringBuilder SB = new StringBuilder();
        SB.append("--- Stock Market Information ---\nName: ")
                .append(this.getRepresentation())
                .append("\nStocks:\n");

        if (stocks.isEmpty())
            SB.append("No stocks available.");

        else {
            for (Stock stock : stocks) {
                SB.append('\n')
                        .append(stock.toString())
                        .append('\n');
            }
        }

        return SB.toString();
    }
}
