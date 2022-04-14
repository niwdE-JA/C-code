#include<stdio.h>
#include<math.h>
unsigned long long int final;
void isPrime(unsigned long long int N,unsigned long long int a, unsigned long long int b) {
    unsigned long long int x = sqrt(N);
    unsigned long long int flag = a;
    unsigned long long int y = b;
//based one the number of primes used
    while(y<=x) {
        if(N%y==0) {
            printf("%llu ;",y);
            printf("No, %llu is NOT a prime number.\n", N);
            return;
        } else {
            y+=flag;
            flag= (flag==4) ? 2: 4;
        }
    }
    printf("Yes, %llu IS a prime number.\n", N );
	final = N;

}
void seekPrime( unsigned long long int check, unsigned long long int next, unsigned long long int bound) {
    unsigned long long int i;
    for(i = check; check <=bound ; check +=check) {
        isPrime (check+1, 4, next);
        isPrime (check-1, 2, next);
    }
}

int main()
{
    seekPrime (2*3*5*7*11*13*17*19*23, 29, 100000000000000000000);
    printf(" largest found:\n");
    printf("%llu", final);
	
    return 0;
}