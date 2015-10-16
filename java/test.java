import java.util.Scanner;
import java.math.BigInteger;
public class test {
    public static void main(String[] args) {
        String s;
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        BigInteger[] b = new BigInteger[100];
        for(int i = 0 ; i < n ; i++){
            b[i] = new BigInteger(scanner.next());
            System.out.println(b[i]);
        }
        BigInteger ans = new BigInteger("0");
        for(int i = 0 ; i < n ; i++){
            ans = ans.add(b[i]);
            //add subtract multiply divide mod isProbablePrime
            System.out.println(b[i]+" "+ans);
        }
    }
}
