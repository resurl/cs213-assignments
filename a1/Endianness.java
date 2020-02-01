import static java.lang.System.out;
import java.lang.Math;

public class Endianness {

  public static int bigEndianValue (Byte[] mem) {
    // order read mem[0] MSB mem[3] LSB
    int n = 0;
    n = mem[3] & 0xff; // 0x000000ff
    n = n | ((mem[2] & 0xff) << 8); // form of 0x0000ff00
    n = n | ((mem[1] & 0xff) << 16 ); // form of 0x00ff0000
    n = n | (mem[0] << 24); // form of 0xff000000
    return n;
  }
  
  public static int littleEndianValue (Byte[] mem) {
    // order mem[0] LSB mem[3] MSB
    int n = 0;
    n = mem[0] & 0xff; // 0x000000ff
    n = n | ((mem[1] & 0xff) << 8); // 0x0000ff00
    n = n | ((mem[2] & 0xff) << 16); //0x00ff0000
    n = n | (mem[3] << 24); //0xff000000
    return n;
  }
  
  public static void main (String[] args) {
    Byte mem[] = new Byte[4];
    try {
      for (int i=0; i<4; i++)
        mem [i] = Integer.valueOf (args[i], 16) .byteValue();
    } catch (Exception e) {
      out.printf ("usage: java Endianness n0 n1 n2 n3\n");
      out.printf ("where: n0..n3 are byte values in memory at addresses 0..3 respectively, in hex (no 0x).\n");
      return;
    }
  
    int bi = bigEndianValue    (mem);
    int li = littleEndianValue (mem);
    
    out.printf ("Memory Contents\n");
    out.printf ("  Addr   Value\n");
    for (int i=0; i<4; i++)
      out.printf ("  %3d:   0x%-5x\n", i, mem[i]);
    out.printf ("The big    endian integer value at address 0 is %d\n", bi);
    out.printf ("The little endian integer value at address 0 is %d\n", li);
  }
}