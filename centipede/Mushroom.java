
package centipede;

import java.awt.*;

public class Mushroom {
    public static int MUSHROOM_HEIGHT = 64;
    public static int MUSHROOM_WIDTH = 64;
    private int horizPosition = 0;
    private int vertPosition = 0;
    Centipede game = null;
    public int hits;
    
    public Mushroom(Centipede c, int dne){
        hits = dne;
        game = c;
    }
    
    public boolean mushroomDead(){
        if(hits >= 3){
            return true;
        }
        else{
            return false;
        }
    }
    
    public boolean hitMushroom(int x, int y){
        if(mushroomDead()){
            return false;
        }
        
        if ((x >= horizPosition) && (x <= (horizPosition+MUSHROOM_WIDTH))) {
            if ((y >= vertPosition) && (y <= (vertPosition+MUSHROOM_HEIGHT))) {
                game.hitMushroomScore();
                hits++;
                if(hits == 3){
                    game.killedMushroomScore();
                }
                return true;
            }
        } 
        return false;
    }
    
    public void displayMushroom(Graphics g) {
        if (!mushroomDead()) {
            g.setColor(Color.magenta);
            g.fillOval(horizPosition, vertPosition, MUSHROOM_WIDTH, MUSHROOM_HEIGHT);
            //g.fillRect(horizPosition, vertPosition, MUSHROOM_WIDTH, MUSHROOM_HEIGHT);
        }
    }
    
    public void healMushroom(){
        if(!mushroomDead()){
            hits = 0;
        }
    }
    
    public void setPosition(int x, int y) {
        horizPosition = x;
        vertPosition = y;
    }
    
    public int getXPos() {
        return horizPosition;
    }
    
    public int getYPos() {
        return vertPosition;
    }
    
    public void commandKill(){
        hits = 3;
    }
}
