package cardgame;

public class CardGameTest {

    public static void main( String[] args )
    {
        Player player1 = new Player( "Player1" );
        Player player2 = new Player( "Player2" );
        Player player3 = new Player( "Player3" );
        Player player4 = new Player( "Player4" );

        CardGame letsPlay = new CardGame( player1, player2 ,player3, player4 );

        while ( !letsPlay.isGameOver() ) {

            for ( int PlayerNo = 0; PlayerNo < 4; PlayerNo++ ) {
                letsPlay.playTurn( letsPlay.players.get( PlayerNo ), letsPlay.players.get( PlayerNo ).playCard() );
            }

        }


    }

}
