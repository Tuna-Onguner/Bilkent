package cardgame;
import java.util.Arrays;
import java.util.Collections;
// ScoreCard - Maintains one integer score per player, for any number of players
//             Caution: invalid playernumbers result in run-time exception!
// author:
// date:
public class ScoreCard
{
    // properties
    int[] scores;
    
    // constructors
    public ScoreCard( int noOfPlayers )
    {
        scores = new int[ noOfPlayers ];
        
        // init all scores to zero
        for ( int i = 0; i < scores.length; i++ )
            scores[ i ] = 0;
    }
    
    // methods
    public int getScore( int playerNo )
    {
        return scores[ playerNo ];
    }
    
    public void update( int playerNo, int amount )
    {
        scores[ playerNo ] += amount;
    }
    
    public String toString()
    {
        String s;
        s = "\n"
            + "_____________\n"
            + "\nPlayer\tScore\n"
            + "_____________\n";
        
        for ( int playerNo = 0; playerNo < scores.length; playerNo++ )
        {
            s = s + playerNo + "\t" + scores[ playerNo ] + "\n";
        }
        
        s += "_____________\n";
        return s;
    }
    
    public int[] getWinners()
    {
        Arrays.sort(scores);
        return null;
    }
    
} // end class ScoreCard
