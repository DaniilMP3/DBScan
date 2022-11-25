#include <iostream>
#include "geodeb.h"
#include <vector>
#include <cmath>
#include <string>

using namespace std;

#define NOISE 0;
#define BORDER 1;
#define CORE 2;


struct point{
    float x, y;
    bool classified = false;
    int cluster = 0;
};


vector<int> getPointQuery(point *arr, const int &N, const int &point_index, const float &eps){
    vector<int> query;

    const point p = arr[point_index];
    const float x = p.x; const float y = p.y;

    for(int i = 0; i < N; i++){
        if (arr[i].classified){
            continue;
        }
        float x0 = arr[i].x, y0 = arr[i].y;
        float distance = sqrt(powf(x - x0, 2) + powf(y - y0, 2));

        if(distance <= eps){
            query.push_back(i);
        }
    }

    return query;
}

void remove(vector<int> &v, const int &elem){
    for (int i = 0; i < v.size(); i++){
        if (v[i] == elem){
            v.erase(v.begin() + i);
        }
    }
}

void changeStatus(point *arr, vector<int> &query, int cluster){
    for (int i : query){
        arr[i].classified = true;
        arr[i].cluster = cluster;
    }
}



bool expandCluster(point *arr, const int &N, const int point_index, int cluster_id, const float &eps, const int minPts){
    vector<int> pointQuery = getPointQuery(arr, N, point_index, eps);

    if(pointQuery.size() < minPts){
        changeStatus(arr, pointQuery, 0); //change status for query
        remove(pointQuery, point_index);

        return false;
    }
    else{ //core
        changeStatus(arr, pointQuery, cluster_id);
        remove(pointQuery, point_index);

        while(!pointQuery.empty()){
            int currentPoint = pointQuery.front();
            pointQuery.erase(pointQuery.begin());

            vector<int> result = getPointQuery(arr, N, currentPoint, eps); //calculate neighbours for point from stack

            if(result.size() >= minPts){ //if currentPoint is core
                for(int i : result){
                    if(!arr[i].classified || arr[i].cluster == 0){
                        if(!arr[i].classified){
                            pointQuery.push_back(i);
                        }
                        arr[i].cluster = cluster_id;
                    }
                }
            }

            else if(!result.empty() && result.size() < minPts){
                changeStatus(arr, result, cluster_id);
                for (int i : result){
                    pointQuery.push_back(i);
                }

            }

            remove(pointQuery, currentPoint);
        }
        return true;
    }
}


int main() {
    point p1{1, 1, false}, p2{1.5, 1, false}, p3{0, 0, false};
    point p4{0.5, 1.5, false}, p5{0.5, 0.5, false}, p6{2, 0, false}, p7{3, 0, false}, p8{-0.5, -0.5, false};
    point p9{3,0, false};

    point arr[9] = {p1, p2, p3, p4, p5, p6, p7, p8, p9};

    const float eps = 1.0; const int minPts = 3;
    int N = 9;


//    expandCluster(arr, N, point_index, 1, eps, minPts);




    int cluster_id = 1;
    for(int i = 0; i < N; i++){
        if (!arr[i].classified){
            if(expandCluster(arr, N, i, cluster_id, eps, minPts)){
                cluster_id += 1;
            }
        }
    }
    for(point i : arr){
        cout << i.cluster << endl;
    }


    return 0;
}
