import java.io.*;
import java.util.*;
public class a {

    public static void main(String[] args) throws NumberFormatException, IOException {
        // TODO Auto-generated method stub
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        PrintWriter out = new PrintWriter(System.out);
        String line = br.readLine();
 
        // Split the line at spaces into tokens (string array)
        String[] tokens = line.split(" ");
        int n = Integer.parseInt(tokens[0]); // games amount
        int k = Integer.parseInt(tokens[1]); // amount of ranges 
        StringBuilder sb = new StringBuilder();
        String streak = br.readLine();
        int[] prefixSum = new int[n + 1];
        int count = 0;
        for(int i = 0; i < n; i++) {
            if(streak.charAt(i) == 'W') {
                count++;
            }
            else {
                count--;
            }
            prefixSum[i+1] = count;
        }
        int z = 0;
        while(z < k) {
            String line1 = br.readLine();
            out.println(line1);
        //     String[] temp = line1.split(" ");
        //     int start = Integer.parseInt(temp[0]);
        //     int end = Integer.parseInt(temp[1]);
        //     int result = prefixSum[end] - prefixSum[start];
        //     if(result >= 0) {
        //         sb.append("Y");
        //     }
        //     else {
        //         sb.append("N");
        //     }
            z++;
        }
        out.println(sb.toString());
        out.flush();
        
          
    }
    
    
 
}