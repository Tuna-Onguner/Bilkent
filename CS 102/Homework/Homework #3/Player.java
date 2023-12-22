package cardgame;

import java.util.Scanner;

// Player - Simple card game player with name and hand of cards
// author:
// date:
public class Player
{
    // properties
    String name;
    Cards hand;
    
    // constructors
    public Player( String name )
    {
        this.name = name;
        hand = new Cards(false);
    }
    
    // methods
    public String getName()
    {
        return name;
    }
    
    public void add( Card c )
    {
        hand.addTopCard( c );
    }
    
    public Card playCard()
    {
        return hand.getTopCard();
    }
    
} // end class Player
