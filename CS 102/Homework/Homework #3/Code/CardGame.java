package cardgame;

import java.util.ArrayList;
import java.util.Arrays;

// Cardgame
// author:
// date:
public class CardGame
{
    // properties
    Cards             fullPack;
    ArrayList<Player> players;
    ScoreCard         scoreCard;
    Cards[]           cardsOnTable;
    int               roundNo;
    int               turnOfPlayer;
    
    // constructors
    public CardGame( Player p1, Player p2, Player p3, Player p4 )
    {
        players = new ArrayList<>();
        players.addAll( Arrays.asList( p1, p2, p3, p4 ) );
        scoreCard = new ScoreCard( players.size() );
        fullPack = new Cards( true );
        fullPack.shuffle();
        cardsOnTable = new Cards[ players.size() ];
        turnOfPlayer = 0;
        roundNo = 1;

        for ( int d = 0; d < players.size(); d++ ) {
            cardsOnTable[ d ] = new Cards(false);
        }

        for ( int initialCardNum = 0; initialCardNum < 52; initialCardNum++ )
        {
            if ( initialCardNum < 13 ) { p1.add( fullPack.getTopCard() ); }
            else if ( initialCardNum < 13 * 2 ) { p2.add( fullPack.getTopCard() ); }
            else if ( initialCardNum < 13 * 3 ) { p3.add( fullPack.getTopCard() ); }
            else { p4.add( fullPack.getTopCard() ); }
        }
    }
    
    // methods
    public boolean playTurn( Player p, Card c )
    {
        int counter = 0;

        for ( int j = 0; j < players.size(); j++ ) {
            if ( players.get( j ) == p )
                counter = j;
        }

        if ( counter == turnOfPlayer )
            cardsOnTable[ turnOfPlayer ].addTopCard( c );

        if ( turnOfPlayer >= 3 )
            turnOfPlayer = 0;

        roundNo += 1;

        Card checkCard;
        Card[] onTheTableCards = new Card[ players.size() ];
        for ( int d = 0; d < players.size(); d++ ) {
            onTheTableCards[ d ] = new Card(0);
        }

        for( int i = 0; i < 4; i++ ) {

            onTheTableCards[ i ] =  cardsOnTable[ i ].getTopCard();
        }

        checkCard = onTheTableCards[ 0 ];
        for( int i = 0; i < 4; i++) {

            if( onTheTableCards[ i ].compareTo( checkCard ) > 0)
                checkCard = onTheTableCards[ i ];

        }
        for( int i = 0; i < 4; i ++) {

            if( checkCard.compareTo(onTheTableCards[ i ]) == 0)
                scoreCard.update( i, 1 );
        }
        System.out.println(Arrays.toString(cardsOnTable) + "\n");

        return false;
    }
    
    public boolean isTurnOf( Player p )
    {
        return players.get( turnOfPlayer ) == p;
    }
    
    public boolean isGameOver()
    {
        return roundNo >= 14;
    }
    
    public int getScore( int playerNumber )
    {
        if ( playerNumber < 4 && playerNumber > 0 )
        {
            return scoreCard.getScore( playerNumber );
        }
        System.out.println( "No Such Player Founded" );
        return -1;
    }
    
    public String getName( int playerNumber )
    {
        if ( playerNumber < 4 && playerNumber > 0 )
        {
            return players.get( playerNumber ).getName();
        }
        System.out.println( "No Such Player Founded" );
        return null;
    }
    
    public int getRoundNo()
    {
        return roundNo;
    }
    
    public int getTurnOfPlayerNo()
    {
        return turnOfPlayer;
    }
    
    public Player[] getWinners()
    {
        // ToDo
        return null;
    }
    
    public String showScoreCard()
    {
        return scoreCard.toString();
    }

}