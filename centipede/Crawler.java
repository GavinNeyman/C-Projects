
package centipede;

import java.awt.*;

public class Crawler implements Runnable {
    int maxSize = 5;
    Segment centi[] = new Segment[maxSize];
    boolean movingRight = false;
    private int centiSpeed = 300; // 100
    private Player player;
    private Centipede game;
    int currSize;
    
    public Crawler(Centipede c, Player p, int n, int startX, int startY){
        game = c;
        player = p;
        currSize = n;
        createCenti(n, startX, startY);
        
        Thread thread = new Thread(this);
        thread.start();
    }
    
    public void run(){
        for(;;){
            try{
                Thread.sleep(centiSpeed);
            }
            catch(InterruptedException ie){}
            
            centiAttack();
            if(moveCenti()){
                break;
            }
        }
    }
    
    public boolean moveCenti(){
        if(centiDead()){
            return true;
        }
        
        //======================================================================
        // Catch centipede segments up
        int x;
        int y;
        for(int i = maxSize-1; i > 0; i--){
            x = centi[i-1].getXPos();
            y = centi[i-1].getYPos();
            centi[i].setPosition(x,y);
        }
        int headX = centi[0].getXPos();
        int headY = centi[0].getYPos();
        //======================================================================
        // Check centipede move direction & final loop
        if(movingRight && (headX >= game.getWindowWidth()-64)){
            if(headY >= game.getWindowHeight()-128){ // in player area loop?
                centi[0].setPosition(headX, headY-64);
                movingRight = false;
                return false;
            }
            else{
                centi[0].setPosition(headX, headY+64);
                movingRight = false;
                return false;
            }
        }
        else if(!movingRight && (headX <= 0)){
            if(headY >= game.getWindowHeight()-128){ // in player area loop?
                centi[0].setPosition(headX, headY-64);
                movingRight = true;
                return false;
            }
            else{
                centi[0].setPosition(headX, headY+64);
                movingRight = true;
                return false;
            }
            
        }
        //======================================================================
        // prefield and postfield
        if(headY<128 || headY>=832){
            if(movingRight){
                centi[0].setPosition(headX+64,headY);
                return false;
            }
            else{
                centi[0].setPosition(headX-64,headY);
                return false;
            }
        }
        
        //======================================================================
        // Move centipede
        if(movingRight){
            if(headX == 1152){
                centi[0].setPosition(headX+64, headY);
            }
            else if(checkMoveable(headX + 64, headY)){
                centi[0].setPosition(headX+64, headY);
            }
            else{
                centi[0].setPosition(headX, headY+64);
                movingRight = false;
            }
        }
        else{
            if(headX == 64){
                centi[0].setPosition(headX-64, headY);
            }
            else if(checkMoveable(headX - 64, headY)){
                centi[0].setPosition(headX-64, headY);
            }
            else{
                centi[0].setPosition(headX, headY+64);
                movingRight = true;
            }
        }
        
        return false; 
    }
    
    public boolean centiDead(){
        for(int i = 0; i < maxSize; i++){
            if(!centi[i].segmentDead()){
                return false;
            }
        }
        return true;
    }
    
    public void centiAttack(){
        for(int i = 0; i < maxSize; i++){
            centi[i].segAttack();
        }
    }
    
    private void createCenti(int n, int x, int y){
        for(int i = 0; i < n; i++){
            centi[i] = new Segment(game, 0);
            centi[i].setPosition(x, y);
            x += 64;
        }
        for(int i = n; i < maxSize; i++){
            centi[i] = new Segment(game, 2);
            centi[i].setPosition(x, y);
            x += 64;
        }
    }
    
    public void displayCenti(Graphics g){
        for(int i = 0; i < maxSize; i++){
            centi[i].displaySegment(g);
        }
    }
    
    public boolean checkMoveable(int x, int y){
        int i = (x / 64) - 1;
        int j = (y / 64) - 2;
        // Check if mushroom blocks way
        if(game.shroom.field[i][j].mushroomDead()){
            return true;
        }
        else{
             return false;
        }     
    }
    
    public boolean checkHit(int x, int y){
        for(int i = 0; i < maxSize; i++){
            if(centi[i].hitSegment(x,y,i)){
                return true;
            }
        }
        return false;
    }

    public void commandKill(){
        for(int i = 0; i < maxSize; i++){
            centi[i].commandKill();
        }
    }
    
    public void convertLeftHits(Crawler c){
        for(int i = 0; i < c.getSize(); i++){
            c.centi[i].hits = centi[i].hits;
        }
    }
    
    public void convertRightHits(Crawler c){
        for(int i = 0; i < c.getSize(); i++){
            c.centi[i].hits = centi[currSize - c.getSize() + i].hits;
        }
    }
    
    public boolean hasSegment(Segment s){
        for(int i = 0; i < maxSize; i++){
            if(centi[i]==s){
                return true;
            }
        }
        return false;
    }
    
    public int getSegX(int i){
        return centi[i].getXPos();
    }
    
    public int getSegY(int i){
        return centi[i].getYPos();
    }
    
    public int getSize(){
        return currSize;
    }
}
