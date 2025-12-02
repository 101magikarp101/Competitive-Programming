import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.StringTokenizer;

public class test {

    private static class City {
        private City parent;
        private int size;
        private int rank;

        private City() {
            size = 1;
        }

        private City find() {
            City root = this;
            while (root.parent != null) {
                root = root.parent;
            }
            City curr = this;
            City next = this.parent;
            while (next != null) {
                curr.parent = root;
                curr = next;
                next = next.parent;
            }
            return root;
        }

        private static int union(City a, City b) {
            if (a.rank >= b.rank) {
                if (a.rank == b.rank) {
                    a.rank++;
                }
                b.parent = a;
                a.size += b.size;
                return a.size;
            } else {
                a.parent = b;
                b.size += a.size;
                return b.size;
            }
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        PrintWriter out = new PrintWriter(System.out); // no autoFlush
        StringTokenizer st = new StringTokenizer(br.readLine());

        int n = Integer.parseInt(st.nextToken());
        int m = Integer.parseInt(st.nextToken());

        City[] cities = new City[n + 1];
        for (int i = 1; i <= n; i++) {
            cities[i] = new City();
        }

        int components = n;
        int largest = 1;

        StringBuilder sb = new StringBuilder();

        for (int i = 0; i < m; i++) {
            st = new StringTokenizer(br.readLine());
            City a = cities[Integer.parseInt(st.nextToken())].find();
            City b = cities[Integer.parseInt(st.nextToken())].find();

            if (a != b) {
                components--;
                int size = City.union(a, b);
                if (size > largest) largest = size;
            }

            sb.append(components).append(' ').append(largest).append('\n');
        }

        out.print(sb.toString());
        out.flush();
    }
}
