import java.util.Scanner;
import java.math.BigInteger;
public class test {
    public static void main(String[] args) {
        String s;
        Scanner scanner = new Scanner(System.in);
        // Scanner: next() nextInt() nextLine() nextDouble()
        int n = scanner.nextInt();
        BigInteger[] b = new BigInteger[2];
        b[0] = new BigInteger(scanner.next());
        b[1] = new BigInteger(scanner.next());
        BigInteger ans = new BigInteger("0");
        //sort:
        Arrays.sort(b);
        Arrays.sort(b, new comp());
        ans = b[0].add(b[1]);
        //BigInteger: add subtract multiply divide mod isProbablePrime gcd
        System.out.println(b[i]+" "+ans);
        //Output: print println
    }
    static class comp implements Comparator<Integer> {
        public int compare(Integer i1, Integer i2) {
            return i2 > i1;
        }
    }
}
