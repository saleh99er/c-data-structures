//testing something real quick

int main(){
    n=1

    int array1d[10] = {0,1,2,3,4,5,6,7,8,9};
    int array1dr[9];
    int len = 10;

    for(int j = 0; j < len; j++){
        if(j < n) array1dr[j] = array1d[j];
        else array1dr[j] = array1d[j+1];
    }
}
