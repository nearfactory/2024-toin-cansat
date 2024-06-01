#include <iostream>
#include <string>

using namespace std;

struct Cursor{
    int x=0,y=0;
};

constexpr int CURSOR_X_MAX = 28;
constexpr int CURSOR_Y_MAX = 11;

string alphabet[28] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","backspace","return"};

/*
string characters[11][5] = {
    {"あ","い","う","え","お"},
    {"か","き","く","け","こ"},
    {"さ","し","す","せ","そ"},
    {"た","ち","つ","て","と"},
    {"な","に","ぬ","ね","の"},
    {"は","ひ","ふ","へ","ほ"},
    {"ま","み","む","め","も"},
    {"や","ゐ","ゆ","ゑ","よ"},
    {"ら","り","る","れ","ろ"},
    {"わ","　","を","　","ん"},
    {"゛", "゜", "削除", "改行", "終了"}
};
*/

int main(){
    static Cursor cursor{0,0};
    static bool button[5]={};   // 0:left 1:up 2:right 3:down 4:center
    static bool previous_button[5]={};
    string input="";
    string output="";

    while(true){
        // simulate button
        input="";
        cin >> input;
        for(int i=0;i<5;i++){
            previous_button[i] = button[i];
            if(input.substr(i,1)=="0") button[i] = false;
            else if(input.substr(i,1)=="1") button[i] = true;
            else cout << "無効な入力" << endl;
        }

        // caliculate cursor
        if(button[0]) cursor.y++; // left
        if(button[1]) cursor.x--; // up
        if(button[2]) cursor.y--; // right
        if(button[3]) cursor.x++; // down
        cursor.x = (cursor.x%CURSOR_X_MAX+CURSOR_X_MAX)%CURSOR_X_MAX;
        cursor.y = (cursor.y%CURSOR_Y_MAX+CURSOR_Y_MAX)%CURSOR_Y_MAX;

        // make string
        if(!button[4] && previous_button[4]){
            if(alphabet[cursor.x]=="backspace" && output.length()>0){
                output.pop_back();
            }else if(alphabet[cursor.x]=="return"){
                output += "\n";
            }else{
                output += alphabet[cursor.x];
            }

            cout << output << endl;
        }

        // output data
        //cout << "x:" << cursor.x << " y:" << cursor.y << " character:" << alphabet[cursor.x] << " output:" << output << endl;
    }

    return 0;
}