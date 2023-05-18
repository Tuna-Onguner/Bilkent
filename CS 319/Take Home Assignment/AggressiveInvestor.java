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
 * Description: Aggressive Investor class for stock market simulation.
 */

public class AggressiveInvestor extends User {

    // Constructor
    public AggressiveInvestor(StockMarket stockMarket, String name, double investmentBudget) {
        super(stockMarket, name, investmentBudget);
        this.allowedDailyTransactions = 10;
    }

    // Methods

    /**
     * Calculates the quantity of stock to buy.
     *
     * @param stockToBuy Stock object to buy
     * @return quantityToBuy Quantity of stock to buy
     */
    @Override
    public int shouldBuyStock(Stock stockToBuy) {

        int quantityToBuy = 0;
        if (stockToBuy.getPercentChange() < 0) {
            quantityToBuy = (int) (getInvestmentBudget() * 0.1 / stockToBuy.getPrice());
        }

        System.out.println(getName() + " calculated that they should buy " +
                quantityToBuy + " shares of " + stockToBuy.getRepresentation() + '.');

        return quantityToBuy;
    }

    /**
     * Calculates the quantity of stock to sell.
     *
     * @param stockToSell Stock object to sell
     * @return quantityToSell Quantity of stock to sell
     */
    @Override
    public int shouldSellStock(Stock stockToSell) {

        int quantityToSell = 0;
        final int currPortfolioQuantity = getPortfolioQuantity(stockToSell.getName());

        if (stockToSell.getPercentChange() > 0) {
            quantityToSell = (int) (currPortfolioQuantity * stockToSell.getPrice() * 0.12);

            if (quantityToSell > currPortfolioQuantity) {
                quantityToSell = currPortfolioQuantity;
            }
        } else if (stockToSell.getPercentChange() < -2.00) {
            quantityToSell = currPortfolioQuantity;
        }

        System.out.println(getName() + " calculated that they should sell " +
                quantityToSell + " shares of " + stockToSell.getRepresentation() + '.');

        return quantityToSell;
    }
}
