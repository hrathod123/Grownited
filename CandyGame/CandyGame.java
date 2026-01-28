import java.util.*;

public class CandyGame {
    static int width = 30;
    static int height = 15;
    static int boxX = width / 2, boxW = 6;
    static int candyX = new Random().nextInt(width), candyY = 0;
    static int score = 0, life = 5;

    static void clear() { System.out.print("\033[H\033[2J"); System.out.flush(); }

    static void draw() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (y == candyY && x == candyX) System.out.print("O");
                else if (y == height - 2 && x >= boxX && x < boxX + boxW) System.out.print("#");
                else System.out.print(" ");
            }
            System.out.println();
        }
        System.out.println("Score: " + score + "  Life: " + life + "  (a=left, d=right, q=quit)");
    }

    public static void main(String[] args) throws Exception {
        Scanner sc = new Scanner(System.in);
        Random r = new Random();

        while (life > 0) {
            clear();
            draw();

            long start = System.currentTimeMillis();
            while (System.currentTimeMillis() - start < 250) {
                if (System.in.available() > 0) {
                    char ch = (char) System.in.read();
                    if (ch == 'q') return;
                    if (ch == 'a' && boxX > 0) boxX--;
                    if (ch == 'd' && boxX + boxW < width) boxX++;
                }
            }

            candyY++;
            if (candyY >= height - 2) {
                if (candyX >= boxX && candyX < boxX + boxW) score += 10;
                else life--;
                candyY = 0;
                candyX = r.nextInt(width);
            }
        }
        clear();
        System.out.println("GAME OVER! Final Score: " + score);
    }
}
