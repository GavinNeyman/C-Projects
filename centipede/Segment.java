
package centipede;

import java.awt.*;

public class Segment {
    public static int SEGMENT_HEIGHT = 64;
    public static int SEGMENT_WIDTH = 64;
    private int horizPosition = 0;
    private int vertPosition = 0;
    Centipede game = null;
    public int hits;
    
    public Segment(Centipede c, int dne){
        hits = dne;
        game = c;
    }
    
    public boolean segmentDead(){
        if(hits >= 2){
            return true;
        }
        else{
            return false;
        }
    }
    
    public boolean hitSegment(int x, int y, int i){
        if(segmentDead()){
            return false;
        }
        
        if ((x >= horizPosition) && (x <= (horizPosition+SEGMENT_WIDTH))) {
            if ((y >= vertPosition) && (y <= (vertPosition+SEGMENT_HEIGHT))) {
                game.hitCentipedeScore();
                hits++;
                if(hits == 2){
                    game.killedSegmentScore();
                    game.centi.segmentKilled(i, this);
                }
                return true;
            }
        } 
        return false;
    }
    
    public void displaySegment(Graphics g) {
        if (!segmentDead()) {
            g.setColor(Color.green);
            g.fillOval(horizPosition, vertPosition, SEGMENT_WIDTH, SEGMENT_HEIGHT);
            //g.fillRect(horizPosition, vertPosition, SEGMENT_WIDTH, SEGMENT_HEIGHT);
        }
    }
    
    public void segAttack(){
        int playerX = game.player.getX();
        int playerY = game.player.getY();
        if(!segmentDead()){
            if((playerX >= horizPosition) && (playerX <= horizPosition+SEGMENT_WIDTH)){
                if(playerY >= vertPosition && playerY <= vertPosition+SEGMENT_HEIGHT){
                    game.player.badCollision();
                }
            }
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
        hits = 2;
    }
}
