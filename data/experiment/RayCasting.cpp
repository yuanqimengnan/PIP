#include <bits/stdc++.h>
#include<ctime>
#include<omp.h>
#include<chrono>
#include<fstream>
#include<iostream>

using namespace std;
typedef long long ll;

const double eps = 1e-6;
const double PI = acos(-1);

int n,m;

struct Point{
    double x,y;
    int id;
    Point(double x=0,double y=0,int id = -1):x(x),y(y),id(id){}
    
    Point operator +(const Point &b)const
    {
        return Point(x+b.x,y+b.y);
    }
    
    Point operator -(const Point &b)const
    {
        return Point(x-b.x,y-b.y);
    }

    double operator *(const Point &b)const
    {
        return x*b.x + y*b.y;
    }

    double operator ^(const Point &b)const
    {
        return x*b.y - b.x*y;
    }
};
typedef Point Vector;

int dcmp(double x)
{
    if(fabs(x)<eps) return 0;
    else
        return x<0?-1:1;
}

bool OnSegment(Point P1,Point P2,Point Q)
{
    return dcmp((P1-Q)^(P2-Q))==0&&dcmp((P1-Q)*(P2-Q))<=0;
}

int InPolygon(Point P, vector<Point> polygon)
{
    bool flag = false;
    Point P1,P2;
    for(int i=1,j=polygon.size();i<=polygon.size();j=i++)
    {
        P1 = polygon[i];
        P2 = polygon[j];
        if(OnSegment(P1,P2,P)) return -1;
        if( (dcmp(P1.y-P.y)>0 != dcmp(P2.y-P.y)>0) && dcmp(P.x - (P.y-P1.y)*(P1.x-P2.x)/(P1.y-P2.y)-P1.x)<0)
            flag = !flag;
    }
    if(flag){
        return P.id;
    }else{
        return -1;
    }
}


int pointInRegion(Point pt,vector<Point> plist)
{
	int nCross = 0;    

	for (int i = 0; i < plist.size(); i++) {  

		Point p1;
		Point p2;

		p1 = plist[i];
		p2 = plist[(i+1)%plist.size()];

		if ( p1.y == p2.y )
			continue;  

		if ( pt.y < min(p1.y, p2.y)) 
			continue; 

		if ( pt.y >= max(p1.y, p2.y))
			continue; 
		double x = (double)(pt.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 
		if ( x > pt.x ) 
			nCross++; 
	} 

	if (nCross % 2 == 1) {

		return pt.id; 
	}
	else {

		return -1; 
	}

}

int main(int argc, char* argv[])
{   
    printf("Point-in test\n\n");
    string PointsFileName = argv[1];
    string PolyFileName = argv[2];
//    string PointsFileName = "/home/liuziang/Desktop/PIP/data/points/testpoint.txt";
//    string PolyFileName = "/home/liuziang/Desktop/PIP/data/polygons/testpolygons.txt";
    ifstream inPointFile(PointsFileName, ios::in);
    ifstream inPolyFile(PolyFileName, ios::in);
    if (!inPointFile.is_open() || !inPolyFile.is_open()) 
    { 
        cout << "未成功打开文件: "<<PointsFileName <<"或者"<<PolyFileName<< endl; 
    } 
    string lineStr;
    vector<Point> points;
    int point_id = 0;
    while(getline(inPointFile,lineStr))
        { 
            istringstream istr(lineStr);
            float x,y;
            istr>>x;
            istr>>y;
            points.push_back({x,y,point_id});
            point_id++;
        }

        getline(inPolyFile,lineStr);
        int PolyNumber = atoi(lineStr.c_str());
        cout<<"PolyNumber: "<< PolyNumber<<endl;
        vector<vector<Point>> polygons;
        while(getline(inPolyFile,lineStr)){
            getline(inPolyFile,lineStr);
            int polys_id,poly_size;
            istringstream istr(lineStr);
            istr>>polys_id>>poly_size;
            vector<Point> temp;
            for(int j = 0; j < poly_size; j++){
                getline(inPolyFile,lineStr);
                istringstream istr(lineStr);
                float x,y;
                istr>>x;
                istr>>y;
                // cout<<"lat: "<<lat<<", lon: "<<lon<<endl;
                temp.push_back({x,y});
            }
            polygons.push_back(temp);
        }

	auto start = std::chrono::high_resolution_clock::now();
	#pragma omp parallel for
        for(int t = 0; t < points.size(); t++)
        {
            for(int p = 0; p < polygons.size(); p++)
            {
                int id = pointInRegion(points[t],polygons[p]);
                if(id!=-1)
                {
        //            cout<<"point " << id << " in polygon " << p <<endl;
                }
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end-start;
        
        string experimentFileName = argv[3];
        std::ofstream file(experimentFileName, std::ios::app);
        if (!file.is_open()) {
            std:cerr << "Failed to open file: " << experimentFileName << std::endl;
            return 1;
        }
        file << duration.count() << std::endl;
        file.close();
        
        std::cout << "Time: " << duration.count() << "ms" << std::endl; 
        cout<<"-----------------------------------------------------------------"<<endl;
        cout<<endl;

    return 0;
}

