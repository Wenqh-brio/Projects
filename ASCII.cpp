#include<iostream>
using std::cout;
using std::endl;
int main(){
    for(int i=0;i<128;i++){
        cout<<i<<" "<<(char)i<<"   ";
        if(i%8==0){
            cout<<endl;
        }
    }
    cout<<endl;
    return 0;
}