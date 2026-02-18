import java.util.*;
import java.io.*;
public class SubArraySums2 {
public static void main(String[] args) throws IOException {
BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
StringTokenizer st = new StringTokenizer(br.readLine());
int n = Integer.parseInt(st.nextToken());
long x = Long.parseLong(st.nextToken());
st = new StringTokenizer(br.readLine());
HashMap<Long, Long> prefixCount = new HashMap<>();
prefixCount.put(0L, 1L); // empty prefix has sum 0, seen once
long prefixSum = 0;
long answer = 0;
for (int i = 0; i < n; i++) {
prefixSum += Long.parseLong(st.nextToken());
// How many previous prefixes equal prefixSum - x?
answer += prefixCount.getOrDefault(prefixSum - x, 0L);
// Record this prefix sum
prefixCount.merge(prefixSum, 1L, Long::sum);
}
System.out.println(answer);
}
}