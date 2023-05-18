/*
 * Bilkent University
 * Department of Computer Engineering
 *
 * Course: CS319 (Object-Oriented Software Engineering)
 * The Take Home Assignment
 *
 * Author: Deniz Tuna Onguner
 * Author ID: 22001788
 *
 * Date: 17.06.2023, Wednesday
 * Description: Stock class for stock market simulation.
 */

import java.util.LinkedList;

public class Stock {

    // Constant instances
    private final String name;
    private final String symbol;
    private final LinkedList<StockMarket> markets; // observers

    // Variable instances
    private double price;
    private double percentChange;
    private double volume;
    private double marketCap;

    // Constructor
    public Stock(String name, String symbol, double price, double percentChange, double volume, double marketCap) {
        this.name = name;
        this.symbol = symbol;
        this.price = price;
        this.percentChange = percentChange;
        this.volume = volume;
        this.marketCap = marketCap;
        this.markets = new LinkedList<>();
    }

    // Getters
    public String getName() {
        return name;
    }

    public double getPrice() {
        return price;
    }

    public double getPercentChange() {
        return percentChange;
    }

    public double getVolume() {
        return volume;
    }

    public double getMarketCap() {
        return marketCap;
    }

    // Other methods

    /**
     * Returns the name and symbol of the stock.
     *
     * @return The representation of the stock.
     */
    public String getRepresentation() {
        return name + " (" + symbol + ')';
    }

    // Subject methods

    /**
     * Registers a market to the stock.
     *
     * @param market The market to be registered.
     */
    public void register(StockMarket market) {
        markets.add(market);
    }

    /**
     * Updates the stock information and notifies the markets.
     *
     * @param price         The new price.
     * @param percentChange The new percent change.
     * @param volume        The new volume.
     * @param marketCap     The new market cap.
     */
    public void update(double price, double percentChange, double volume, double marketCap) {

        this.price = price;
        this.percentChange = percentChange;
        this.volume = volume;
        this.marketCap = marketCap;

        System.out.println("Stock " + getRepresentation() + " is updated to price: " + price +
                " TRY, percent change: " + percentChange + ", volume: " + volume + " and market cap: " + marketCap);

        notifyMarkets();
    }

    /**
     * Notifies the registered markets about the updated stock information.
     */
    public void notifyMarkets() {
        for (StockMarket market : markets) {
            market.alert(this);
        }
    }

    // toString
    @Override
    public String toString() {

        return ("--- Stock Information ---\n" +
                "Name: " + name + " (" + symbol + ")\n" +
                "Price: " + price + " TRY\n" +
                "Percent Change: " + percentChange + '\n' +
                "Volume: " + volume + '\n' +
                "Market Cap: " + marketCap);
    }
}
