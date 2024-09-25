#include <stdio.h>


static int _coins[] = {1, 5, 10, 20, 50, 100};
static int coin_with_greedy(int *coins, int n, int amount);

int main()
{   
    int res = 0;
    res = coin_with_greedy(_coins, sizeof(_coins)/sizeof(_coins[0]), 123);

    printf("res = %d\n", res);

}


static int coin_with_greedy(int *coins, int n, int amount)
{
    int i = n - 1;
    int count = 0;

    while (amount > 0) {
        while (i > 0 && coins[i] > amount) {
            i--;
        }
        printf("amount = %d\n", amount);
        amount -= coins[i];
        count++;
    }
    
    return amount == 0 ? count : -1;
}