
package centipede;
import java.awt.*;

public class ScoreBoard {
    private int speed = 1;
    private int lives;
    private int score;
    private boolean gameOver = false;
    public ScoreBoard(){
        lives = 3;
        score = 0;
    }
    
    public void display(Graphics g){
        if(!gameOver){
            g.drawString("Score:", 50, 50);
            g.drawString("Lives:", 1200, 50);
            g.drawString(String.valueOf(score), 50, 65);
            g.drawString(String.valueOf(lives), 1200, 65);
        }
    }
    
    public void updateLives(int update){
        lives = update;
    }
    
    public void updateScore(int update){
        score += update;
    }
    
    public int getScore(){
        return score;
    }
    
    public void endGame(){
        gameOver = true;
    }
}
