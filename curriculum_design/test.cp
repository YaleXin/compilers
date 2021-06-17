void main(){
   int num1, num2, num3, index;
   num1 = 1;
   num2 = 1;
   index = 3;
   print(num1);
   print(num2);
   while(index < 50){
       num3 = num1 + num2;
       print(num3);
       num1 = num2;
       num2 = num3;
       index = index + 1;
   }
}