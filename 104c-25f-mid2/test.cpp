  for (int max_index = 0; max_index < n; max_index++) {
    for (int capacity = W; capacity >= weights[max_index]; capacity--) {
      dp[capacity] = max(dp[capacity],dp[capacity - weights[max_index]] + values[max_index]);
    }
  }

for (int max_index = 1; max_index <= n; max_index++) {
    for (int zero_capacity = m; zero_capacity >= zero_nums[max_index]; zero_capacity--) {
      for (int one_capacity = n; one_capacity >= one_nums[max_index]; one_capacity--) {
        dp[zero_capacity][one_capacity] = max(dp[zero_capacity][one_capacity], dp[zero_capacity - zeros[max_index]][one_capacity - ones[max_index]]+1);
       }
    }
  }