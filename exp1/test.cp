int main(){
    char hello[20] = "hello world";
    int f_i = 1;
    int sum = 0;
    while(f_i <= 100){
        sum = sum + f_i * f_i;
        f_i = f_i + 1; 
    }
    return 0;
}