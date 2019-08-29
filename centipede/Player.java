
package centipede;

import java.awt.event.*;
import java.awt.*;
import java.util.Iterator;
import javax.swing.ImageIcon;
import java.util.LinkedList;

public class Player implements MouseListener, MouseMotionListener{
    final private Image playerImage = new ImageIcon(("src/centipede/images/player.jpg")).getImage();
    public static int SHIP_HEIGHT = 25;
    public static int SHIP_WIDTH = 15;
    private int x;
    private int y;
    Centipede game = null;
    LinkedList<Bullet> ammo = new LinkedList<Bullet>();
    //Bullet bullet = null;
    boolean hit = false;
    int lives = 3;
    
    public Player(Centipede c) {
        game = c;
        x = (game.WIDTH/2)+(SHIP_WIDTH/2);
        y = game.HEIGHT-SHIP_HEIGHT-20;
    }
    
    public void displayShip(Graphics g) {
        g.setColor(Color.white);
        g.fillRect(x, y, SHIP_WIDTH, SHIP_HEIGHT);
        for(Bullet element : ammo){
            if(element.getBulletStatus()){
                element.displayBullet(g);
            }
        }
        /*
        if ((bullet != null) && (bullet.getBulletStatus())) {
            bullet.displayBullet(g);
        }
        */
    }
    
    public boolean checkHit(int xCoord, int yCoord) {
        if ((xCoord >= x) && (xCoord <= (x+SHIP_WIDTH))) {
            if ((yCoord >= y) && (yCoord <= (y+SHIP_HEIGHT))) {
                hit = true;
                return true;
            }
        } 
        return false;
    }

    public void badCollision() {
        lives -= 1;
        game.death();
    }
    
    public int getLives(){
        return lives;
    }
    
    public int getX(){
        return x;
    }
    
    public int getY(){
        return y;
    }
    
    public void mouseMoved(MouseEvent me){
        int newX = me.getX();
        int newY = me.getY();
        if(newX > (game.WIDTH-SHIP_WIDTH-10)) {
            x = game.WIDTH-SHIP_WIDTH-10;
        } 
        else {
            x = newX;
        }
        if(newY > (game.HEIGHT-SHIP_HEIGHT-10)){
            y = game.HEIGHT-SHIP_HEIGHT-10;
        }
        else{
            y = newY;
        }
        
        //game.spidey.spiderAttack();
        //game.centi.squadAttack();
    }
    public void mouseClicked(MouseEvent me) {
        MushroomField shroom = game.getMushroomField();
        Spider spidey = game.getSpider();
        CentiSquad centi = game.getCentipede();
        ammo.add(new Bullet(x+(int)(SHIP_WIDTH/2), y, shroom, spidey, centi, game));
        try{
            pewpew sound = new pewpew();
        } catch(Exception e){}
    }
    // Unused Mouse Functions
    public void mouseDragged(MouseEvent me) {}
    public void mouseEntered(MouseEvent me) {}
    public void mouseExited(MouseEvent me) {}
    public void mouseReleased(MouseEvent me) {}
    public void mousePressed(MouseEvent me) {}

    void removeBullet(Bullet b) {
        /*
        for(Bullet element : ammo){
            if(element == b){
                ammo.remove(element);
                return;
            }
        }
        */
        Iterator<Bullet> iter = ammo.iterator();

        while (iter.hasNext()) {
            if (iter == b)
                iter.remove();
                return;
        }
        
    }
}
