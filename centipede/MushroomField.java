
package centipede;

import java.util.Random;
import java.awt.*;

public class MushroomField {
    Mushroom field[][] = new Mushroom[18][11];
    private Player player;
    private Centipede game;
    
    public MushroomField(Centipede c, Player p, int userPick){
        player = p;
        game = c;
        createField(userPick);
    }
    
    private void createField(int userPick){
        if(userPick <= 0){
            userPick = 1;
        }
        while(userPick >= 50){
            userPick/=10;
        }
        int randomNum; // Random number from 1 to 1000 
        int check; // Number after being passed through algorithm
        Random rand = new Random();
        int y = 128; // left most mushroom starting location
        for(int j = 0; j < 11; j++){
            int x = 64; // right most mushroom starting location
            for(int i = 0; i < 18; i++){
                randomNum = rand.nextInt(1000)+1; 
                check = ((userPick * randomNum) / 10) % 10;
                if(check == 2){ // increase number of checks for higher probability of placing a mushroom
                    // Make sure the new mushroom is not blocking the centipedes path
                    if(checkPlaceable(i,j)){
                        // place mushroom
                        field[i][j] = new Mushroom(game, 0);
                        field[i][j].setPosition(x, y);
                    }
                    else{
                        field[i][j] = new Mushroom(game, 3); // place a dead mushroom
                        field[i][j].setPosition(x, y);
                    }
                }
                else{
                    field[i][j] = new Mushroom(game, 3); // place a dead mushroom
                    field[i][j].setPosition(x, y);
                }
                x += 64;
            }
            y += 64;
        }
    }
    
    public void displayField(Graphics g){
        for(int i = 0; i < 18; i++){
            for(int j = 0; j < 11; j++){
                field[i][j].displayMushroom(g);
            }
        }
    }
    
    public boolean checkPlaceable(int i, int j){
        if((i == 0) || (j == 0) || (i == 17) || (j == 11)){
            return true;
        }
        if(!(field[i+1][j-1].mushroomDead()) || !(field[i-1][j-1].mushroomDead())){
            return false;
        }
        return true;
    }
    
    public boolean checkHit(int x, int y){
        for(int j = 0; j < 11; j++){
            for(int i = 0; i < 18; i++){
                if(field[i][j].hitMushroom(x, y)){
                    return true;
                }
            }
        }
        return false;
    }
    
    public void healField(){
        for(int j = 0; j < 11; j++){
            for(int i = 0; i < 18; i++){
                field[i][j].healMushroom();
            }
        }
    }
    
    public void commandKill(){
        for(int j = 0; j < 11; j++){
            for(int i = 0; i < 18; i++){
                field[i][j].commandKill();
            }
        }
    }
}
