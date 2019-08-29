
package centipede;

import java.awt.*;

public class Bullet implements Runnable{
    private int bulletSpeed = 1;
    private int bulletHeight = 10;
    private int bulletWidth = 5;
    private int x = 0;
    private int y = 0;
    boolean shooting = true;
    Centipede game = null;
    Spider spidey = null;
    MushroomField shrooms = null;
    CentiSquad centi = null;
    
    public Bullet(int xCoord, int yCoord, MushroomField mf, Spider s, CentiSquad c, Centipede g){
        x = xCoord;
        y = yCoord;
        shrooms = mf;
        spidey = s;
        centi = c;
        game = g;
        Thread thread = new Thread(this);
        thread.start();
    }
    public boolean moveBullet(){
        if(spidey.checkHit(x,y)){
            shooting = false;
            return true;
        }
        else if(shrooms.checkHit(x,y)){
            shooting = false;
            return true;
        }
        else if(centi.checkHit(x,y)){
            shooting = false;
            return true;
        }
        
        y -= 2;
        if(y < 0){
            shooting = false;
            return true;
        }
        return false;
    }
    public void displayBullet(Graphics g){
        if(shooting){
            g.setColor(Color.red);
        }
        else{
            g.setColor(Color.black);
        }
        g.fillRect(x, y, bulletWidth, bulletHeight);
    }
    public boolean getBulletStatus(){
       return shooting;
    }
    
    public void run(){
        for(;;){
            try{
                Thread.sleep(bulletSpeed);
            }
            catch(InterruptedException ie){}
            if(moveBullet()){
                game.player.removeBullet(this);
                break;
            }
        }
    }
}
