import java.util.Random;
import java.io.*;

public class MMRNG {

	private static int RAND_MAX = 2147483647;
	public static void main(String[] args) {
		Random r = new Random();
    if (args.length != 1)
      System.out.println("Llamar con 'java MMRNG <archivo>'");
    else {
      try {
        BufferedWriter bw = new BufferedWriter(new FileWriter(new File(args[0]), true));
        for(int i=1;i<=100000;i++){
          bw.write(new Integer(r.nextInt(RAND_MAX))+" ");
          if (i%10 == 0)
            bw.newLine();
        }
        bw.close();
      } catch (Exception e) {
      }
    }
	}

}
