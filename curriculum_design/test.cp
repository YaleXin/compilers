
void main(){
   int num1, num2, num3, index;
   num1 = 1;
   num2 = 1;
   index = 3;
   while(index < 21){
       num3 = num1 + num2;
       num1 = num2;
       num2 = num3;
       index = index + 1;
   }
}
