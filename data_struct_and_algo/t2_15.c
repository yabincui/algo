// 2.15 Show that X^62 can be computed with only eight multiplications.

#include <limits.h>

#include "logging.h"
#include "utils.h"

int findMinWithPrepare(int n, int prepare) {
	int dp[n + 1];
	dp[0] = 0;
	dp[1] = 0;
	for (int i = 2; i <= n; ++i) {
		if (!(i & (i-1)) && i <= prepare) {
			dp[i] = 0;
			continue;
		}
		int min_cost = INT_MAX;
		for (int j = 1; j < i; ++j) {
			int tmp = dp[j] + dp[i - j] + 1;
			min_cost = min(tmp, min_cost);
		}
		dp[i] = min_cost;
	}
	return dp[n];
}

int findMin(int n) {
	int prepare = 1;
	int min_sum = INT_MAX;
	for (int cost = 0; prepare <= n; cost++, prepare *= 2) {
		int tmp = findMinWithPrepare(n, prepare);
		tmp += cost;
		if (min_sum > tmp) {
			min_sum = tmp;
		}
	}
	return min_sum;
}

int main() {
	int res = findMin(62);
	printf("res = %d\n", res);
	return 0;
}
