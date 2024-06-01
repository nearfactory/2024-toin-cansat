#include <iostream>

using namespace std;

template <typename T, typename U>
int difference(T& arr1, U& arr2){
    int arr1_length = sizeof(arr1)/sizeof(arr1[0]);
    int arr2_length = sizeof(arr2)/sizeof(arr2[0]);

    int min_length = arr1_length < arr2_length ? arr1_length : arr2_length;
    cout << arr1_length << " " << arr2_length << " " << min_length;

    for(int i=0;i<min_length;i++){
        if(arr1[i]!=arr2[i]){
            return 1;
        }
    }

    return 0;
}

int main(){
    int arr1[] = {1,2,3,4,5};
    int arr2[] = {1,2,3,4,5};

    cout << difference(arr1, arr2);

    return 0;
}