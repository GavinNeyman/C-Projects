
package centipede;

import java.io.File;
import javax.sound.sampled.*;

public class pewpew {

    public pewpew() throws Exception {
        File soundFile = new File("src/centipede/sounds/pew.wav");
        AudioInputStream sound = AudioSystem.getAudioInputStream(soundFile);
        DataLine.Info info = new DataLine.Info(Clip.class,sound.getFormat());
        Clip clip = (Clip)AudioSystem.getLine(info);
        clip.open(sound);
        clip.start();
    }
}