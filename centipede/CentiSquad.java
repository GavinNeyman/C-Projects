
package centipede;

import java.awt.*;

import java.util.LinkedList;

public class CentiSquad {
    Centipede game;
    Player player;
    LinkedList<Crawler> squad = new LinkedList<Crawler>();
    int segments; // number of segments currently alive
    int maxSize = 5;
    
    public CentiSquad(Centipede c, Player p){
        game = c;
        player = p;
        segments = 5;
        squad.add(new Crawler(game, player, maxSize, 1280-64, 0));
        segments = 5;
    }
    
    public void displaySquad(Graphics g){
        for(Crawler element : squad){
            element.displayCenti(g);
        }
    }
    
    public boolean checkHit(int x, int y){
        for(Crawler element : squad){
            if(element.checkHit(x,y)){
                return true;
            }
        }
        return false;
    }
    
    public void squadAttack(){
        for(Crawler element : squad){
            element.centiAttack();
        }
    }
    
    public void commandKill(){
        for(Crawler element : squad){
            element.commandKill();
        }
    }
    
    void segmentKilled(int i, Segment s){
        segments-=1;
        if(segments==0){
            game.killedCentipedeScore();
            game.reset();
        }
        else{
            split(i, s);
        }
    }
    
    public void split(int i, Segment s){
        for(Crawler element : squad){
            if(element.hasSegment(s)){
                if(i>0){
                    int firstX = element.getSegX(0);
                    int firstY = element.getSegY(0);
                    Crawler firstHalf = new Crawler(game, player, i, firstX, firstY);
                    firstHalf.movingRight = element.movingRight;
                    element.convertLeftHits(firstHalf);
                    squad.add(firstHalf);
                }
                
                if(i < maxSize-1){
                    int secondX = element.getSegX(i+1);
                    int secondY = element.getSegY(i+1);
                    int size = element.getSize();
                    Crawler secondHalf = new Crawler(game, player, size-1-i, secondX, secondY);
                    secondHalf.movingRight = !element.movingRight;
                    element.convertRightHits(secondHalf);
                    squad.add(secondHalf);
                }
                element.commandKill();
                squad.remove(element);
                return;
            }
        }
    }
}
