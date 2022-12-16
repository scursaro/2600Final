
// have some funcitions that check the board,print the board, have a random move, or a player move
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include <C:\Program Files (x86)\mosquitto\devel\mosquitto.h> //mosquitto.h file

void on_connect(struct mosquitto *mosq, void *obj, int rc) { //connects to mqtt
	printf("ID: %d\n", * (int *) obj);
	if(rc) {
		printf("Error with result code: %d\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "test/t1", 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
	printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
}
char board[3][3];
const char PLAYER = 'X';
const char PLAYER2 = '0';
const char COMPUTER = '0';


void resetBoard();
void printBoard(); //print out the new layout
int checkSpace(); ////check if the location is taken, if not place guess, else return to user to pick new spot
void playerEsp();
void player2Move();
//void computerMove();
char checkWinner(); // another function to check if the person on didWin()
void printWinner(char);

int main(){
    int rc, id=12;

	mosquitto_lib_init();

	struct mosquitto *mosq;

	mosq = mosquitto_new("subscribe-test", true, &id);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);
	
	rc = mosquitto_connect(mosq, "localhost", 1883, 10);
	if(rc) {
		printf("Could not connect to Broker with return code %d\n", rc);
		return -1;
	}

	

	//return 0;

    char winner = ' ';

    resetBoard();
//prompt user for game they wish to play
    printf("WELCOME TO TIC-TAC-TOE!\n");
//read information from console
    int option;

    scanf("%d", &option);
    //possibly start the game, maybe a while loop with a nested switch
    //Will also need two separate cases, One for With Player, and One with random 

    if(rc ==true)
    {
        while(winner == ' ' && checkSpace() !=0)
    {
        printBoard();

        playerEsp();//after selection is made, prompt user to enter two int's for their location
        winner = checkWinner();
        if(winner != ' ' || checkSpace() ==0)
        {
            break;

        }
        printBoard();
        player2Move();
        winner = checkWinner();
        if(winner != ' ' || checkSpace() ==0)
        {
            break;

        }
    }
    printBoard();
    printWinner(winner);

    }
   
   
   
   mosquitto_loop_start(mosq);
	printf("Press Enter to quit...\n");
	getchar();
	mosquitto_loop_stop(mosq, true);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
    
    
    return 0;
}

void resetBoard()
{
    for(int i =0; i<3;i++)
    {
        for(int j =0; j < 3; j++)
        {
            board[i][j] = ' ';
        }

    }

}
void printBoard()
{
    printf(" %c | %c | %c", board[0][0], board[0][1], board[0][2]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c", board[1][0], board[1][1], board[1][2]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c", board[2][0], board[2][1], board[2][2]);
    printf("\n---|---|---\n");
    printf("\n");

}
int checkSpace()
{
    int openSpaces =9;

    for(int i =0; i < 3; i++)
    {
        for(int j =0; j <3; j++)
        {
            if(board[i][j] != ' ')
            {
                openSpaces--;
            }

        }
    }
    return openSpaces;


}
void playerEsp()
{
    int x;
    int y;

    do
    {
    printf("Enter a row Player 1(1-3): ");
    scanf("%d", &x);
    x--;
    printf("Enter a col. Player 1(1-3): ");
    scanf("%d", &y);
    y--;

    if(board[x][y] != ' ' )
    {
        printf("Invalid move\n");
    }
    else
    {
        board[x][y] = PLAYER;
        break;

    }

        
    } while (board[x][y] != ' ');
    
    


}
void player2Move()
{
    int x;
    int y;

    do
    {
    printf("Enter a row Player 2(1-3): ");
    scanf("%d", &x);
    x--;
    printf("Enter a col. Player 2 (1-3): ");
    scanf("%d", &y);
    y--;

    if(board[x][y] != ' ' )
    {
        printf("Invalid move\n");
    }
    else
    {
        board[x][y] = PLAYER2;
        break;

    }

        
    } while (board[x][y] != ' ');
    
    


}
/*void computerMove()
{
   srand(time(0));
   int x;
   int y;

   if(checkSpace() > 0)
   {
    do
    {
        x=rand() % 3;
        y =rand() %3;
    } while (board[x][y] != ' ');
    board[x][y] = COMPUTER;
   } 
   else{
    printWinner(' ');
   }


}*/
char checkWinner()
{
    //check the rows
    for(int i =0; i<3; i++ )
    {
        if(board[i][0] == board[i][1] && board[i][0] == board[i][2])
        {
            return board[i][0];
        }
    }

    //check the columns
    for(int i =0; i<3; i++ )
    {
        if(board[0][i] == board[1][i] && board[0][i] == board[2][i])
        {
            return board[0][i];
        }
    }

    //check diagonals
    if(board[0][0] == board[1][1] && board[0][0] == board[2][2])
        {
            return board[0][0];
        }

    if(board[0][2] == board[1][1] && board[0][2] == board[2][0])
        {
            return board[0][2];
        }

    return ' ';




}
void printWinner(char winner)
{
    if(winner == PLAYER)
    {
        printf("Player wins");
    }
    else if(winner == COMPUTER)
    {
        printf("Computer Wins");
    }
    else if(winner == PLAYER2)
    {
        printf("Player2 Wins");
    }
    else
    {
        printf("Y'all trash, y'all tied");
    }

}
