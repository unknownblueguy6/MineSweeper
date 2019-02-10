#include <iostream>

using namespace std;

string red_bg = "\033[1;41m";
string reset = "\033[0m";

void drawGrid(int l, int b, int x, int y){
	cout << "┌" ;
	for(int i = 0; i < l - 1; ++i) cout << "───┬";
	cout << "───┐";
	cout << endl;
	for(int j = 0; j < b; ++j){
        cout << "│";
		for(int k = 0; k < l; ++k){
            cout << " ";
            if(j == y && k == x) cout << red_bg;
            cout << " " << reset << " │";
		}
        if(j != b-1) {
            cout << endl << "├";
            for(int k = 0; k < l-1; ++k) cout << "───┼";
            cout << "───┤";
        }
        cout << endl;
	}
    cout << "└";
    for(int i = 0; i < l - 1; ++i) cout << "───┴";
    cout << "───┘";
	
}

int main(){
    int l, b, x, y;
    cin >> l >> b >> x >> y;
	drawGrid(l, b, x, y);
    cout << endl;
	return 0;
}