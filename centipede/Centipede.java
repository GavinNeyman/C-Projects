
package centipede;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.awt.image.BufferedImage;
import java.util.Scanner;

public class Centipede extends JFrame implements Runnable{
    // Maintaining a 4:3 aspect ratio
    public static int WIDTH = 1280;//The width of the frame
    public static int HEIGHT = 1024;//The height of the frame
    private int gameSpeed = 10;
    Graphics offscreen_high;
    BufferedImage offscreen;
    Image backGroundImage = null;
    // Any other images include here
    public int score = 0; 
    ScoreBoard scoreBoard = null;
    Player player = null;
    MushroomField shroom = null;
    CentiSquad centi = null;
    Spider spidey = null;
    private boolean paused = false;
    static public int userInput;

    public Centipede(String frameTitle){
        super(frameTitle); // from JFrame
        addWindowListener (new java.awt.event.WindowAdapter() {
        @Override public void windowClosing(java.awt.event.WindowEvent windowEvent) { System.exit(0);}});
        backGroundImage = new javax.swing.ImageIcon("back3.jpg").getImage();
        // Load other images if needed here
        player = new Player(this);
        addMouseListener(player);
        addMouseMotionListener(player);
        scoreBoard = new ScoreBoard();
        centi = new CentiSquad(this, player);
        shroom = new MushroomField(this, player, userInput);
        spidey = new Spider(this, player);
        offscreen = new BufferedImage(WIDTH, HEIGHT,BufferedImage.TYPE_INT_RGB);
        offscreen_high = offscreen.createGraphics();
        setBackground(Color.black);
        setSize(WIDTH, HEIGHT);
        offscreen_high.drawString("Score", 5, 25);
        Cursor invisibleCursor =
        Toolkit.getDefaultToolkit( ).createCustomCursor(
        Toolkit.getDefaultToolkit( ).getImage(""),
        new Point(0, 0),
        "invisible");
        getContentPane().setCursor(invisibleCursor);
        setVisible(true);
        startGame();
    }
    //==========================================================================
    // Master Thread
    public void startGame() {
        Thread thread = new Thread(this);
        thread.start();
    }
    //==========================================================================
    // Runnable
    public void run() {
        while(true) {
            try {
                Thread.sleep(gameSpeed);
            } catch(InterruptedException ie) {}
            repaint();
        }
    }
    
    public void reset(){
        centi.commandKill();
        centi = new CentiSquad(this, player);
    }
    
    public void death(){
        int check = player.getLives();
        scoreBoard.updateLives(check);
        if(check <= 0){
            // GAME OVER
            spidey.commandKill();
            centi.commandKill();
            shroom.commandKill();
            scoreBoard.endGame();
            for(;;){
                offscreen_high.setFont(new Font("Helvetica", Font.PLAIN, 40)); 
                offscreen_high.drawString("GAME OVER", 500, 200);
                offscreen_high.drawString("Score: ", 500, 300);
                offscreen_high.drawString(String.valueOf(scoreBoard.getScore()), 650, 300);
            }
        }
        else{
            shroom.healField();
            spidey.commandKill();
            centi.commandKill();
            centi = new CentiSquad(this, player);
            spidey = new Spider(this, player);
        }
    }
    //==========================================================================
    // Graphical interface
    
    public void paint(Graphics g) {
        offscreen_high.setColor(Color.black);
        offscreen_high.fillRect(0,0, WIDTH, HEIGHT);
        centi.displaySquad(offscreen_high);
        shroom.displayField(offscreen_high);
        player.displayShip(offscreen_high);
        spidey.displaySpider(offscreen_high);
        scoreBoard.display(offscreen_high);
        g.drawImage(offscreen,0,0,this); 
    }
    public void update(Graphics g) {
        paint(g);
    }
    
    
    //==========================================================================
    // Score Functions
    public void healMushroomScore(){
        scoreBoard.updateScore(10);
    }
    public void hitMushroomScore(){
        scoreBoard.updateScore(1);
    }
    public void hitCentipedeScore(){
        scoreBoard.updateScore(2);
    }
    public void hitSpiderScore(){
        scoreBoard.updateScore(100);
    }
    public void killedSpiderScore(){
        scoreBoard.updateScore(600);
    }
    public void killedSegmentScore(){
        scoreBoard.updateScore(5);
    }
    public void killedCentipedeScore(){
        scoreBoard.updateScore(600);
    }
    public void killedMushroomScore(){
        scoreBoard.updateScore(5);
    }
    
    //==========================================================================
    // Getter Functions
    public MushroomField getMushroomField(){
        return shroom;
    }
    public Spider getSpider(){
        return spidey;
    }
    public CentiSquad getCentipede(){
        return centi;
    }
    public int getWindowHeight(){
        return HEIGHT;
    }
    public int getWindowWidth(){
        return WIDTH;
    }
   
    //==========================================================================
    // Program Entry Point
    public static void main(String[] args) {
        Scanner reader = new Scanner(System.in);  // Reading from System.in
        System.out.println("Enter a number: ");
        int pick = reader.nextInt();
        userInput = pick;
        reader.close();
        Centipede centipede = new Centipede("Centipede");
    }
}
