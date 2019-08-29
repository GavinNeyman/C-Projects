
package centipede;

import java.awt.*;
import java.util.Random;

public class Spider implements Runnable{
    public int hits = 0;
    private int spiderSpeed = 50;
    public static int SPIDER_HEIGHT = 50;
    public static int SPIDER_WIDTH = 50;
    private int horizPosition = 0;
    private int vertPosition = 0;
    //private Image spiderImage = null;
    Player player = null;
    Centipede game = null;
    boolean movingRight = true;
    
    public Spider(Centipede c, Player p){
        //spiderImage = i;
        horizPosition = 0;
        vertPosition = 512;
        player = p;
        game = c;
        Thread thread = new Thread(this);
        thread.start();
    }
    
    public void run(){
        for(;;){
            try{
                Thread.sleep(spiderSpeed);
            }
            catch(InterruptedException ie){}
           
            spiderAttack();
            if(moveSpider()){
                break;
            }
        }
    }
    
    public boolean moveSpider(){
        if(spiderDead()){
            return true;
        }
        //======================================================================
        // Determine if the spider is moving up or down so that it is random and erratic
        Random rand = new Random();
        int randomNum = rand.nextInt(10);
        int check = randomNum % 2; // check if selected a even or odd number
        
        // Check if spider is at a wall && move it approprate way
        //======================================================================
        // Vertical Movement
        if(check == 0){ 
                // movedown if not at max height
                if(vertPosition > (game.getWindowHeight() - (2*SPIDER_HEIGHT))){
                    vertPosition -= SPIDER_HEIGHT/2;
                }
                else{
                    vertPosition += SPIDER_HEIGHT/2;
                }
        }
        else{
            if(vertPosition < (game.getWindowHeight()/2)){
                vertPosition += SPIDER_HEIGHT/2;
            }
            else{
                vertPosition -= SPIDER_HEIGHT/2;
            }
        }
        
        //if(horizPosition+SPIDER_WIDTH > game.getWidth()){
        if(movingRight && (horizPosition > game.getWindowWidth()-64)){
            movingRight = false;
        }
        else if(!movingRight && (horizPosition < 0)){
            movingRight = true;
        }
        
        if(movingRight){
            horizPosition +=SPIDER_WIDTH/2;
        }
        else{
            horizPosition -= SPIDER_WIDTH/2;
        }
        
        return false;
    }
    
    public boolean spiderDead(){
        if(hits >= 2){
            return true;
        }
        return false;
    }
    
    public void spiderAttack(){
        int playerX = player.getX();
        int playerY = player.getY();
        if((playerX >= horizPosition) && playerX <= horizPosition+SPIDER_WIDTH){
            if(playerY >= vertPosition && playerY <= vertPosition+SPIDER_WIDTH){
                player.badCollision();
            }
        }
    }
    
    public boolean checkHit(int x, int y){
        if(spiderDead()){
            return false;
        }
        if ((x >= horizPosition) && (x <= (horizPosition+SPIDER_WIDTH))) {
            if ((y >= vertPosition) && (y <= (vertPosition+SPIDER_HEIGHT))) {
                game.hitSpiderScore();
                hits++;
                if(hits == 2){
                    game.killedSpiderScore();
                }
                return true;
            }
        }
        return false;
    }
    
    public void displaySpider(Graphics g) {
        if (!spiderDead()) {
            g.setColor(Color.yellow);
            g.fillOval(horizPosition, vertPosition, SPIDER_WIDTH, SPIDER_HEIGHT);
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
